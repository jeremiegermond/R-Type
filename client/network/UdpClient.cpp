#pragma once
#include "include.hpp"
#include "UdpRequest.cpp"
#include "id_generator.cpp"
#include "session.cpp"

/**
 * TODO:
 * check response params correspond to
 * whats expected for type as protocol
 * 
 * unisgned int id to uint32
 * 
 * array<float, 2> pos to Vector3 to match raylib
 */
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


class UdpClient {
    private:
        struct pollfd *pfd;
        struct sockaddr_in servaddr;
        socklen_t len;
        std::thread heartbeat;
        std::thread listener;
        std::string server_ip;
        unsigned short server_port;
        std::atomic<int> is_alive = -1; // -1: dead, 0: unknown, 1: alive
        std::atomic<bool> shutdown = false;
        std::vector<UdpRequest*> requests; //string to send to the server
        id_generator rid;
        std::atomic<bool> connected = false;
        Session *session;

    public:
        UdpClient() {
            pfd = (struct pollfd *) malloc(sizeof(struct pollfd));
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

        bool connect(const std::string& ip, unsigned short port) { // to do : add option to set ip and port
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
                if (sendHeartbeat(true)) // put in a thread to avord blocking
                    break;
            }
            if (alive()) {
                std::cout << "Connected to server" << std::endl;
                heartbeat = std::thread(&UdpClient::loopHeartbeat, this);
                //shutdown = false;
                sendConnect(); // put in a thread to avord blocking
                return true;
            } else {
                std::cout << "Failed to connect to server" << std::endl;
                shutdown = true;
                return false;
            }
        }

        void request(std::string data) {
            unsigned int id = rid.get_unique_id();
            requests.push_back(new UdpRequest(data, id));
            send(std::to_string(id) + ":" + data);
        }

        void send(std::string data, bool showdebug = false) {
            sendto(pfd[0].fd, data.data(), data.size(), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr));
            if (showdebug)
                std::cout << "Sent :" << data << std::endl;
            show_requests();
        }

        std::vector<UdpRequest*> get_requests() {
            return requests;
        }

        void pop_request(unsigned int id) {
            for (auto i = requests.begin(); i != requests.end(); i++) {
                if ((*i)->get_id() == id) {
                    requests.erase(i);
                    break;
                }
            }
        }

        void show_requests() {
            for (auto i: requests) {
                std::cout << i->get_id() << " " << i->get_data() << " " << i->get_state() << std::endl;
            }
        }

        void loopListener() {
            struct timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            while (true) {
                if (shutdown) {
                    std::cout<<"Shutting down listener"<<std::endl;
                    return;
                }
                if (poll(pfd, 1, 1000) > 0)
                    receive(true);
            }
        }

        void receive(bool showdebug = false) {
            char buffer[1024];
            int n;
            n = recvfrom(pfd[0].fd, buffer, 1024, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
            buffer[n] = '\0';
            dispatcher(buffer);
        }

        void dispatcher(std::string data) {
            std::string type = data.substr(0, data.find_first_of(':'));
            if (type == "heartbeat") {
                is_alive = 1;
            } else if (type == "connect") {
                connected = true;
                auto res = split(data, ':');
                for (auto i = res.begin() + 2; i != res.end(); i++) {
                    auto opt = split(*i, ',');
                    std::array<float, 2> pos = {std::stof(opt[1]), std::stof(opt[2])};
                    session->add_player(new Player(std::stoul(opt[0]), pos, std::stoul(opt[3])));
                }
            } else if (type == "connexion") {
                session->add_player(new Player(std::stoul(data.substr(data.find_first_of(':') + 1))));
            }  else if (type == "deconnexion") {
                session->remove_player(std::stoul(data.substr(data.find_first_of(':') + 1)));
            } else if (type.c_str()[0] == '[') {
                std::cout << data << std::endl;
                unsigned int player_id = std::stoul(data.substr(1, data.find_first_of(']') - 1));
                type = data.substr(data.find_first_of(']') + 1, data.find_first_of(':') - data.find_first_of(']') - 1);
                if (type == "pos") { //HAS to be optimized
                    std::array<float, 2> pos = {std::stof(data.substr(data.find_first_of(':') + 1, data.find_first_of(',') - data.find_first_of(':') - 1)),
                                                std::stof(data.substr(data.find_first_of(',') + 1))};
                    auto player = session->get_player(player_id);
                    if (player != nullptr)
                        player->set_pos(pos);
                    else
                        session->add_player(new Player(player_id, pos, 100)); // hp may have to be changed
                }
                //} else if (type == "disconnect") { to be added
                //    session->remove_player(player_id);
                //}
            } else {
                std::cout << data << std::endl;
                for (auto i:  requests) {
                    if (i->get_id() == std::stoi(type)) {
                        i->set_response(data);
                        show_requests();
                        break;
                    }
                }
            }
        }

        bool disconnected() {
            return !connected;
        }

        void loopHeartbeat() {
            static int count = 0;
            while (!shutdown) {
                if (connected) {
                    if (sendHeartbeat() == false)
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
            std::cout<<"Shutting down heartbeat"<<std::endl;
        }

        bool sendConnect() {
            int n;
            //send(first_connection ? "connect:" + std::to_string(session->get_player_id()) : "heartbeat");
            std::cout << "connect:" + std::to_string(session->get_player_id()) << std::endl;
            send("connect:" + std::to_string(session->get_player_id()));
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

        bool sendHeartbeat(bool first_connection = false) {
            int n;
            send("heartbeat");
            if (is_alive == 1)
                is_alive = 0;
            auto to = std::chrono::system_clock::now() + 1s; // timeout val
            while (std::chrono::system_clock::now() < to) {
                if (is_alive == 1) {
                    //std::cout << "Server is alive" << std::endl;
                    if (!first_connection)
                        std::this_thread::sleep_until(to);
                    return true;
                }
            }
            std::cout << "Server is dead" << std::endl;
            is_alive = -1;
            return false;
        }

        bool alive() {
            return is_alive != -1 ? true : false;
        }

        std::vector<Player *> get_players() {
            return session->get_players();
        }

        ~UdpClient() {
            send("disconnect");
            shutdown = true;
            if (listener.joinable())
                listener.join();
            if (heartbeat.joinable())
                heartbeat.join();
            close(pfd[0].fd);
        }
};