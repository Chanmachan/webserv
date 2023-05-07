//
// Created by chanma on 2023/05/07.
//

#include "Socket.hpp"
#include <cstring>
#include <unistd.h>

/* ASocket */

ASocket::ASocket() : sock_fd_(-1), conf_(NULL) {
  memset(&sockaddr_in_, 0, sizeof(sockaddr_in_));
}

ASocket::~ASocket() {
  close(sock_fd_);
}

int ASocket::setNonBlocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


/* ConnectSocket */

ConnectSocket::ConnectSocket() : ASocket() {

}

ConnectSocket::~ConnectSocket() {

}


/* ListenSocket */

ListenSocket::ListenSocket(std::vector<Server> &confs) : ASocket() {
  conf_ = confs;
}

ListenSocket::~ListenSocket() {

}

int ListenSocket::createSocket() {
  sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd_ == -1) {
    throw std::runtime_error("socket() failed");
  }
  return setNonBlocking(sock_fd_);
}

bool ListenSocket::setupSocket() {
  // sockaddr_in_の設定
  std::string ip = conf_[0].listen_.listen_ip_;
  int port = conf_[0].listen_.listen_port_;
  sockaddr_in_.sin_family = AF_INET;
  sockaddr_in_.sin_addr.s_addr = inet_addr(ip.c_str());
  sockaddr_in_.sin_port = htons(port);

  if (bind(sock_fd_, (sockaddr *) &sockaddr_in_, sizeof(sockaddr_in_)) == -1) {
    std::cerr << "bind: " << strerror(errno) << std::endl;
    return false;
  }

  if (listen(sock_fd_, SOMAXCONN) == -1) {
    std::cerr << "listen: " << strerror(errno) << std::endl;
    return false;
  }
}

