#pragma once
#include "ClientManager.hpp"
#include "MessageBroadcaster.hpp"


class Server {
public:
    Server(int port, int backlog = 10);
    ~Server();

    void run();
    void stop();

private:
    int createListeningSocket(int port, int backlog);

    int listenFd_ = -1;
    bool running_ = false;
    MessageBroadcaster broadcaster_;
    ClientManager clientManager_;
};