#pragma once
#include <thread>
#include <atomic>
#include <string>
#include "Connection.hpp"

class InputHandler {
public:
    explicit InputHandler(Connection& connection);
    ~InputHandler();

    void setUsername(const std::string& username) { username_ = username; }

    void start();

    void stop();

    bool isRunning() const { return running_; }

private:
    void run();

    Connection&       connection_;
    std::string       username_;
    std::atomic<bool> running_{false};
    std::thread       thread_;
};