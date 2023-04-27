//
// Created by chanma on 2023/04/27.
//

#include "ConfParser.hpp"

ConfParser::ConfParser(): index_(0) {}

ConfParser::~ConfParser() {}

std::vector<Server> ConfParser::parseConfigFile(const std::string &conf_path) {
  // configファイルを読み込む
  if (!readFile(conf_path)) {
    throw std::runtime_error("Failed to read config file.");
  }

  std::vector<Server> servers;
  return servers;
}

void ConfParser::parseServerBlock(std::vector<Server> &servers) {
  // file_data_を改行で分割する

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
    while (!isEof() && !isspace(file_data_[index_]) && !isDelimiter()) {
        word += file_data_[index_];
        index_++;
    }
    index_++;
    return word;
}

bool ConfParser::isDelimiter() {
    if (file_data_[index_] == ' ' || file_data_[index_] == ';' \
        || file_data_[index_] == '{' || file_data_[index_] == '}') {
        return true;
    }
    return false;
}

// eofかを判定する
bool ConfParser::isEof() {
    if (index_ >= file_data_.size()) {
        return true;
    }
    return false;
}