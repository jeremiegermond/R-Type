/*#include "../network/UdpRequest.cpp"

UdpRequest::UdpRequest(asio::io_context &ioc, asio::ip::udp::socket &soc, asio::ip::udp::endpoint endpoint, int id, std::string data, void(*handle)(int), std::chrono::milliseconds to) {
    io_context = &ioc;
    endp = &endpoint;
    socket_ = &soc;
    this->id = id;
    this->data = data;
    timeout_limit = to;
    thrd = new std::thread(&UdpRequest::run, this);
}

void UdpRequest::run() {
    std::array<char, 128> recv_buf_;
    send(data);
    io_context->run();
    receive(recv_buf_);
    io_context->run();
    /*if (recv_buf_.data()[0] == '\0') {
        state = 2;
        std::cout << "timeout" << std::endl;
    }*/
    /*state = 1;
}

int UdpRequest::get_state() {
    return state;
}

void UdpRequest::send(const std::string &message, void (*handler)()) {
    //unsigned int id = rid.get_unique_id();
    //pending[id] = new UdpRequest(id, message);
    socket_->async_send_to(asio::buffer(std::to_string(id) + ":" + message), *endp, [&](std::error_code ec, std::size_t bytes_received) {
        if (!ec) {
            //pending[id] = new UdpRequest(id, message);
            std::cout << "Sent " << std::to_string(id) + ":" + message << std::endl;
        } else
            std::cout << "Error: " << ec.message() << std::endl;
    });
}

void UdpRequest::receive(std::array<char, 128> &recv_buf_) {
    socket_->async_receive_from(asio::buffer(recv_buf_), sender_endpoint_, [&](std::error_code ec, std::size_t bytes_received) {
        std::cout << recv_buf_.data() << std::endl;
    });
}*/