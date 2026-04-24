#pragma once


class Server {
public:
    Server(int port, int backlog = 10);
    ~Server();

    void run();  
    void stop();

private:
    int createListeningSocket(int port, int backlog);
    void handleClient(int clientFd);

    int  listenFd_ = -1;
    bool running_  = false;
};