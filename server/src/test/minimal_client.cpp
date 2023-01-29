#include <string.h>
#include <netinet/in.h>
#include <chrono>
#include <iostream>
#include <cstring>
#include <thread>
#include <future>
#include <arpa/inet.h>
#include <poll.h>
#include <queue>
#include <vector>
#include <functional>
using namespace std::chrono_literals;

class id_generator  {
    private:
        std::vector<unsigned int> ids;
    public:
        id_generator(){}
        unsigned int get_unique_id(){
            unsigned int tmp = rand();
            while (std::find(ids.begin(), ids.end(), tmp) != ids.end())
                tmp = rand();
            return tmp;
        }
};

class UdpRequest {
    private:
        std::string data;
        std::string response;
        unsigned int id;
        int state = 0; // -1: not processed,  0: pending, 1: response received, 2: timeout
    public:
        UdpRequest(std::string data, unsigned int id) : data(data), id(id) {}
        std::string get_data() { return data; }
        unsigned int get_id() { return id; }
        int get_state() { return state; }
        bool operator==(int num) {
            if (id == num) {
                return true;
            }
            return false;
        }
        void set_response(std::string data) {
            response = data;
            state = 1;
        }
        void set_state(int new_state) {
            state = new_state;
        }
};


class UdpClient {
public:
    UdpClient(const std::string& ip, unsigned short port) : server_ip(ip), server_port(port) {
        pfd = (struct pollfd *) malloc(sizeof(struct pollfd));
        pfd->fd = socket(AF_INET, SOCK_DGRAM, 0);
        pfd->events = POLLIN;
        if (pfd[0].fd < 0) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        servaddr.sin_addr.s_addr = inet_addr(ip.data());
        len = sizeof(servaddr);
        listener = std::thread(&UdpClient::loopListener, this);
        if (connect()) {
            //heartbeat = std::thread(&UdpClient::loopHeartbeat, this);
        }
    }

    bool connect() { // to do : add option to set ip and port
        std::cout << "Connecting to server" << std::endl;
        sendHeartbeat(true);
        if (alive()) {
            std::cout << "Connected to server" << std::endl;
            shutdown = false;
            return true;
        } else {
            std::cout << "Failed to connect to server" << std::endl;
            shutdown = true;
            return false;
        }
    }

    void loopHeartbeat() {
        while (alive()) {
            sendHeartbeat();
        }
        shutdown = true;
        std::cout<<"Shutting down heartbeat"<<std::endl;
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

    std::string receive(bool showdebug = false) {
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

    void sendHeartbeat(bool first_connection = false) {
        int n;
        send("heartbeat");
        is_alive = 0;
        auto to = std::chrono::system_clock::now() + 5s;
        while (std::chrono::system_clock::now() < to) {
            if (is_alive == 1) {
                std::cout << "Server is alive" << std::endl;
                if (!first_connection)
                    std::this_thread::sleep_until(to);
                return;
            }
        }
        std::cout << "Server is dead" << std::endl;
        is_alive = -1;
    }

    bool alive() {
        return is_alive != -1 ? true : false;
    }

    ~UdpClient() {
        shutdown = true;
        if (listener.joinable())
            listener.join();
        if (heartbeat.joinable())
            heartbeat.join();
        close(pfd[0].fd);
    }

private:
    struct pollfd *pfd;
    struct sockaddr_in servaddr;
    socklen_t len;
    std::thread heartbeat;
    std::thread listener;
    std::string server_ip;
    unsigned short server_port;
    std::atomic<int> is_alive = 0; // -1: dead, 0: unknown, 1: alive
    std::atomic<bool> shutdown = false;
    std::vector<UdpRequest*> requests; //string to send to the server
    id_generator rid;
};

int main() {
    UdpClient client("127.0.0.1", 12345);
    std::string send_buf;
    //while(true)
    //    client.send("send_buf", true);
    //    client.receive(true);
    while(send_buf != "exit" && client.alive()) {
        std::cout << "Enter message: ";
        std::cin >> send_buf; // why send words ?
        client.request(send_buf);
        //client.receive(true);
    }
    std::cout << "Exiting" << std::endl;
    return 0;
}
