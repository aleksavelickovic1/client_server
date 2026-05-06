#pragma once
#include <QObject>
#include <thread>
#include <atomic>
#include "../communication/Connection.hpp"

class UiMessageReceiver : public QObject {
    Q_OBJECT
public:
    // Creates message receiver for UI client
    explicit UiMessageReceiver(Connection& connection, QObject* parent = nullptr);

    // Stops receiver and cleans up thread
    ~UiMessageReceiver();

    // Starts background receiving thread
    void start();

    // Stops message receiving
    void stop();

    bool isRunning() const { return running_; }

signals:
    void messageReceived(const QString& text);
    void disconnected();

private:
    // Main loop for receiving and processing messages
    void run();

    Connection&       connection_;
    std::atomic<bool> running_{false};
    std::thread       thread_;
};