#include "ClientManager.hpp"
#include "../common/Logger.hpp"
#include <algorithm>

ClientManager::ClientManager(MessageBroadcaster& broadcaster)
    : broadcaster_(broadcaster) {}

void ClientManager::addClient(int fd) {
    broadcaster_.addClient(fd);
    auto session = std::make_unique<ClientSession>(fd, broadcaster_);
    session->start();
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.push_back(std::move(session));
}

void ClientManager::clean() {
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(
        std::remove_if(sessions_.begin(), sessions_.end(),
            [](const std::unique_ptr<ClientSession>& s) {
                return !s->isRunning();
            }),
        sessions_.end());
}

void ClientManager::stopAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    LOG_INFO("manager", "Stopping all " << sessions_.size() << " session(s)");
    for (auto& s : sessions_) s->stop();
    sessions_.clear();
}