//
// Created by chanma on 2023/04/27.
//

/*
webserverの設定を扱うクラス
設定ファイルのパスを引数にとり、設定ファイルを読み込む
設定ファイルが無い場合はデフォルトの内容を設定する
設定ファイルのフォーマットは以下の通り
・設定ファイルの拡張子は.Conf



扱う情報は以下の通り
・サーバーのポート番号
・サーバーのルートディレクトリ
・サーバーのindexファイル
・サーバーのエラーページ
*/

#ifndef MYWEBSERV_CONF_HPP
#define MYWEBSERV_CONF_HPP

#include <vector>
#include <sys/socket.h>
#include <string>
#include <map>
#include <set>
#include <netinet/in.h>
// デバッグ用
#include <iostream>

// 複数指定不可の単一のみの設定項目で、複数指定された場合は最後の一つだけ保持する
enum match_type {
    prefix, // 前方一致
    back,   // 後方一致
};

enum method {
    GET = 1,
    POST = 2,
    DELETE = 4,
};

struct Location {
    match_type match_; // 後方一致は、CGIの場合のみ使用可能
    std::set<method> allow_method_; // GET POST DELETE から１個以上指定 ビット演算で複数指定可
    size_t client_max_body_size_; // 任意 単一 デフォルト１MB, 0は無制限 制限超え
    // 413 Request Entity Too Large
    // 制限されるのはボディ部分でヘッダーは含まない
    std::string root_; // 必須 単一 相対パスの起点
    std::string index_; // 任意 単一 ディレクトリへのアクセス時に返すファイル
    bool is_cgi_; // デフォルトはfalse 一致する場合は、CGIとして処理する
    std::string cgi_executor_; // execve(cgi_path, X, X) is_cgi_がtrueの場合必須
    std::map<int, std::string> error_pages_;
    // 任意 複数可 mapのkeyはエラーコード, valueはエラーページのパス
    // エラーコードに重複があった場合は、最後の一つだけ保持する（上書きされる）
    bool autoindex_;     // 任意 単一 デフォルトはfalse
    std::string return_; // 任意 単一
    // HTTPステータスコード301と共に<path>へリダイレクトする｡
    // std::string upload_path_;
};

struct Server { // 各バーチャルサーバーの設定を格納する
    struct sockaddr_in listen_; // 必須 単一

    std::vector<std::string> server_names_;
    // 任意 単一 ディレクティブは一つで、複数指定された場合は最後の一つだけ保持
    // 一つのディレクティブ内に、サーバーネームは並べて複数可能

    std::vector<Location> locations_; // 任意 複数可
};

class Conf {
public:
    Conf();

    Conf(std::string conf_path);

    void setConfServer(const std::vector<Server> &server_vec) {
      server_vec_ = server_vec;
    };

    void parseConfigFile(std::string conf_path);

private:
    std::vector<Server> server_vec_;

public:
    // server_vec_を出力する関数
    void printConf() {
      for (std::vector<Server>::iterator it = server_vec_.begin(); it != server_vec_.end(); ++it) {
        std::cout << "listen: " << it->listen_.sin_port << std::endl;
        std::cout << "server_name: ";
        for (std::vector<std::string>::iterator it2 = it->server_names_.begin();
             it2 != it->server_names_.end(); ++it2) {
          std::cout << *it2 << " ";
        }
        std::cout << std::endl;
        for (std::vector<Location>::iterator it3 = it->locations_.begin(); it3 != it->locations_.end(); ++it3) {
          std::cout << "location: " << it3->root_ << std::endl;
          // allow_method_の出力
          std::set<method>::iterator it4 = it3->allow_method_.begin();
          std::cout << "allow_method: ";
          for (; it4 != it3->allow_method_.end(); ++it4) {
            if (*it4 == GET)
              std::cout << "GET ";
            else if (*it4 == POST)
              std::cout << "POST ";
            else if (*it4 == DELETE)
              std::cout << "DELETE ";
          }
          std::cout << std::endl;
          std::cout << "index: " << it3->index_ << std::endl;
          std::cout << "is_cgi: " << it3->is_cgi_ << std::endl;
          std::cout << "cgi_executor: " << it3->cgi_executor_ << std::endl;
          std::cout << "autoindex: " << it3->autoindex_ << std::endl;
          std::cout << "return: " << it3->return_ << std::endl;
          std::cout << "error_pages: ";
          for (std::map<int, std::string>::iterator it4 = it3->error_pages_.begin();
               it4 != it3->error_pages_.end(); ++it4) {
            std::cout << it4->first << " " << it4->second << " ";
          }
          std::cout << std::endl;
        }
      }
    }

};


#endif //MYWEBSERV_CONF_HPP
