#pragma once
#include <string>
#include <unordered_map>
#include <mutex>


class MessageBroadcaster {
public:
    void addClient(int fd);
    void removeClient(int fd);
    void broadcast(const std::string& message, int excludeFd = -1);

private:
    std::unordered_map<int, bool> clients_;
    std::mutex mutex_;
};