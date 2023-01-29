/*#include "../network/UdpClient.cpp"

UdpClient::UdpClient(asio::io_context &io_c) : socket_(io_c), resolver_(io_c) {
        io_context = &io_c;
        asio_thread = new std::thread {[this](){
            srand (time(NULL));
            std::cout << endpoint_ << std::endl; 
            while(true){
                std::string tmp;
                if (!calls.empty()) {
                    unsigned int id = rid.get_unique_id();
                    tmp = calls.front();
                    pending.insert(std::pair<unsigned int, UdpRequest*>(id, new UdpRequest(*io_context, std::ref(socket_), endpoint_, id, tmp)));
                    //tmpreq.push_back(new UdpRequest(*io_context, std::ref(socket_), endpoint_, 1, tmp));
                    calls.pop();
                }
                int cnt = 0;
                for (auto it = pending.begin(); it != pending.end();) {
                    if (it->second->get_state() == 1 || it->second->get_state() == 2) {
                        it = pending.erase(it);
                    } else
                        ++it;
                }
            }
        }};
    }

UdpClient::~UdpClient() {
    asio_thread->detach();
    while(!responses.empty())
        delete responses.front();
    for (auto &i: pending)
        pending.erase(i.first);
}

void UdpClient::connect(const std::string &host, const std::string &port) {
    unsigned int id = rid.get_unique_id();
    asio::ip::udp::resolver::results_type endpoints = resolver_.resolve(host, port);
    endpoint_ = *endpoints.begin();
    socket_.open(endpoint_.protocol());
    //pending.insert(std::pair<unsigned int, UdpRequest*>(id, new UdpRequest(*io_context, std::ref(socket_), endpoint_, id, "ping", ping_handle(1))));
    //calls.push("ping");  //  create ping obj that has udpRequest and handle (prbly gon need an interface to have different kind of calls in the same queue)
    static bool loop = false;
    if (!loop)
        std::async(std::launch::async, [&](){
            loop = true;
            std::this_thread::sleep_for(3s);
            connected = true;
            loop = false;
        });
    std::cout << "ping" << std::endl;
}

bool UdpClient::is_connected() {
    return connected;
}

void UdpClient::set_connected(bool state) {
    connected = state;
}

void UdpClient::ping_handle(int state) {
    std::cout << state << std::endl;
    if (state == 1)
        set_connected(true);
    else
        set_connected(false);
}*/