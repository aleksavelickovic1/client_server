#include "client/Client.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: chat_client <host> <port>\n";
        std::cerr << "  e.g. chat_client 127.0.0.1 9000\n";
        return 1;
    }

    std::string host = argv[1];
    int         port = std::stoi(argv[2]);

    try {
        Client client(host, port);
        client.run();
    } catch (const std::exception& ex) {
        std::cerr << "[error] " << ex.what() << "\n";
        return 1;
    }
    return 0;
}