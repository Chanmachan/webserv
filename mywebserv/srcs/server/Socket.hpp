//
// Created by chanma on 2023/05/07.
//

#ifndef MYPROJECT_SOCKET_HPP
#define MYPROJECT_SOCKET_HPP

#include "Server.hpp"
#include "Conf.hpp"
#include <vector>

// 継承用のsocketクラス
class ASocket {
protected:
    int sock_fd_;
    struct sockaddr_in sockaddr_in_;
    std::vector<Server> conf_;
public:
    ASocket();
    virtual ~ASocket() = 0;
    static int setNonBlocking(int fd);

    // ゲッター
    int getSockFd() const {
        return sock_fd_;
    };
    struct sockaddr_in getSockaddrIn() const {
        return sockaddr_in_;
    };
    // セッター
    void setSockFd(int sock_fd) {
        sock_fd_ = sock_fd;
    };
    void setSockaddrIn(struct sockaddr_in sockaddr_in) {
        sockaddr_in_ = sockaddr_in;
    };
    void setConf(std::vector<Server> conf) {
        conf_ = conf;
    };
};

// connect用のsocketクラス
class ConnectSocket : public ASocket {
private:
    std::vector<char> recv_buffer_; // 受信バッファ
    std::vector<char> send_buffer_; // 送信バッファ
public:
    ConnectSocket();
    ~ConnectSocket();
    void bindSocket(int port);
    void listenSocket();
    int acceptSocket();
};

// listen用のsocketクラス
// clientとの接続を受け付ける
class ListenSocket : public ASocket {
public:
    ListenSocket(std::vector<Server>& confs);
    ~ListenSocket();
    int createSocket();
    bool setupSocket();
    int acceptSocket();
};

#endif //MYPROJECT_SOCKET_HPP
