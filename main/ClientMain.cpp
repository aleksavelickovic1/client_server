#include <iostream>
#include <stdexcept>
#include <string>
#include "common/Logger.hpp"

// Prints program usage instructions
static void printUsage() {
    std::cerr << "Usage: chat_client <host> <port> [--ui | --cli]\n";
    std::cerr << "  --cli  (default) run in terminal mode\n";
    std::cerr << "  --ui            run with graphical interface\n";
}

#include "client/Client.hpp"

// Runs client in CLI mode
static int runCli(const std::string& host, int port) {
    try {
        Client client(host, port);
        client.run();
    } catch (const std::exception& ex) {
        LOG_ERROR("main", ex.what());
        return 1;
    }
    return 0;
}

#ifdef CHAT_UI_ENABLED
#include <QApplication>
#include "ui/ChatWindow.hpp"

// Runs client with UI
static int runUi(int argc, char* argv[], const std::string& host, int port) {
    try {
        QApplication app(argc, argv);
        ChatWindow   window(host, port);
        window.show();
        return app.exec();
    } catch (const std::exception& ex) {
        LOG_ERROR("main", ex.what());
        return 1;
    }
}
#endif

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }

    Logger::instance().setMinLevel(LogLevel::DEBUG);
    Logger::instance().setLogFile("../client.log");
    Logger::instance().setConsoleLogging(false); 

    std::string host = argv[1];
    int         port = std::stoi(argv[2]);

    bool useUi = false;
    if (argc >= 4) {
        std::string mode = argv[3];
        if (mode == "--ui") {
            useUi = true;
        } else if (mode == "--cli") {
            useUi = false;
        } else {
            std::cerr << "Unknown mode: " << mode << "\n";
            printUsage();
            return 1;
        }
    }

#ifdef CHAT_UI_ENABLED
    if (useUi) return runUi(argc, argv, host, port);
#else
    if (useUi) {
        LOG_ERROR("main", "This build was compiled without UI support");
        return 1;
    }
#endif

    return runCli(host, port);
}