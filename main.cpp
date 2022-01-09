#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <thread>

#include "proccom.h"

std::queue<const std::vector<std::string>*> log_queue;
std::queue<std::tuple<const std::vector<std::string>*, std::chrono::time_point<std::chrono::system_clock>, int>> file1_queue;
std::queue<std::tuple<const std::vector<std::string>*, std::chrono::time_point<std::chrono::system_clock>, int>> file2_queue;

std::mutex log_mutex;
std::mutex file1_mutex;
std::mutex file2_mutex;

void log_thrd() {
    //??? а что если мы батц и закрыли прогу?
    while (true) {
        if (log_queue.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else {
            const std::vector<std::string>* pVec = log_queue.front();
            Utils::print_commands(pVec);
            if (log_mutex.try_lock()) {
                log_queue.pop();
                log_mutex.unlock();
            }
            else {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
    }
}

void file_thrd(std::queue<std::tuple<const std::vector<std::string>*, std::chrono::time_point<std::chrono::system_clock>, int>>& file_queue,
    std::mutex& file_mutex) {
    //??? а что если мы батц и закрыли прогу?
    while (true) {
        if (file_queue.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else {
            Utils::save_to_file(std::get<0>(file_queue.front()), std::get<1>(file_queue.front()), std::get<2>(file_queue.front()));
            if (file_mutex.try_lock()) {

                file_queue.pop();
                file_mutex.unlock();
            }
            else {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
    }
}

void file1_thrd() {
    file_thrd(file1_queue, file1_mutex);
}

void file2_thrd() {
    file_thrd(file2_queue, file2_mutex);
}

//async write log
void log_writer(const std::vector<std::string>* pVector) {
    std::lock_guard<std::mutex> guard{ log_mutex };
    log_queue.push(pVector);
}

void push_to_file(std::queue<std::tuple<const std::vector<std::string>*, std::chrono::time_point<std::chrono::system_clock>, int>>& file_queue, std::mutex& file_mutex,
    const std::vector<std::string>* pVector, int file_number, std::chrono::time_point<std::chrono::system_clock> time) {
    std::lock_guard<std::mutex> guard{ file_mutex };
    file_queue.push(std::make_tuple(pVector, time, file_number));
}

//async write text
void file_writer(int file_number, const std::vector<std::string>* pVector, std::chrono::time_point<std::chrono::system_clock> time) {
    //тут я решила не лочить переменную ибо типа размер вектора больше как ориентир и нам не нужно точное значение
    if (file1_queue.size() < file2_queue.size() || ((file1_queue.size() == file2_queue.size()) && file_number % 2 == 1)) {
        push_to_file(file2_queue, file2_mutex, pVector, file_number, time);
    }
    else {
        push_to_file(file1_queue, file1_mutex, pVector, file_number, time);
    }
}

void treat_finished(int numbe, std::vector<std::string>& commands_vector, std::chrono::time_point<std::chrono::system_clock> start_time_millisec) {
    
    /// это проверочка чтоб не плодить пустых файлов если динамический блок начинается сразу за другим динамическим или по окончанию нормального или первой коммандой
    if (commands_vector.size() > 0) {
        std::vector<std::string>* fin_commands_vector = new std::vector<std::string>(std::move(commands_vector));
       log_writer(fin_commands_vector);
        //TODO commented for debug porpouses
        file_writer(numbe, fin_commands_vector, start_time_millisec);
       commands_vector.clear();
    }
}

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

    std::vector<std::string> commands_vector;
    bool is_dynamik_started = false;
    int count_start_breakets = 0;
    bool dynamik_need_to_stop = false;

    std::thread log_thread(&log_thrd);
    std::thread file1_thread(&file1_thrd);
    std::thread file2_thread(&file2_thrd);

    std::string current_line;
    std::chrono::time_point<std::chrono::system_clock> start_time_millisec = std::chrono::system_clock::now();

    int numbe = 0;
    while (std::getline(std::cin, current_line)) {

        if (Utils::trim(current_line) == "{") {
            if (!is_dynamik_started) {
                is_dynamik_started = true;
                treat_finished(numbe++, commands_vector, start_time_millisec);
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
               treat_finished(numbe++, commands_vector, start_time_millisec);
                if (is_dynamik_started) {
                  is_dynamik_started = false;
                    dynamik_need_to_stop = false;
                }
            }
        
    }




    return 0;
}


