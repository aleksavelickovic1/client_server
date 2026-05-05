#include "UiClient.hpp"
#include "../common/Protocol.hpp"
#include "../common/Logger.hpp"
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
    LOG_INFO("ui_client", "Connecting to " << host_ << ":" << port_
                          << " as '" << username << "'");
    connection_.connect(host_, port_);

    if (!connection_.send(Protocol::makeLogin(username)))
        throw std::runtime_error("Failed to send LOGIN");

    LOG_INFO("ui_client", "Connected as '" << username << "'");
    receiver_.start();
}

bool UiClient::sendMessage(const std::string& text) {
    bool ok = connection_.send("MSG|" + text + "\n");
    LOG_DEBUG("input", "Sending message: " << text);
    if (!ok)
        LOG_WARN("ui_client", "Failed to send message");
    return ok;
}

void UiClient::disconnect() {
    LOG_INFO("ui_client", "Disconnecting");
    connection_.close();
    receiver_.stop();
}