#include "MessageBroadcaster.hpp"
#include "../common/Logger.hpp"
#include <sys/socket.h>

void MessageBroadcaster::addClient(int fd) {
    std::lock_guard<std::mutex> lock(mutex_);
    clients_[fd] = true;
    LOG_DEBUG("broadcaster", "Added client fd=" << fd << ", total=" << clients_.size());
}

void MessageBroadcaster::removeClient(int fd) {
    std::lock_guard<std::mutex> lock(mutex_);
    clients_.erase(fd);
    LOG_DEBUG("broadcaster", "Removed client fd=" << fd << ", total=" << clients_.size());
}

void MessageBroadcaster::broadcast(const std::string& message, int excludeFd) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& [fd, alive] : clients_) {
        if (fd == excludeFd) continue;
        if (send(fd, message.c_str(), message.size(), MSG_NOSIGNAL) < 0)
            LOG_WARN("broadcaster", "Failed to send to fd=" << fd);
    }
}

void MessageBroadcaster::registerUser(const std::string& username, int fd) {
    std::lock_guard<std::mutex> lock(mutex_);
    userMap_[username] = fd;
}

void MessageBroadcaster::unregisterUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(mutex_);
    userMap_.erase(username);
}

bool MessageBroadcaster::isUsernameTaken(const std::string& username) {
    std::lock_guard<std::mutex> lock(mutex_);
    return userMap_.count(username) > 0;
}

bool MessageBroadcaster::sendTo(const std::string& username, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = userMap_.find(username);
    if (it == userMap_.end()) return false;
    if (send(it->second, message.c_str(), message.size(), MSG_NOSIGNAL) < 0) {
        LOG_WARN("broadcaster", "Failed to send private to " << username);
        return false;
    }
    return true;
}