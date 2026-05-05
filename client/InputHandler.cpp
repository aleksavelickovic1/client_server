#include "InputHandler.hpp"
#include "../common/Protocol.hpp"
#include <iostream>
#include "../common/Logger.hpp"
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

    LOG_INFO("input", "Input handler started");

    while (running_ && std::getline(std::cin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line == "quit") {
            LOG_INFO("input", "User requested quit");
            std::cout << "Quitting...\n";
            break;
        }

        if (line.empty()) continue;

        LOG_DEBUG("input", "Sending message: " << line);

        if (!connection_.send("MSG|" + line + "\n")) {
            LOG_ERROR("input", "Failed to send message");
            break;
        }
    }

    LOG_INFO("input", "Input handler stopping");

    running_ = false;
    connection_.close();
}