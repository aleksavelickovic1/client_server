#include "UiClient.hpp"
#include "../common/Protocol.hpp"
#include <stdexcept>

UiClient::UiClient(const std::string& host, int port, QObject* parent)
    : QObject(parent),
      host_(host), port_(port),
      receiver_(connection_, this)
{
    connect(&receiver_, &UiMessageReceiver::messageReceived,
            this,       &UiClient::messageReceived);
    connect(&receiver_, &UiMessageReceiver::disconnected,
            this,       &UiClient::disconnected);
}

UiClient::~UiClient() {
    disconnect();
}

void UiClient::connectToServer(const std::string& username) {
    connection_.connect(host_, port_);

    if (!connection_.send(Protocol::makeLogin(username)))
        throw std::runtime_error("Failed to send LOGIN");

    receiver_.start();
}

bool UiClient::sendMessage(const std::string& text) {
    return connection_.send("MSG|" + text + "\n");
}

void UiClient::disconnect() {
    connection_.close();
    receiver_.stop();
}