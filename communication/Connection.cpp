#include "Connection.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>

Connection::~Connection() {
    close();
}

void Connection::connect(const std::string& host, int port) {
    addrinfo hints{}, *res = nullptr;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res) != 0)
        throw std::runtime_error("Cannot resolve host: " + host);

    fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd_ < 0) { freeaddrinfo(res); throw std::runtime_error("socket() failed"); }

    if (::connect(fd_, res->ai_addr, res->ai_addrlen) < 0) {
        freeaddrinfo(res);
        ::close(fd_); fd_ = -1;
        throw std::runtime_error("connect() failed: " + std::string(strerror(errno)));
    }
    freeaddrinfo(res);
}

bool Connection::send(const std::string& data) {
    if (fd_ < 0) return false;
    ssize_t sent = ::send(fd_, data.c_str(), data.size(), MSG_NOSIGNAL);
    return sent == static_cast<ssize_t>(data.size());
}

bool Connection::readLine(std::string& out) {
    out.clear();
    char ch;
    while (true) {
        ssize_t n = recv(fd_, &ch, 1, 0);
        if (n <= 0) return false;
        if (ch == '\n') return true;
        out += ch;
    }
}

void Connection::close() {
    if (fd_ >= 0) {
        ::shutdown(fd_, SHUT_RDWR);
        ::close(fd_);
        fd_ = -1;
    }
}