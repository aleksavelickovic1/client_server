#pragma once
#include <string>

class Connection {
public:
    Connection() = default;

    // Cleans up resources and stops the connection
    ~Connection();

    // Connects to server at given host and port
    void connect(const std::string& host, int port); 

    // Sends message to the server
    bool send(const std::string& data);              

    // Reads a single line until '\n' from server
    bool readLine(std::string& out);                 

    // Closes the connection
    void close();

    bool isConnected() const { return fd_ >= 0; }
    int  fd()          const { return fd_; }

private:
    int fd_ = -1;
};