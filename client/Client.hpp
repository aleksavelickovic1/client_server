#pragma once
#include <string>
#include "Connection.hpp"

class Client {
public:
    Client(const std::string& host, int port);
    void run();

private:
    std::string host_;
    int         port_;
    Connection  connection_;
};