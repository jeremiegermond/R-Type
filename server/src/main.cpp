#include <array>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <functional>
#include <cstring>
#include <map>

#include "../../utils/id_generator.cpp"
#include "../../utils/split.cpp"

class Player {
    private:
        unsigned int id;
        int hp;
    public:
        std::array<float, 2> server_pos;
        Player(uint id) {
            server_pos.fill(0);
            this->id = id;
        }
        Player(int id, std::array<float, 2> pos, int hp) {
            this->id = id;
            this->server_pos = pos;
            this->hp = hp;
        }
        std::array<float, 2> get_server_pos() {
            return server_pos;
        }
        int get_hp() {
            return hp;
        }
        unsigned int get_id() {
            return id;
        }
        void set_pos(std::array<float, 2> pos) {
            this->server_pos = pos;
        }
        void set_hp(int hp) {
            this->hp = hp;
        }
        bool move(std::string direction, std::array<float, 2> &client_pos) {
            float speed = 0.1;
            if (direction == "right")
                server_pos[0] += speed;
            if (direction == "left")
                server_pos[0] -= speed;
            if (direction == "up")
                server_pos[1] += speed;
            if (direction == "down")
                server_pos[1] -= speed;
            std::cout << "server_pos: " << server_pos[0] << " " << server_pos[1] << std::endl;
            std::cout << "client_pos: " << client_pos[0] << " " << client_pos[1] << std::endl;
            if (server_pos[0] != client_pos[0] || server_pos[1] != client_pos[1])
                return false;
            return true;
        }
        bool process_movement(std::string data, std::array<float, 2> &server_pos) {
            if (data == "right" || data == "left" || data == "up" || data == "down") {
                if (!move(data, server_pos))
                    return false;
                return true;
            }
            return false;
        }
};

/*class Session {
    private:
        unsigned int id;
        std::vector<Player *> players;
    public:
        Session() {
        }
        Session(unsigned int id) {
            this->id = id;
        }
        Player *get_player(unsigned int id) {
            for (int i = 0; i < players.size(); i++) {
                if (players[i]->get_id() == id)
                    return players[i];
            }
            return nullptr;
        }
        std::vector<Player *> get_players() {
            return players;
        }
        void add_player(Player *player) {
            players.push_back(player);
        }
        void remove_player(int id) {
            for (int i = 0; i < players.size(); i++) {
                if (players[i]->get_id() == id) {
                    players.erase(players.begin() + i);
                    break;
                }
            }
        }
        void update_player_pos(int id, std::array<float, 2> pos) {
            for (int i = 0; i < players.size(); i++) {
                if (players[i]->get_id() == id) {
                    players[i]->set_pos(pos);
                    break;
                }
            }
        }
};*/

class cli_addr {
    private:
        struct sockaddr_in addr;
        socklen_t len;
    public:
        cli_addr() {
            memset(&addr, 0, sizeof(addr));
            len = sizeof(addr);
        }
        struct sockaddr_in *get_addr() {
            return &addr;
        }
        socklen_t *get_len() {
            return &len;
        }
        std::string get_addr_str() {
            return std::string(inet_ntoa(addr.sin_addr)) + ":" + std::to_string(ntohs(addr.sin_port));
        }
        bool operator<(const cli_addr &other) const {
            //return true;
            std::string a = std::string(inet_ntoa(addr.sin_addr)) + ":" + std::to_string(ntohs(addr.sin_port));
            std::string b = std::string(inet_ntoa(other.addr.sin_addr)) + ":" + std::to_string(ntohs(other.addr.sin_port));
            return a < b;
        }
        bool operator==(const cli_addr &other) const {
            //return true;
            std::string a = std::string(inet_ntoa(addr.sin_addr)) + ":" + std::to_string(ntohs(addr.sin_port));
            std::string b = std::string(inet_ntoa(other.addr.sin_addr)) + ":" + std::to_string(ntohs(other.addr.sin_port));
            return a == b;
        }
};

