#pragma once
#include <QObject>
#include <string>
#include "../communication/Connection.hpp"
#include "UiMessageReceiver.hpp"

class UiClient : public QObject {
    Q_OBJECT
public:
    // Creates UI client with server info
    UiClient(const std::string& host, int port, QObject* parent = nullptr);

    // Disconnects client and cleans up resources
    ~UiClient();

    // Connects to server and sends login message
    void connectToServer(const std::string& username);

    // Sends chat message to server
    bool sendMessage(const std::string& text);

    // Disconnects from server
    void disconnect();

signals:
    void messageReceived(const QString& text);
    void disconnected();

private:
    std::string         host_;
    int                 port_;
    Connection          connection_;
    UiMessageReceiver   receiver_;
};