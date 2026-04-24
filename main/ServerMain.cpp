#include "Server.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        Server server(9000);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "[server] Fatal: " << e.what() << "\n";
        return 1;
    }
    return 0;
}