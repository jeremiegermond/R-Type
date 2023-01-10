#include <iostream>
#include <array>
#include <asio.hpp>

using asio::ip::udp;

int main()
{
  try
  {
    asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
    udp::socket socket(io_context);
    socket.open(udp::v4());

    std::string send_buf;
    udp::endpoint sender_endpoint;

    while (true)
    {

        std::cout << "Enter message: ";
        std::cin >> send_buf;

        socket.async_send_to(
        asio::buffer(send_buf), *endpoints.begin(),
        [](std::error_code ec, std::size_t bytes_sent) {
            std::cout << "debug" << std::endl;
            if (!ec)
                std::cout << "Sent " << bytes_sent << " bytes" << std::endl;
            else
                std::cout << "Error: " << ec.message() << std::endl;
        });
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
