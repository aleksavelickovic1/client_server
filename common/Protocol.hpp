#pragma once
#include <string>
#include <utility>
#include <chrono>
#include <ctime>

enum class MessageType { LOGIN, MSG, SYS, PRIVATE, ERROR, UNKNOWN };

struct Message {
    MessageType type = MessageType::UNKNOWN;
    std::string payload;
};

namespace Protocol {

    // Creates login message string
    inline std::string makeLogin(const std::string& username) {
        return "LOGIN|" + username + "\n";
    }

    // Returns a timestamped broadcast message string.
    // Format:  MSG|[HH:MM] username: text\n
    inline std::string makeMsg(const std::string& username, const std::string& text) {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        char buf[6];
        std::strftime(buf, sizeof(buf), "%H:%M", std::localtime(&t));
        return "MSG|[" + std::string(buf) + "] " + username + ": " + text + "\n";
    }

    // Creates system message string
    inline std::string makeSys(const std::string& text) {
        return "SYS|" + text + "\n";
    }

    // Returns an error notice string sent only to one client.
    // Format:  ERROR|text\n
    inline std::string makeError(const std::string& text) {
        return "ERROR|" + text + "\n";
    }

    // Returns a private message string addressed to the recipient's display.
    // Format:  PRIVATE|[HH:MM] [Private] sender: text\n
    
    inline std::string makePrivate(const std::string& sender, const std::string& text) {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        char buf[6];
        std::strftime(buf, sizeof(buf), "%H:%M", std::localtime(&t));
        return "PRIVATE|[" + std::string(buf) + "] [Private] " + sender + ": " + text + "\n";
    }

    inline std::string makePrivateSent(const std::string& recipient, const std::string& text) {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        char buf[6];
        std::strftime(buf, sizeof(buf), "%H:%M", std::localtime(&t));
        return "PRIVATE|[" + std::string(buf) + "] [To " + recipient + "]: " + text + "\n";
    }

    // Parses raw message line into Message struct
    inline Message parse(const std::string& line) {
        Message msg;

        auto sep = line.find('|');
        if (sep == std::string::npos) return msg;

        std::string tag  = line.substr(0, sep);
        std::string body = line.substr(sep + 1);

        if (tag == "LOGIN") {
            msg.type = MessageType::LOGIN;
            msg.payload = body;
        }

        else if (tag == "MSG") {
            msg.type = MessageType::MSG;
            msg.payload = body;
        }

        else if (tag == "SYS") {
            msg.type = MessageType::SYS;
            msg.payload = body;
        }

        else if (tag == "PRIVATE") {
        msg.type = MessageType::PRIVATE;
        msg.payload = body;
        }

        else if (tag == "ERROR") {
            msg.type = MessageType::ERROR;
            msg.payload = body;
        }

        return msg;
    }

}