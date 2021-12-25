#include <iostream>
#include "Utils.h"
#include <fstream>

void Utils::print_commands(std::vector<std::string> *pVector, std::ostream& strm ) {

    strm << "bulk:";
    for(std::string command : (*pVector)){
        strm << " " << command << ",";
    }

    strm<<'\b'<<" ";
    strm<<std::endl;
}

void Utils::save_to_file(std::vector<std::string> *pVector, std::chrono::time_point<std::chrono::system_clock> time_point_start) {

    auto start_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(time_point_start);
    auto start_casted = std::chrono::duration_cast<std::chrono::milliseconds>(start_ms.time_since_epoch());
     std::string time_start = std::to_string(start_casted.count());
     std::string  file_name = "bulk" + time_start + ".log";

     std::ofstream log_file;
     log_file.open(file_name);

    print_commands(pVector, log_file);

     log_file.close();

}

std::string Utils::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}