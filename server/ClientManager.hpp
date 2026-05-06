#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include "ClientSession.hpp"
#include "MessageBroadcaster.hpp"

class ClientManager {
public:
    // Initializes manager with shared broadcaster
    explicit ClientManager(MessageBroadcaster& broadcaster);

    // Creates and starts a new client session
    void addClient(int fd);

    // Removes sessions that are no longer running
    void clean();

    // Stops all sessions and clears them
    void stopAll();

private:
    std::vector<std::unique_ptr<ClientSession>> sessions_;
    MessageBroadcaster& broadcaster_;
    std::mutex mutex_;
};