#include "Client.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        Client client("127.0.0.1", 9000);
        client.run();
    } catch (const std::exception& e) {
        std::cerr << "[client] Fatal: " << e.what() << "\n";
        return 1;
    }
    return 0;
}