class UdpServer {
    private:
        struct pollfd *pfd;
        socklen_t len;
        struct sockaddr_in servaddr;
        std::map<cli_addr *, Player *> players;
        id_generator rid;
    public:
        UdpServer() {
            pfd = (struct pollfd *) malloc(sizeof(struct pollfd));
            pfd->fd = socket(AF_INET, SOCK_DGRAM, 0);
            pfd->events = POLLIN;
            if (pfd[0].fd < 0) {
                std::cout << "socket creation failed" << std::endl;
                exit(EXIT_FAILURE);
            }

            memset(&servaddr, 0, sizeof(servaddr));    // Filling server information
            servaddr.sin_family = AF_INET; // IPv4
            servaddr.sin_addr.s_addr = INADDR_ANY;
            servaddr.sin_port = htons(12345);
            // Bind the socket with the server address
            if ( bind(pfd->fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
            {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }
        }

        void send(std::string data, cli_addr &cliaddr, bool showdebug = false) {
            sendto(pfd[0].fd, data.data(), data.size(), MSG_CONFIRM, (struct sockaddr *) cliaddr.get_addr(), *cliaddr.get_len());
            if (showdebug)
                std::cout << "Sent :" << data << " to : " << cliaddr.get_addr_str() << std::endl;
            //show_requests();
        }

        void receive(bool showdebug = false) {
            char buffer[1024];
            int n;
            while(true) {
                if (poll(pfd, 1, 1000) > 0) {
                    cli_addr cliaddr;
                    n = recvfrom(pfd[0].fd, buffer, 1024, MSG_WAITALL, (struct sockaddr *) cliaddr.get_addr(), cliaddr.get_len());
                    buffer[n] = '\0';
                    if (showdebug)
                        std::cout << "Received :" << buffer << std::endl\
                        << "from :" << cliaddr.get_addr_str() << std::endl;
                    handle_request(buffer, cliaddr);
                    memset(&cliaddr, 0, sizeof(cliaddr));
                    memset(buffer, 0, 1024);
                }
            }
        }

        Player *get_player(cli_addr &cliaddr) {            
            Player *player = nullptr;
            std::cout << "players : " << std::endl;
            for (auto it = players.begin(); it != players.end(); it++) {
                std::cout << it->first->get_addr_str() << ' ' << std::endl;
                if (it->first->get_addr_str() == cliaddr.get_addr_str()) {
                    player = it->second;
                    break;
                }
            }
            return player;
        }
        /**
         * 
         * when joining the game, additional player created idk why
        */

        void handle_request(std::string request, cli_addr &cliaddr) {
            Player *player = get_player(cliaddr);
            if (player == nullptr) {
                std::cout << "unknown player" << std::endl;
            }
                std::cout << "Received : " << request << std::endl;
            std::vector<std::string> req = split(request, ':');
            if (req[0] == "heartbeat") // do the handlers as in minimal_udp_server + todo: add threads
                send("heartbeat", cliaddr);
            else if (req[0] == "connect") {
                std::cout << "New connexion : " << cliaddr.get_addr_str() << std::endl;
                player = new Player(std::stoul(req[1]));
                players.insert(std::pair<cli_addr *, Player *>(new cli_addr(cliaddr), player));
                req[0] += ":OK";
                for (auto it = players.begin(); it != players.end(); it++) {
                    std::cout << "Player : " << it->second->get_id() << std::endl;
                    if (it->first->get_addr_str() != cliaddr.get_addr_str())
                        req[0] += ':' + std::to_string(it->second->get_id()) + ',' + std::to_string(it->second->get_server_pos()[0]) + ',' + std::to_string(it->second->get_server_pos()[1]) + ",100"; //hp to be changed
                }
                std::cout << "Client connected" << std::endl;
                for (auto it = players.begin(); it != players.end(); it++) // braodcast to all clients
                    if (it->first->get_addr_str() != cliaddr.get_addr_str())
                        send("connexion:" + req[1], *it->first);
            }
            else if (player && req[0] == "disconnect") {
                std::cout << "Client disconnected" << std::endl;
                for (auto it = players.begin(); it != players.end(); it++) // braodcast to all clients
                    if (it->first->get_addr_str() != cliaddr.get_addr_str())
                        send("deconnexion:" + std::to_string(player->get_id()), *it->first, true);
                for (auto it = players.begin(); it != players.end(); it++) // braodcast to all clients
                    if (it->first->get_addr_str() == cliaddr.get_addr_str()) {
                        players.erase(it);
                        break;
                    }
            }
            else if (player && req[1] == "move") {
                auto args = split(req[2], ',');
                std::array<float, 2> pos = { std::stof(args[1]), std::stof(args[1]) };
                if (player->process_movement(args[0], pos))
                    req[0] += ":OK";
                else
                    req[0] += ":KO:" + std::to_string(player->get_server_pos()[0]) + ":" + std::to_string(player->get_server_pos()[1]);
                std::cout << "Sending : " << req[0] << std::endl;
                auto serv_pos = player->get_server_pos();
                for (auto it = players.begin(); it != players.end(); it++) {// braodcast to all clients 
                    if (it->first->get_addr_str() != cliaddr.get_addr_str())
                        send('[' + std::to_string(player->get_id()) + "]pos:" + std::to_string(serv_pos[0]) + "," + std::to_string(serv_pos[1]), *it->first, true);
            }
            }
            send(req[0], cliaddr);
        }
};

int main() {
    UdpServer server;
    server.receive();
    return 0;
}