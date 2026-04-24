#pragma once
#include <string>

class Connection {
public:
    Connection() = default;
    ~Connection();

    void connect(const std::string& host, int port); 
    bool send(const std::string& data);              
    bool readLine(std::string& out);                 
    void close();

    bool isConnected() const { return fd_ >= 0; }
    int  fd()          const { return fd_; }

private:
    int fd_ = -1;
};