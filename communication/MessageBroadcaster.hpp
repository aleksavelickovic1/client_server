#pragma once
#include <string>
#include <unordered_map>
#include <mutex>

class MessageBroadcaster {
public:
    // Adds new client file descriptor to broadcast list
    void addClient(int fd);

    // Removes client from broadcast list
    void removeClient(int fd);

    // Sends message to all clients except excluded one
    void broadcast(const std::string& message, int excludeFd = -1);

private:
    std::unordered_map<int, bool> clients_;
    std::mutex mutex_;
};