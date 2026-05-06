#pragma once
#include <thread>
#include <atomic>
#include "Connection.hpp"

class MessageReceiver {
public:
    // Creates receiver bound to a connection
    explicit MessageReceiver(Connection& connection);

    // Stops receiver and joins thread
    ~MessageReceiver();

    // Starts background thread for receiving messages
    void start();

    // Stops receiving messages
    void stop();

    bool isRunning() const { return running_; }

private:
    // Main loop for reading and handling messages
    void run();

    Connection& connection_;
    std::atomic<bool> running_{false};
    std::thread thread_;
};