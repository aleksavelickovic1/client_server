#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include "ClientSession.hpp"
#include "MessageBroadcaster.hpp"

class ClientManager {
public:
    explicit ClientManager(MessageBroadcaster& broadcaster);

    void addClient(int fd);

    void clean();

    void stopAll();

private:
    std::vector<std::unique_ptr<ClientSession>> sessions_;
    MessageBroadcaster& broadcaster_;
    std::mutex mutex_;
};