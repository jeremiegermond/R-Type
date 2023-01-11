#include <array>
#include <asio.hpp>
#include <iostream>

using asio::ip::udp;

class UdpServer {
  private:
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    std::array<char, 1024> recv_buf_;

    void do_receive() {
        recv_buf_.fill(0);
        socket_.async_receive_from(asio::buffer(recv_buf_), sender_endpoint_, [this](std::error_code ec, std::size_t bytes_recvd) {
            std::cout << bytes_recvd << " " << std::endl;
            if (!ec && bytes_recvd > 0)
                do_send(bytes_recvd);
            else
                do_receive();
        });
    }

    void do_send(std::size_t length) {
        std::cout << "Received message: " << recv_buf_.data() << std::endl;

        socket_.async_send_to(asio::buffer("OK"), sender_endpoint_, [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) { do_receive(); });
    }

  public:
    UdpServer(asio::io_context &io_context, short port) : socket_(io_context, udp::endpoint(udp::v4(), port)) { do_receive(); }
};

int main() {
    try {
        asio::io_context io_context;
        UdpServer server(io_context, 12345);
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
