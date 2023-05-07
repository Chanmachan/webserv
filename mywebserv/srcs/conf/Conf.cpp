//
// Created by chanma on 2023/04/27.
//

#include "Conf.hpp"
#include "ConfParser.hpp"

Conf::Conf() {

}

Conf::Conf(std::string conf_path) {
  (void )conf_path;
}

void Conf::parseConfigFile(std::string conf_path) {
  ConfParser conf_parser;
  conf_parser.parseConfigFile(conf_path, *this);
}