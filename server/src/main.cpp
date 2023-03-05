#include "network.hpp"
#include "serverEngine.hpp"
#include "utils.hpp"
#include <queue>

std::unordered_map<int, Color> _log_colors = {
    {LOG_INFO, WHITE},
    {LOG_ERROR, RED},
    {LOG_WARNING, YELLOW},
    {LOG_DEBUG, BLUE},
};

int main() {
    server_engine server;
    try {
        server.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
