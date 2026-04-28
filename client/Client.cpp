#include "Client.hpp"
#include "../common/Protocol.hpp"
#include <iostream>
#include <thread>
#include <chrono>

Client::Client(const std::string& host, int port)
    : host_(host), port_(port),
      inputHandler_(connection_),
      receiver_(connection_) {}

void Client::run() {
    std::cout << "Enter username: ";
    std::getline(std::cin, username_);

    inputHandler_.setUsername(username_);

    std::cout << "Connecting to " << host_ << ":" << port_ << " ...\n";
    connection_.connect(host_, port_);

    if (!connection_.send(Protocol::makeLogin(username_))) {
        std::cerr << "Failed to send LOGIN.\n";
        return;
    }

    std::cout << "Connected as '" << username_ << "'. "
              << "Type a message and Enter to send, or 'quit' to exit.\n\n";

    receiver_.start();
    inputHandler_.start();

    while (receiver_.isRunning() && inputHandler_.isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    inputHandler_.stop();
    receiver_.stop();
    connection_.close();

    std::cout << "Disconnected. Goodbye!\n";
}