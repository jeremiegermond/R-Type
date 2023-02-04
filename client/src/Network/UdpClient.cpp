#include "network.hpp"

UdpClient::UdpClient() {
    pfd = (struct pollfd *)malloc(sizeof(struct pollfd));
    pfd->fd = socket(AF_INET, SOCK_DGRAM, 0);
    pfd->events = POLLIN;
    if (pfd[0].fd < 0) {
        std::cout << "socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    len = sizeof(servaddr);
    session = new Session(rid.get_unique_id());
}

bool UdpClient::connect(const std::string &ip, unsigned short port) { // to do : add option to set ip and port
    is_alive = 0;
    connected = false;
    server_ip = ip;
    server_port = port;
    servaddr.sin_port = htons(server_port);
    servaddr.sin_addr.s_addr = inet_addr(server_ip.data());
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

void UdpClient::request(std::string data) {
    unsigned int id = rid.get_unique_id();
    requests.push_back(new UdpRequest(data, id));
    send(std::to_string(id) + ":" + data);
}

void UdpClient::send(std::string data, bool showDebug) {
    sendto(pfd[0].fd, data.data(), data.size(), MSG_CONFIRM, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (showDebug)
        std::cout << "Sent :" << data << std::endl;
    // show_requests();
}

std::vector<UdpRequest *> UdpClient::get_requests() { return requests; }

void UdpClient::pop_request(unsigned int id) {
    for (auto i = requests.begin(); i != requests.end(); i++) {
        if ((*i)->get_id() == id) {
            requests.erase(i);
            break;
        }
    }
}

void UdpClient::show_requests() {
    for (auto i : requests) {
        std::cout << i->get_id() << " " << i->get_data() << " " << i->get_state() << std::endl;
    }
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
    char buffer[1024];
    int n;
    n = recvfrom(pfd[0].fd, buffer, 1024, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    dispatcher(buffer);
}

void UdpClient::dispatcher(const std::string &data) {
    std::string type = data.substr(0, data.find_first_of(':'));
    if (type == "heartbeat") {
        is_alive = 1;
    } else if (type == "connect") {
        connected = true;
        auto res = split(data, ':');
        for (auto i = res.begin() + 2; i != res.end(); i++) {
            auto opt = split(*i, ',');
            std::array<float, 2> pos = {std::stof(opt[1]), std::stof(opt[2])};
            session->addPlayer(new Player(std::stoi(opt[0]), pos, std::stoi(opt[3])));
        }
    } else if (type == "connexion") {
        session->addPlayer(new Player(std::stoi(data.substr(data.find_first_of(':') + 1))));
    } else if (type == "deconnexion") {
        session->removePlayer(std::stoi(data.substr(data.find_first_of(':') + 1)));
    } else if (type.c_str()[0] == '[') {
        std::cout << data << std::endl;
        int player_id = std::stoi(data.substr(1, data.find_first_of(']') - 1));
        type = data.substr(data.find_first_of(']') + 1, data.find_first_of(':') - data.find_first_of(']') - 1);
        if (type == "pos") { // HAS to be optimized
            std::array<float, 2> pos = {std::stof(data.substr(data.find_first_of(':') + 1, data.find_first_of(',') - data.find_first_of(':') - 1)),
                                        std::stof(data.substr(data.find_first_of(',') + 1))};
            std::cout << "Player " << player_id << " moved to " << pos[0] << " " << pos[1] << std::endl;
            auto player = session->getPlayer(player_id);
            if (player != nullptr)
                player->setPos(pos);
            else
                session->addPlayer(new Player(player_id, pos, 100)); // _hp may have to be changed
        }
        //} else if (type == "disconnect") { to be added
        //    session->removePlayer(player_id);
        //}
    } else {
        std::cout << data << std::endl;
        for (auto i : requests) {
            if (i->get_id() == std::stoi(type)) {
                i->set_response(data);
                // show_requests();
                break;
            }
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
    int n;
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
    is_alive = -1;
    return false;
}

bool UdpClient::sendHeartbeat(bool first_connection) {
    int n;
    send("heartbeat");
    if (is_alive == 1)
        is_alive = 0;
    auto to = std::chrono::system_clock::now() + 1s; // timeout val
    while (std::chrono::system_clock::now() < to) {
        if (is_alive == 1) {
            // std::cout << "Server is alive" << std::endl;
            if (!first_connection)
                std::this_thread::sleep_until(to);
            return true;
        }
    }
    std::cout << "Server is dead" << std::endl;
    is_alive = -1;
    return false;
}

bool UdpClient::alive() { return is_alive != -1; }

std::vector<Player *> UdpClient::get_players() { return session->getPlayers(); }

UdpClient::~UdpClient() {
    send("disconnect");
    shutdown = true;
    if (listener.joinable())
        listener.join();
    if (heartbeat.joinable())
        heartbeat.join();
    close(pfd[0].fd);
}
