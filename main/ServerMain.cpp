#include "server/Server.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {
    int port = 9000;
    if (argc > 1) port = std::stoi(argv[1]);

    try {
        Server server(port);
        server.run();
    } catch (const std::exception& ex) {
        std::cerr << "[error] " << ex.what() << "\n";
        return 1;
    }
    return 0;
}