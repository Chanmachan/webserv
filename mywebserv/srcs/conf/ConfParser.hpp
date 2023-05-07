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
    void parseConfigFile(const std::string& conf_path, Conf& conf);
    bool readFile(const std::string& conf_path);
    void parseServerBlocks(std::vector<Server>& servers);
    bool parseServerBlock(Server& server);
    void parseListenDirective(Server& server);
    void parseServerNameDirective(Server& server);
    void parseLocationBlock(Server& server);
    void parseAllowMethods(Location& location);
    std::string getWord();
    bool isDelimiter();
    bool isEof();
    void skipSpace();
private:
    std::string file_data_;
    std::string::size_type pos_;

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
