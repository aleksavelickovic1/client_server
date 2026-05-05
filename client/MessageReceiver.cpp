#include "MessageReceiver.hpp"
#include "../common/Protocol.hpp"
#include "../common/Logger.hpp"
#include <iostream>

MessageReceiver::MessageReceiver(Connection& connection)
    : connection_(connection) {}

MessageReceiver::~MessageReceiver() {
    stop();
}

void MessageReceiver::start() {
    running_ = true;
    thread_ = std::thread(&MessageReceiver::run, this);
}

void MessageReceiver::stop() {
    running_ = false;
    if (thread_.joinable()) thread_.join();
}

void MessageReceiver::run() {
    std::string line;
    while (running_ && connection_.readLine(line)) {
        auto msg = Protocol::parse(line);
        switch (msg.type) {
            case MessageType::MSG:
                std::cout << msg.payload << "\n";
                break;
            case MessageType::SYS:
                std::cout << "--- " << msg.payload << " ---\n";
                break;
            default:
                LOG_WARN("receiver", "Unknown message type, ignoring");
                break;
        }
    }
    if (running_) {
        LOG_INFO("receiver", "Disconnected from server");
        std::cout << "\n[disconnected from server]\n";
        running_ = false;
    }
}