#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

int main() {
    try {
        // Create an io_context object to handle I/O
        asio::io_context io_context;

        // Create a TCP resolver object to resolve the server endpoint
        tcp::resolver resolver(io_context);

        // Resolve the server endpoint
        auto endpoints = resolver.resolve("127.0.0.1", "12345");

        // Create a TCP socket object and connect it to the server
        tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        // Send a message to the server
        std::string message = "Hello, server!";
        asio::write(socket, asio::buffer(message));

        // Read the response from the server
        std::array<char, 128> buffer;
        asio::error_code error;
        size_t length = socket.read_some(asio::buffer(buffer), error);

        if (error == asio::error::eof) {
            // Connection closed by peer
            std::cout << "Connection closed by peer." << std::endl;
        }
        else if (error) {
            // Some other error occurred
            throw asio::system_error(error);
        }
        else {
            // Print the response from the server
            std::cout << "Response from server: " << std::string(buffer.data(), length) << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
