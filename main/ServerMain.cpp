#include "server/Server.hpp"
#include "common/Logger.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {
    int port = 9000;
    if (argc > 1) port = std::stoi(argv[1]);

    Logger::instance().setMinLevel(LogLevel::DEBUG);
    Logger::instance().setLogFile("../server.log");
    Logger::instance().setConsoleLogging(false); 
    
    try {
        Server server(port);
        server.run();
    } catch (const std::exception& ex) {
        LOG_ERROR("main", ex.what());
        return 1;
    }
    return 0;
}