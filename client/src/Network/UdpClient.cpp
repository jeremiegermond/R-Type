#include "network.hpp"

UdpClient::UdpClient()
    : pfd{new pollfd}, sockaddrIn{}, socklen{}, heartbeat{}, listener{}, server_ip{}, server_port{}, _isAlive{-1}, shutdown{false}, requests{},
      _generator{}, connected{false}, session{} {
    pfd->fd = socket(AF_INET, SOCK_DGRAM, 0);
    pfd->events = POLLIN;
    if (pfd[0].fd < 0) {
        std::cout << "socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    sockaddrIn.sin_family = AF_INET;
    socklen = sizeof(sockaddrIn);
    auto id = _generator.getUniqueId();
    std::cout << "id session: " << id << std::endl;
    session = new Session(id);
}

bool UdpClient::connect(const std::string &ip, unsigned short port) { // to do : add option to set ip and port
    _isAlive = 0;
    connected = false;
    server_ip = ip;
    server_port = port;
    sockaddrIn.sin_port = htons(server_port);
    sockaddrIn.sin_addr.s_addr = inet_addr(server_ip.data());
    if (listener.joinable())
        listener.join();
    listener = std::thread(&UdpClient::loopListener, this);
    std::cout << "Connecting to server" << std::endl;
    for (int i = 0; i < 15; ++i) { // attempt to connect 15 times
        if (sendHeartbeat(true))   // put in a thread to avord blocking
            break;
    }
    if (alive()) {
        std::cout << "Connected to server" << std::endl;
        heartbeat = std::thread(&UdpClient::loopHeartbeat, this);
        // shutdown = false;
        sendConnect(); // put in a thread to avord blocking
        return true;
    } else {
        std::cout << "Failed to connect to server" << std::endl;
        shutdown = true;
        return false;
    }
}

void UdpClient::sendRequest(const std::string &data) {
    auto id = _generator.getUniqueId();
    requests[id] = new UdpRequest(data);
    send(std::to_string(id) + ":" + data, true);
}

void UdpClient::send(std::string data, bool showDebug) {
    sendto(pfd[0].fd, data.data(), data.size(), MSG_CONFIRM, (const struct sockaddr *)&sockaddrIn, socklen);
    if (showDebug)
        std::cout << "Sent:" << data << std::endl;
}

requestMap UdpClient::get_requests() { return requests; }

void UdpClient::popRequest(int id) {
    if (requests.find(id) == requests.end())
        return;
    delete requests[id];
    requests.erase(id);
}

void UdpClient::loopListener() {
    while (true) {
        if (shutdown) {
            std::cout << "Shutting down listener" << std::endl;
            return;
        }
        if (poll(pfd, 1, 1000) > 0)
            receive(true);
    }
}

void UdpClient::receive(bool showDebug) {
    std::string buffer;
    buffer.resize(1024);
    auto n = recvfrom(pfd[0].fd, (char *)buffer.data(), 1024, MSG_WAITALL, (struct sockaddr *)&sockaddrIn, &socklen);
    buffer.resize(n);
    handleRequest(buffer);
}

void UdpClient::handleRequest(const std::string &data) {
    if (data.empty()) {
        std::cout << "Empty data" << std::endl;
        return;
    }
    std::string requestType = data.substr(0, data.find_first_of(':'));
    if (requestType.empty()) {
        std::cout << "Empty request type" << std::endl;
        return;
    }

    if (requestType == "heartbeat") {
        _isAlive = 1;
    } else if (requestType == "connect") {
        connected = true;
        auto res = split(data, ':');
        for (auto i = res.begin() + 2; i != res.end(); i++) {
            auto opt = split(*i, ',');
            Vector2 pos = {std::stof(opt[1]), std::stof(opt[2])};
            session->addPlayer(new Player(std::stoi(opt[0]), pos, std::stoi(opt[3])));
        }
    } else if (requestType == "connexion") {
        session->addPlayer(new Player(std::stoi(data.substr(data.find_first_of(':') + 1))));
    } else if (requestType == "deconnexion") {
        session->removePlayer(std::stoi(data.substr(data.find_first_of(':') + 1)));
    } else if (requestType.c_str()[0] == '[') {
        std::cout << data << std::endl;
        auto playerId = std::stoi(data.substr(1, data.find_first_of(']') - 1));
        requestType = data.substr(data.find_first_of(']') + 1, data.find_first_of(':') - data.find_first_of(']') - 1);
        if (requestType == "_position") { // HAS to be optimized
            auto x = data.substr(data.find_first_of(':') + 1, data.find_first_of(',') - data.find_first_of(':') - 1);
            auto y = data.substr(data.find_first_of(',') + 1);
            Vector2 pos = {std::stof(x), std::stof(y)};
            auto player = session->getPlayer(playerId);
            if (player != nullptr)
                player->setPos(pos);
            else
                session->addPlayer(new Player(playerId, pos, 100)); // _hp may have to be changed
        }
        //} else if (requestType == "disconnect") { to be added
        //    session->removePlayer(playerId);
        //}
    } else if (requestType == "disconnect") {
        std::cout << "Server disconnected" << std::endl;
        connected = false;
    } else {
        std::cout << "Received:" << data << std::endl;
        auto id = std::stoi(requestType);
        // to avoid setting on a deleted request (if the request is deleted before the response is set)
        // the request should be deleted in the same thread as the response is set
        // (the thread that calls the receive function)

        if (requests.find(id) != requests.end()) {
            requests[id]->setResponse(data);
        }
    }
}

bool UdpClient::disconnected() { return !connected; }

void UdpClient::loopHeartbeat() {
    static int count = 0;
    while (!shutdown) {
        if (connected) {
            if (!sendHeartbeat())
                count++;
            else
                count = 0;
            if (count > 5) {
                std::cout << "Server disconnected" << std::endl;
                connected = false;
                break;
            }
        }
    }
    shutdown = true;
    std::cout << "Shutting down heartbeat" << std::endl;
}

bool UdpClient::sendConnect() {
    // send(first_connection ? "connect:" + std::to_string(session->getPlayerId()) : "heartbeat");
    std::cout << "connect:" + std::to_string(session->getPlayerId()) << std::endl;
    send("connect:" + std::to_string(session->getPlayerId()));
    auto to = std::chrono::system_clock::now() + 5s; // timeout val
    while (std::chrono::system_clock::now() < to) {
        if (connected) {
            std::cout << "Successfully connected to server" << std::endl;
            return true;
        }
    }
    std::cout << "Couldn't connect to server" << std::endl;
    _isAlive = -1;
    return false;
}

bool UdpClient::sendHeartbeat(bool first_connection) {
    send("heartbeat");
    if (_isAlive == 1)
        _isAlive = 0;
    auto to = std::chrono::system_clock::now() + 1s; // timeout val
    while (std::chrono::system_clock::now() < to) {
        if (_isAlive == 1) {
            // std::cout << "Server is alive" << std::endl;
            if (!first_connection)
                std::this_thread::sleep_until(to);
            return true;
        }
    }
    std::cout << "Server is dead" << std::endl;
    _isAlive = -1;
    return false;
}

bool UdpClient::alive() { return _isAlive != -1; }

std::vector<Player *> UdpClient::getPlayers() { return session->getPlayers(); }

UdpClient::~UdpClient() {
    send("disconnect");
    shutdown = true;
    if (listener.joinable())
        listener.join();
    if (heartbeat.joinable())
        heartbeat.join();
    close(pfd[0].fd);
}
