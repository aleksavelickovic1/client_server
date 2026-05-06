#pragma once
#include <thread>
#include <atomic>
#include <string>
#include "Connection.hpp"

class InputHandler {
public:
    // Creates input handler bound to a connection
    explicit InputHandler(Connection& connection);

    // Stops input handler and joins thread
    ~InputHandler();

    // Sets username for outgoing messages
    void setUsername(const std::string& username) { username_ = username; }

    // Starts input thread
    void start();

    // Stops input handling
    void stop();

    bool isRunning() const { return running_; }

private:
    // Main loop for reading user input and sending messages
    void run();

    Connection&       connection_;
    std::string       username_;
    std::atomic<bool> running_{false};
    std::thread       thread_;
};