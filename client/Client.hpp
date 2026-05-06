#pragma once
#include <string>
#include "Connection.hpp"
#include "InputHandler.hpp"
#include "MessageReceiver.hpp"

class Client {
public:
    // Creates client with server host and port
    Client(const std::string& host, int port);

    // Runs client: connects, starts input and receiving
    void run();

private:
    std::string host_;
    int         port_;
    std::string username_;

    Connection      connection_;
    InputHandler    inputHandler_;
    MessageReceiver receiver_;
};