#pragma once
#include <string>
#include <thread>
#include <memory>
#include "MessageBroadcaster.hpp"

class ClientSession {
public:
    ClientSession(int fd, MessageBroadcaster& broadcaster);
    ~ClientSession();

    void start();
    void stop();

    int fd() const { return fd_; }
    const std::string& username() const { return username_; }
    bool isRunning() const { return running_; }

private:
    void run();
    bool readLine(std::string& out);

    int fd_;
    std::string username_;
    bool running_ = false;
    MessageBroadcaster& broadcaster_;
    std::thread thread_;
};