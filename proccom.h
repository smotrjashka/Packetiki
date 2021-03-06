#pragma once
#ifndef PROCCOM_H
#define PROCCOM_H

#include <vector>
#include <string>
#include <chrono>
#include <memory>

namespace Utils {

    void print_commands(const std::shared_ptr<std::vector<std::string>> pVector, std::ostream& strm = std::cout);

    void save_to_file(const std::shared_ptr<std::vector<std::string>> pVector, std::chrono::time_point<std::chrono::system_clock> time_point_start, int postfix);

    std::string trim(const std::string& str);

};


#endif