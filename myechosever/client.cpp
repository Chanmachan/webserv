//
// Created by chanma on 2023/04/21.
//

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main() {
  int socker_fd;    // ソケットハンドル
  struct sockaddr_in server;

  // TCPソケット
  socker_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socker_fd < 0) {
    cerr << "Error: in socket()" << endl;
    return 1;
  }
  memset(&server, 0, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = htons(8080);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");

  // ソケットを接続したいサーバと繋ぐ
  if (connect(socker_fd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0) {
    cerr << "Error: in connect()" << endl;
    return 1;
  }

  // サーバへ送信
  string input;
  while (getline(std::cin, input)) {
    ssize_t bytes_sent = send(socker_fd, input.c_str(), input.length(), 0);
    if (bytes_sent < 0) {
      cerr << "Error: in send()" << endl;
      return 1;
    }

    char buff[256];
    ssize_t bytes_recv = recv(socker_fd, buff, sizeof(buff) - 1, 0);
    if (bytes_recv < 0) {
      cerr << "Error: in recv()" << endl;
      return 1;
    }
    buff[bytes_recv] = '\0';
    cout << "Received from server: " << buff << endl;
  }

  close(socker_fd);
  return 0;
}