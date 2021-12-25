#ifndef BULK_UTILS_H
#define BULK_UTILS_H

#include <vector>
#include <string>
#include <chrono>

namespace Utils {

    void print_commands(std::vector<std::string> *pVector, std::ostream& strm = std::cout);

    void save_to_file(std::vector<std::string> *pVector, std::chrono::time_point<std::chrono::system_clock> time_point_start);

    std::string trim(const std::string& str);

};


#endif //BULK_UTILS_H
