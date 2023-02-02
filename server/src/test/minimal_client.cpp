#include <array>
#include <asio.hpp>
#include <iostream>

class UdpClient {
    private:
        asio::ip::udp::resolver resolver_;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint endpoint_;
        asio::ip::udp::endpoint sender_endpoint_;
        std::array<char, 128> recv_buf_;

    public:
        UdpClient(asio::io_context &io_context, const std::string &host, const std::string &port) : socket_(io_context), resolver_(io_context) {
            asio::ip::udp::resolver::results_type endpoints = resolver_.resolve(host, port);
            endpoint_ = *endpoints.begin();
            socket_.open(endpoint_.protocol());
        }

        void send(const std::string &message) {
            socket_.async_send_to(asio::buffer(message), endpoint_, [](std::error_code ec, std::size_t bytes_sent) {
                if (!ec)
                    std::cout << "Sent " << bytes_sent << " bytes" << std::endl;
                else
                    std::cout << "Error: " << ec.message() << std::endl;
            });
        }

        void receive() {
            socket_.async_receive_from(asio::buffer(recv_buf_), sender_endpoint_, [this](std::error_code ec, std::size_t bytes_received) {
                if (!ec)
                    std::cout << "Received " << bytes_received << " bytes :" << std::string(recv_buf_.data(), bytes_received) << std::endl;
                else
                    std::cout << "Error: " << ec.message() << std::endl;
            });
        }
};

int main() {
    try {
        asio::io_context io_context;
        UdpClient client(io_context, "127.0.0.1", "12345");

        while (true) {
            std::cout << "Enter message: ";
            std::string send_buf;
            std::cin >> send_buf;

            client.send(send_buf);
            client.receive();

            io_context.run();
            io_context.reset();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
