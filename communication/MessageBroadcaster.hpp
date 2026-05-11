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
    
    // Registers a username-to-fd mapping when a client finishes login.
    void registerUser(const std::string& username, int fd);

    // Removes the mapping when a client disconnects.
    void unregisterUser(const std::string& username);

    // Returns true if the username is already present in userMap_.
    // Called before accepting a login to enforce uniqueness.
    bool isUsernameTaken(const std::string& username);

    // Sends a message directly to a single user by username.
    // Returns true on success, false if the user is not found or send fails.
    bool sendTo(const std::string& username, const std::string& message);
        std::unordered_map<int, bool> clients_;
        std::mutex mutex_;

private:
    // Maps connected usernames to their socket file descriptors.
    // Used for private message routing and duplicate-username detection.
    std::unordered_map<std::string, int> userMap_;
    
};