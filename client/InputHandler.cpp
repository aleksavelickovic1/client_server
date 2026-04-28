#include "InputHandler.hpp"
#include "../common/Protocol.hpp"
#include <iostream>

InputHandler::InputHandler(Connection& connection)
    : connection_(connection) {}

InputHandler::~InputHandler() {
    stop();
}

void InputHandler::start() {
    running_ = true;
    thread_ = std::thread(&InputHandler::run, this);
}

void InputHandler::stop() {
    running_ = false;
    if (thread_.joinable()) thread_.detach();
}

void InputHandler::run() {
    std::string line;
    while (running_ && std::getline(std::cin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line == "quit") {
            std::cout << "Quitting...\n";
            break;
        }

        if (line.empty()) continue;

        if (!connection_.send("MSG|" + line + "\n")) {
            break;
        }
    }


    running_ = false;
    connection_.close();
}