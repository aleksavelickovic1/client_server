#pragma once
#include <QObject>
#include <string>
#include "../communication/Connection.hpp"
#include "UiMessageReceiver.hpp"

class UiClient : public QObject {
    Q_OBJECT
public:
    UiClient(const std::string& host, int port, QObject* parent = nullptr);
    ~UiClient();

    void connectToServer(const std::string& username);

    bool sendMessage(const std::string& text);

    void disconnect();

signals:
    void messageReceived(const QString& text);
    void disconnected();

private:
    std::string     host_;
    int             port_;
    Connection      connection_;
    UiMessageReceiver receiver_;
};