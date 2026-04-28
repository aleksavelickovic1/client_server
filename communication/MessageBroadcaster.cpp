#include "MessageBroadcaster.hpp"
#include <sys/socket.h>
#include <iostream>

void MessageBroadcaster::addClient(int fd) {
    std::lock_guard<std::mutex> lock(mutex_);
    clients_[fd] = true;
}

void MessageBroadcaster::removeClient(int fd) {
    std::lock_guard<std::mutex> lock(mutex_);
    clients_.erase(fd);
}

void MessageBroadcaster::broadcast(const std::string& message, int excludeFd) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& [fd, alive] : clients_) {
        if (fd == excludeFd) continue;
        if (send(fd, message.c_str(), message.size(), MSG_NOSIGNAL) < 0) {
            std::cerr << "[broadcaster] Failed to send to fd " << fd << "\n";
        }
    }
}