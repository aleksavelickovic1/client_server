#include "UiMessageReceiver.hpp"
#include "../common/Protocol.hpp"
#include "../common/Logger.hpp"

UiMessageReceiver::UiMessageReceiver(Connection& connection, QObject* parent)
    : QObject(parent), connection_(connection) {}

UiMessageReceiver::~UiMessageReceiver() {
    stop();
}

void UiMessageReceiver::start() {
    running_ = true;
    thread_ = std::thread(&UiMessageReceiver::run, this);
}

void UiMessageReceiver::stop() {
    running_ = false;
    if (thread_.joinable()) thread_.join();
}

void UiMessageReceiver::run() {
    std::string line;
    while (running_ && connection_.readLine(line)) {
        auto msg = Protocol::parse(line);
        QString text;
        switch (msg.type) {
            case MessageType::MSG:
                text = QString::fromStdString(msg.payload);
                break;
            case MessageType::SYS:
                text = "*** " + QString::fromStdString(msg.payload) + " ***";
                break;
            case MessageType::PRIVATE:
                text = QString::fromStdString(msg.payload);
                break;
            case MessageType::ERROR:
                text = "[!] " + QString::fromStdString(msg.payload);
                break;
            default:
                LOG_WARN("receiver", "Unknown message type, ignoring");
                continue;
        }
        emit messageReceived(text);
    }
    if (running_) {
        LOG_INFO("receiver", "Disconnected from server");
        emit disconnected();
        running_ = false;
    }
}