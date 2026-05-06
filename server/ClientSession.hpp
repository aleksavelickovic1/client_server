#pragma once
#include <string>
#include <thread>
#include <memory>
#include "MessageBroadcaster.hpp"

class ClientSession {
public:
    // Creates session for a connected client
    ClientSession(int fd, MessageBroadcaster& broadcaster);

    // Cleans up resources and stops the session
    ~ClientSession();

    // Starts session thread
    void start();

    // Stops session and closes connection
    void stop();

    int fd() const { return fd_; }
    const std::string& username() const { return username_; }
    bool isRunning() const { return running_; }

private:
    // Main loop: handles login and incoming messages
    void run();

    // Reads a single line (until '\n') from socket
    bool readLine(std::string& out);

    int fd_;
    std::string username_;
    bool running_ = false;
    MessageBroadcaster& broadcaster_;
    std::thread thread_;
};