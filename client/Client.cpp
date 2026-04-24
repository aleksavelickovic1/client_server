#include "Client.hpp"
#include <iostream>

Client::Client(const std::string& host, int port)
    : host_(host), port_(port) {}

void Client::run() {
    std::cout << "[client] Connecting to " << host_ << ":" << port_ << " ...\n";
    connection_.connect(host_, port_);
    std::cout << "[client] Connected.\n";

   
    const std::string message = "Hello!\n";
    if (connection_.send(message))
        std::cout << "[client] Sent: " << message;
    else
        std::cerr << "[client] Send failed.\n";

    connection_.close();
    std::cout << "[client] Done.\n";
}