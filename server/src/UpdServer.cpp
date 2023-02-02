/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** UpdServer.cpp by thibb1
*/

#include "network.hpp"

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void UdpServer::do_receive() {
    recv_buf_.fill(0);
    socket_.async_receive_from(asio::buffer(recv_buf_), sender_endpoint_, [this](std::error_code ec, std::size_t bytes_recvd) {
        // std::cout << bytes_recvd << " " << std::endl;
        if (!ec && bytes_recvd > 0)
            do_send(bytes_recvd);
        else
            do_receive();
    });
}

void UdpServer::do_send(std::size_t length) {
    std::string recv = recv_buf_.data();
    // std::cout << "Received message: " << recv << std::endl;
    std::vector<std::string> resp = split(recv, ':');
    auto client_it = clients.find(sender_endpoint_);
    Client *client = client_it->second;
    // std::cout << "Received message: " << resp[0] << (resp.size() >= 2 ? ":" + resp[1] : "") << std::endl;
    if (resp.size() > 1 && (resp[1] == "right" || resp[1] == "left" || resp[1] == "up" || resp[1] == "down")) {
        // resp[1] += ":" + client->get_server_pos();
        std::array<float, 2> pos = {std::stof(resp[2]), std::stof(resp[3])};
        if (client->process_movement(resp[1], pos))
            resp[0] += ":OK";
        else
            resp[0] += ":KO:" + std::to_string(client->get_server_pos()[0]) + ":" + std::to_string(client->get_server_pos()[1]);
        auto serv_pos = client->get_server_pos();
        for (auto it : clients) { // make broadcast function to send to all other clients
            if (it.first != sender_endpoint_) {
                // std::cout << "sent: " << resp[0]  << " to :" << it.first << std::endl;
                socket_.async_send_to(
                    asio::buffer('[' + std::to_string(client->get_id()) + "]pos:" + std::to_string(serv_pos[0]) + "," + std::to_string(serv_pos[1])),
                    it.first, [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) { do_receive(); });
            }
        }
    } else if (resp[0] == "connect") {
        std::cout << "New connexion :" << sender_endpoint_ << std::endl;
        clients.insert(
            std::pair<asio::ip::udp::endpoint, Client *>(sender_endpoint_, new Client(std::stoul(resp[1])))); // doesnt check if id already exists
        resp[0] += ":OK";
        for (auto it : clients) {
            if (it.first != sender_endpoint_) {
                resp[0] += ':' + std::to_string(it.second->get_id()) + ',' + std::to_string(it.second->get_server_pos()[0]) + ',' +
                           std::to_string(it.second->get_server_pos()[1]) + ",100"; // hp to be changed
            }
        }
        std::cout << resp[0] << std::endl;
        std::cout << "Client connected" << std::endl;
        for (auto it : clients) { // make broadcast function to send to all other clients
            if (it.first != sender_endpoint_) {
                // std::cout << "sent: " << resp[0]  << " to :" << it.first << std::endl;
                socket_.async_send_to(asio::buffer("connexion:" + resp[1]), it.first,
                                      [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) { do_receive(); });
            }
        }
    } else if (resp[0] == "disconnect") {
        std::cout << "Client disconnected" << std::endl;
        for (auto it : clients) { // make broadcast function to send to all other clients
            if (it.first != sender_endpoint_) {
                // std::cout << "sent: " << resp[0]  << " to :" << it.first << std::endl;
                socket_.async_send_to(asio::buffer("deconnexion:" + std::to_string(client->get_id())), it.first,
                                      [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) { do_receive(); });
            }
        }
        clients.erase(sender_endpoint_);
    }
    // auto start = std::chrono::high_resolution_clock::now();
    // std::this_thread::sleep_for(3s);
    // auto end = std::chrono::high_resolution_clock::now();
    // std::cout << "sent: " << resp[0]  << " to :" << sender_endpoint_ << std::endl;
    socket_.async_send_to(asio::buffer(resp[0]), sender_endpoint_, [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) { do_receive(); });
}
