#pragma once
#include <QObject>
#include <thread>
#include <atomic>
#include "../communication/Connection.hpp"

class UiMessageReceiver : public QObject {
    Q_OBJECT
public:
    explicit UiMessageReceiver(Connection& connection, QObject* parent = nullptr);
    ~UiMessageReceiver();

    void start();
    void stop();
    bool isRunning() const { return running_; }

signals:
    void messageReceived(const QString& text);
    void disconnected();

private:
    void run();

    Connection&       connection_;
    std::atomic<bool> running_{false};
    std::thread       thread_;
};