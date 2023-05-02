//
// Created by chanma on 2023/04/27.
//

#include "ConfParser.hpp"

ConfParser::ConfParser() : index_(0), pos_(0) {}

ConfParser::~ConfParser() {}

std::vector<Server> ConfParser::parseConfigFile(const std::string &conf_path) {
  // configファイルを読み込む
  if (!readFile(conf_path)) {
    throw std::runtime_error("Failed to read config file.");
  }
  // serverブロックをパースする
  std::vector<Server> servers;
  parseServerBlocks(servers);

  return servers;
}

// "server"文字列があるごとにwhileループを回す
void ConfParser::parseServerBlocks(std::vector<Server> &servers) {
  // "server"文字列があるごとにwhileループを回す
  // posは"}"

  while ((pos_ = file_data_.find("server", pos_)) != std::string::npos) {
    pos_ += 6; // "server"の次の文字から
    Server server;
    if (!parseServerBlock(server)) {
      std::cerr << "Error: Failed to parse server block." << std::endl;
      exit(false);
    }
    servers.push_back(server);
  }


}

// サーバーブロックひとつひとつをパースする
bool ConfParser::parseServerBlock(Server &server) {
  skipSpace();
  // 次の文字が"{"でなければエラー
  if (file_data_[pos_] != '{') {
    std::cerr << "Error: Expected '{' after server block." << std::endl;
    return false;
  }

  pos_++; // "{"の次の文字から

  while (!isEof()) {
    std::string word = getWord();
    if (word == "listen") {
      // listenディレクティブをパースする
      parseListenDirective(server);
    } else if (word == "server_name") {
      // server_nameディレクティブをパースする
      parseServerNameDirective(server);
    } else if (word == "location") {
      // locationディレクティブをパースする
      parseLocationBlock(server);
    } else if (word == "}") {
      break;
    }
  }

  return true;
}

// locationブロックをパースする
void ConfParser::parseLocationBlock(Server &server) {
  Location location;
  // "{"まで読み飛ばす
  while (file_data_[pos_] != '{') {
    pos_++;
  }
  pos_++; // "{"の次の文字から
  // rootを設定
  std::string word;
  // "}"が見つかるまでループ
  while (true) {
    skipSpace();
    if (!isalnum(file_data_[pos_]) && file_data_[pos_] == '}') {
      break;
    } else if (isEof()) {
      std::cerr << "Error: Expected '}' after location block." << std::endl;
      exit(false);
    }
    word = getWord();
    if (word == "root") {
      location.root_ = getWord();
    } else if (word == "index") {
      location.index_ = getWord();
    } else if (word == "error_page") {
      std::cout << "error_page" << std::endl;
    }
  }
  // locationをserver.locations_に格納する
  server.locations_.push_back(location);
}

// server_nameディレクティブをパースする
void ConfParser::parseServerNameDirective(Server &server) {
  std::string word;
  // server.server_name_に格納する
  // セミコロンまでループを回す
  // 空白区切りでserver_nameにpush_backする
  while (true) {
    skipSpace();
    //　黄色で何回ここの処理に来たか出力
    std::cout << "\033[33m" << "parseServerNameDirective: " << "\033[0m" << std::endl;
    if (file_data_[pos_] == ';') {
      pos_++;
      break;
    } else if (isEof()) {
      std::cerr << "Error: Expected ';' after server_name directive." << std::endl;
      exit(false);
    }
    word = getWord();
    server.server_names_.push_back(word);
  }
  for (size_t i = 0; i < server.server_names_.size(); i++) {
    std::cout << "\033[36m" << "server_name: " << &server.server_names_ << "\033[0m";
  }
}

// listenディレクティブをパースする
void ConfParser::parseListenDirective(Server &server) {
  // listenの次の文字列を取得する
  std::string word = getWord();
  // wordが数字でなければエラー
  for (size_t i = 0; i < word.size(); i++) {
    if (!isdigit(word[i])) {
      std::cerr << "Error: in parseListenDirective: port number is not a number." << std::endl;
      exit(false);
    }
  }
  // server.listen_に格納する
  server.listen_.sin_family = AF_INET;
  server.listen_.sin_addr.s_addr = INADDR_ANY;
  server.listen_.sin_port = htons(std::stoi(word));

}

bool ConfParser::readFile(const std::string &conf_path) {
  std::cout << "Trying to open file: " << conf_path << std::endl;
  std::ifstream ifs(conf_path);
  if (!ifs) {
    std::cerr << "Failed to open file." << std::endl;
    return false;
  }
  std::stringstream ss;
  ss << ifs.rdbuf();
  file_data_ = ss.str();
  return true;
}

// delimiterで区切られた文字列を返す
std::string ConfParser::getWord() {
  std::string word;
  while (!isEof() && !isspace(file_data_[pos_]) && !isDelimiter()) {
    word += file_data_[pos_];
    pos_++;
  }
  pos_++;
  return word;
}

bool ConfParser::isDelimiter() {
  if (isspace(file_data_[pos_]) || file_data_[pos_] == ';' \
 || file_data_[pos_] == '{' || file_data_[pos_] == '}') {
    return true;
  }
  return false;
}

// eofかを判定する
bool ConfParser::isEof() {
  return pos_ >= file_data_.size();
}

// 空白文字をスキップする
void ConfParser::skipSpace() {
  while (!isEof() && isspace(file_data_[pos_])) {
    pos_++;
  }
}