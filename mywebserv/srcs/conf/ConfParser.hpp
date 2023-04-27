//
// Created by chanma on 2023/04/27.
//

#ifndef MYPROJECT_CONFPARSER_HPP
#define MYPROJECT_CONFPARSER_HPP

#include "Conf.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

class ConfParser {
public:
    ConfParser();
    ~ConfParser();
    std::vector<Server> parseConfigFile(const std::string& conf_path);
    bool readFile(const std::string& conf_path);
    void parseServerBlock(std::vector<Server>& servers);
    std::string getWord();
    bool isDelimiter();
    bool isEof();
private:
    std::string file_data_;
    size_t index_;

public:
    //テスト用
    void setFileData(std::string file_data) {
        file_data_ = file_data;
    }
    std::string getFileData() {
        return file_data_;
    }

};


#endif //MYPROJECT_CONFPARSER_HPP
