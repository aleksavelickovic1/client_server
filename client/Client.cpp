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
    std::cout << "Connecting to " << host_ << ":" << port_ << " ...\n";
    connection_.connect(host_, port_);

    while (true) {
        std::cout << "Enter username: ";
        std::getline(std::cin, username_);
        if (username_.empty()) continue;

        if (!connection_.send(Protocol::makeLogin(username_))) {
            std::cerr << "Failed to send LOGIN.\n";
            return;
        }
                
        std::string response;
        if (!connection_.readLine(response)) {
            std::cerr << "Server disconnected.\n";
            return;
        }

        auto msg = Protocol::parse(response);
        if (msg.type == MessageType::ERROR) {
            std::cout << "[!] " << msg.payload << "\n";
            continue;  
        }

        if (msg.type == MessageType::SYS)
            std::cout << "--- " << msg.payload << " ---\n";
        break;
    }

    inputHandler_.setUsername(username_);
    std::cout << "Connected as '" << username_ << "'. Type a message or /private <user> <msg>.\n\n";

    receiver_.start();
    inputHandler_.start();

    while (receiver_.isRunning() && inputHandler_.isRunning())
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    inputHandler_.stop();
    receiver_.stop();
    connection_.close();
    std::cout << "Disconnected. Goodbye!\n";
}