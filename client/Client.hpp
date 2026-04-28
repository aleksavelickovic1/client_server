#pragma once
#include <string>
#include "Connection.hpp"
#include "InputHandler.hpp"
#include "MessageReceiver.hpp"

class Client {
public:
    Client(const std::string& host, int port);

    void run();

private:
    std::string host_;
    int         port_;
    std::string username_;

    Connection      connection_;
    InputHandler    inputHandler_;
    MessageReceiver receiver_;
};