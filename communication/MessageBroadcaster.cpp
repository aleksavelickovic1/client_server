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