#pragma once
#include <thread>
#include <atomic>
#include "Connection.hpp"

class MessageReceiver {
public:
    explicit MessageReceiver(Connection& connection);
    ~MessageReceiver();

    void start();
    void stop();
    bool isRunning() const { return running_; }

private:
    void run();

    Connection& connection_;
    std::atomic<bool> running_{false};
    std::thread thread_;
};