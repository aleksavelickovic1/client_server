#pragma once
#include <string>
#include <utility>


enum class MessageType { LOGIN, MSG, SYS, UNKNOWN };

struct Message {
    MessageType type = MessageType::UNKNOWN;
    std::string payload;
};

namespace Protocol {

inline std::string makeLogin(const std::string& username) {
    return "LOGIN|" + username + "\n";
}

inline std::string makeMsg(const std::string& username, const std::string& text) {
    return "MSG|" + username + ": " + text + "\n";
}

inline std::string makeSys(const std::string& text) {
    return "SYS|" + text + "\n";
}

inline Message parse(const std::string& line) {
    Message msg;
    auto sep = line.find('|');
    if (sep == std::string::npos) return msg;

    std::string tag  = line.substr(0, sep);
    std::string body = line.substr(sep + 1);

    if (tag == "LOGIN") 
    {
         msg.type = MessageType::LOGIN; 
         msg.payload = body; 
    }
    else if (tag == "MSG") 
    { 
        msg.type = MessageType::MSG;   
        msg.payload = body; 
    }
    else if (tag == "SYS") 
    { 
        msg.type = MessageType::SYS;   
        msg.payload = body; 
    }

    return msg;
}

}