#pragma once
#include "ClientManager.hpp"
#include "MessageBroadcaster.hpp"

class Server {
public:
    // Creates server, initializes listening socket on given port
    Server(int port, int backlog = 10);

    // Cleans up resources and stops the server
    ~Server();

    // Starts accepting client connections in blocking loop
    void run();

    // Stops the server and unblocks accept()
    void stop();

private:
    // Creates, binds and starts listening socket
    int createListeningSocket(int port, int backlog);

    int listenFd_ = -1;
    bool running_ = false;
    MessageBroadcaster broadcaster_;
    ClientManager clientManager_;
};