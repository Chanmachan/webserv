//
// Created by chanma on 2023/04/20.
//

#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "socket failed" << std::endl;
    return 1;
  }

  int opt = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    std::cerr << "setsockopt failed" << std::endl;
    return 1;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(8080);

  if (bind(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "bind failed" << std::endl;
    return 1;
  }

  if (listen(sockfd, 5) < 0) {
    std::cerr << "listen failed" << std::endl;
    return 1;
  }

  std::cout << "Server is listening on port 8080" << std::endl;

  sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  int newsockfd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_size);
  if (newsockfd < 0) {
    std::cerr << "accept failed" << std::endl;
    return 1;
  }

  char buffer[256];
  ssize_t bytes_received;
  while ((bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
    buffer[bytes_received] = '\0';
    std::cout << "Received: " << buffer << std::endl;

    ssize_t bytes_sent = send(newsockfd, buffer, bytes_received, 0);
    if (bytes_sent < 0) {
      std::cerr << "send failed" << std::endl;
      return 1;
    }
  }

  close(newsockfd);
  close(sockfd);

  return 0;
}
