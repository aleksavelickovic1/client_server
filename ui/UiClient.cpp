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
    connection_.connect(host_, port_);
    if (!connection_.send(Protocol::makeLogin(username)))
        throw std::runtime_error("Failed to send LOGIN");

    std::string response;
    if (!connection_.readLine(response))
        throw std::runtime_error("Server disconnected");

    auto msg = Protocol::parse(response);
    if (msg.type == MessageType::ERROR)
        throw std::runtime_error(msg.payload);

    if (msg.type == MessageType::SYS)
        emit messageReceived(QString("*** %1 ***").arg(QString::fromStdString(msg.payload)));

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