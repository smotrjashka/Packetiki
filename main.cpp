#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <thread>
#include <memory>

#include "proccom.h"
#include "thread_handler.h"
#include "storage.h"


int main(int argc, char* argv[]) {


    if (argc < 2) {
        std::cout << "You forget to pass the N value!!!";
        return 0;
    }

    const int N = atoi(argv[1]);

    if (N <= 0) {
        std::cout << "You wrote the wrong argument!!!" << std::endl << "It must be a number grater than 0";
        return 0;
    }

    storage* stor_ptr = new storage();

    std::vector<std::string> commands_vector;
    bool is_dynamik_started = false;
    int count_start_breakets = 0;
    bool dynamik_need_to_stop = false;

    std::thread log_thread(&thread_handler::log_thrd);
    std::thread file1_thread(&thread_handler::file1_thrd);
    std::thread file2_thread(&thread_handler::file2_thrd);

    std::string current_line;
    std::chrono::time_point<std::chrono::system_clock> start_time_millisec = std::chrono::system_clock::now();

    int numbe = 0;
    while (std::getline(std::cin, current_line)) {

        if (Utils::trim(current_line) == "{") {
            if (!is_dynamik_started) {
                is_dynamik_started = true;
                thread_handler::treat_finished(numbe++, commands_vector, start_time_millisec);
                //тут не будем придираться к тому что комманда была введена немного раньше мы ж типа супер быстро всё делаем)))
                start_time_millisec = std::chrono::system_clock::now();
            }
            count_start_breakets++;
        }
        else if (Utils::trim(current_line) == "}") {
            count_start_breakets--;
            if (count_start_breakets == 0) {
                dynamik_need_to_stop = true;
            }
        }
        else {
            if (commands_vector.empty()) {
                start_time_millisec = std::chrono::system_clock::now();
            }
            commands_vector.push_back(current_line);
        }
           if ((!is_dynamik_started && commands_vector.size() == N) || (is_dynamik_started && dynamik_need_to_stop)) {
               thread_handler::treat_finished(numbe++, commands_vector, start_time_millisec);
                if (is_dynamik_started) {
                  is_dynamik_started = false;
                    dynamik_need_to_stop = false;
                }
            }
        
    }




    return 0;
}


