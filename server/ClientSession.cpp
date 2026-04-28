#include "ClientSession.hpp"
#include "../common/Protocol.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

ClientSession::ClientSession(int fd, MessageBroadcaster& broadcaster)
    : fd_(fd), broadcaster_(broadcaster) {}

ClientSession::~ClientSession() {
    stop();
}

void ClientSession::start() {
    running_ = true;
    thread_ = std::thread(&ClientSession::run, this);
}

void ClientSession::stop() {
    running_ = false;
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
    if (thread_.joinable()) thread_.join();
}
    
bool ClientSession::readLine(std::string& out) {
    out.clear();
    char ch;
    while (true) {
        ssize_t n = recv(fd_, &ch, 1, 0);
        if (n <= 0) return false;
        if (ch == '\n') return true;
        out += ch;
    }
}

void ClientSession::run() {
    std::string line;

    if (!readLine(line)) {
        running_ = false;
        return;
    }

    auto loginMsg = Protocol::parse(line);
    if (loginMsg.type != MessageType::LOGIN || loginMsg.payload.empty()) {
        std::cerr << "[session fd=" << fd_ << "] Bad login.\n";
        running_ = false;
        return;
    }

    username_ = loginMsg.payload;
    std::cout << "[server] User '" << username_ << "' joined (fd=" << fd_ << ")\n";

    broadcaster_.broadcast(Protocol::makeSys("User " + username_ + " joined"));

    while (running_ && readLine(line)) {
        auto msg = Protocol::parse(line);
        if (msg.type == MessageType::MSG) {
            std::cout << "[server] " << username_ << ": " << msg.payload << "\n";
            broadcaster_.broadcast(Protocol::makeMsg(username_, msg.payload));
        }
    }

    std::cout << "[server] User '" << username_ << "' left (fd=" << fd_ << ")\n";
    broadcaster_.removeClient(fd_);
    broadcaster_.broadcast(Protocol::makeSys("User " + username_ + " left"));
    running_ = false;
}