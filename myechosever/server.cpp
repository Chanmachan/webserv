//
// Created by chanma on 2023/04/20.
//

#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <cerrno>
#include <sys/epoll.h>

using namespace std;

int error_exit(const char* msg) {
  cerr << "Error: in " << msg << endl;
  exit(1);
}

int set_nonblocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error_exit("socket()");
  }

  int opt = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    error_exit("setsocketopt()");
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(8080);

  if (bind(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind: ");
    error_exit("bind()");
  }

  if (listen(sockfd, 5) < 0) {
    error_exit("listen()");
  }

  if (set_nonblocking(sockfd) < 0) {
    error_exit("set_nonblocking()");
  }

  int epoll_fd = epoll_create1(0);
  if (epoll_fd < 0) {
    error_exit("epoll_create()");
  }

  epoll_event server_event;
  server_event.events = EPOLLIN | EPOLLOUT;
  server_event.data.fd = sockfd;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &server_event) < 0) {
    error_exit("epoll_ctl()");
  }

  const int max_events = 10;
  epoll_event events[max_events];

  cout << "Server is listening on port 8080" << endl;

  while (1) {
    // eventが起こるまで待つ
    // eventが起きるとevents配列に情報が格納される
    int num_events = epoll_wait(epoll_fd, events, max_events, -1);
    if (num_events < 0) {
      error_exit("epoll_wait");
    }

    for (int i = 0; i < num_events; ++i) {
      // 新しいクライアント接続
      if (events[i].data.fd == sockfd) {
        sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int newsockfd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_size);
        if (newsockfd < 0) {
          cerr << "accept failed" << endl;
          continue;
        }

        if (set_nonblocking(newsockfd) < 0) {
          cerr << "set_nonblocking failed" << endl;
          close(newsockfd);
          continue;
        }

        epoll_event client_event;
        client_event.events = EPOLLIN | EPOLLOUT;
        client_event.data.fd = newsockfd;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, newsockfd, &client_event) < 0) {
          cerr << "epoll_ctl failed" << endl;
          close(newsockfd);
        }

        cout << "New connection accepted: " << newsockfd << endl;

        // クライアントからのデータを受信
      } else {
        int client_fd = events[i].data.fd;
        char buffer[256];
        ssize_t bytes_received;

        while ((bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
          buffer[bytes_received] = '\0';
          cout << "Received from " << client_fd << ": " << buffer << endl;

          ssize_t bytes_sent = send(client_fd, buffer, bytes_received, 0);
          if (bytes_sent < 0) {
            cerr << "send failed" << endl;
            break;
          }
        }

        if (bytes_received == 0 || (bytes_received < 0 && errno != EAGAIN && errno != EWOULDBLOCK)) {
          close(client_fd);
          cout << "Connection closed: " << client_fd << endl;
        }
      }
    }
  }

  close(sockfd);
  close(epoll_fd);

  return 0;
}
