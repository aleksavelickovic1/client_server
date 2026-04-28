#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <cstring>

Server::Server(int port, int backlog)
    : clientManager_(broadcaster_) {
    listenFd_ = createListeningSocket(port, backlog);
    std::cout << "[server] Listening on port " << port << "\n";
}

Server::~Server() {
    stop();
    if (listenFd_ >= 0) ::close(listenFd_);
}

int Server::createListeningSocket(int port, int backlog) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) throw std::runtime_error("socket() failed");

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(static_cast<uint16_t>(port));

    if (bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        throw std::runtime_error("bind() failed: " + std::string(strerror(errno)));

    if (listen(fd, backlog) < 0)
        throw std::runtime_error("listen() failed");

    return fd;
}

void Server::run() {
    running_ = true;
    while (running_) {
        sockaddr_in clientAddr{};
        socklen_t   addrLen = sizeof(clientAddr);

        int clientFd = accept(listenFd_,
                              reinterpret_cast<sockaddr*>(&clientAddr),
                              &addrLen);
        if (clientFd < 0) {
            if (!running_) break;        
            std::cerr << "[server] accept() error: " << strerror(errno) << "\n";
            continue;
        }

        std::cout << "[server] New connection fd=" << clientFd << "\n";
        clientManager_.addClient(clientFd);
        clientManager_.clean();
    }
    clientManager_.stopAll();
}

void Server::stop() {
    running_ = false;
    if (listenFd_ >= 0) ::shutdown(listenFd_, SHUT_RDWR);
}