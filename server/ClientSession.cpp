#include "ClientSession.hpp"
#include "../common/Protocol.hpp"
#include "../common/Logger.hpp"
#include <sys/socket.h>
#include <unistd.h>

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

    const int maxAttempts = 3;
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        if (!readLine(line)) { running_ = false; return; }

        auto loginMsg = Protocol::parse(line);
        if (loginMsg.type != MessageType::LOGIN || loginMsg.payload.empty()) {
            LOG_WARN("session", "Bad login on fd=" << fd_);
            if (send(fd_, Protocol::makeError("Invalid login format").c_str(),
                     Protocol::makeError("Invalid login format").size(), MSG_NOSIGNAL) < 0) {}
            running_ = false;
            return;
        }

        std::string requested = loginMsg.payload;
        if (broadcaster_.isUsernameTaken(requested)) {
            std::string err = Protocol::makeError("Username '" + requested + "' is already taken. Try another.");
            send(fd_, err.c_str(), err.size(), MSG_NOSIGNAL);
            LOG_WARN("session", "Duplicate username '" << requested << "' on fd=" << fd_);
            continue;
        }

        username_ = requested;
        break;
    }

    if (username_.empty()) {
        std::string err = Protocol::makeError("Too many failed login attempts. Disconnecting.");
        send(fd_, err.c_str(), err.size(), MSG_NOSIGNAL);
        running_ = false;
        return;
    }

    broadcaster_.registerUser(username_, fd_);
    LOG_INFO("session", "User '" << username_ << "' joined (fd=" << fd_ << ")");
    broadcaster_.broadcast(Protocol::makeSys("User " + username_ + " joined"));

    while (running_ && readLine(line)) {
        auto msg = Protocol::parse(line);
        if (msg.type != MessageType::MSG) continue;

        const std::string& text = msg.payload;
        if (text.rfind("/private ", 0) == 0) {

            auto after = text.substr(9); 
            auto space = after.find(' ');
            if (space == std::string::npos) {       
                std::string err = Protocol::makeError("Usage: /private <username> <message>");
                send(fd_, err.c_str(), err.size(), MSG_NOSIGNAL);
                continue;
            }

            std::string recipient = after.substr(0, space);
            std::string privateText = after.substr(space + 1);

            if (recipient == username_) {
                std::string err = Protocol::makeError("You cannot send a private message to yourself.");
                send(fd_, err.c_str(), err.size(), MSG_NOSIGNAL);
                continue;
            }

            if (!broadcaster_.sendTo(recipient, Protocol::makePrivate(username_, privateText))) {
                std::string err = Protocol::makeError("User '" + recipient + "' not found.");
                send(fd_, err.c_str(), err.size(), MSG_NOSIGNAL);
            } else {
                std::string confirm = Protocol::makePrivateSent(recipient, privateText);
                send(fd_, confirm.c_str(), confirm.size(), MSG_NOSIGNAL);
                LOG_DEBUG("session", username_ << " -> [private] -> " << recipient);
            }
            continue;
        }

        LOG_DEBUG("session", username_ << ": " << text);
        broadcaster_.broadcast(Protocol::makeMsg(username_, text));
    }

    LOG_INFO("session", "User '" << username_ << "' left (fd=" << fd_ << ")");
    broadcaster_.unregisterUser(username_);
    broadcaster_.removeClient(fd_);
    broadcaster_.broadcast(Protocol::makeSys("User " + username_ + " disconnected"));
    running_ = false;
}