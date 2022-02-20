#include "thread_handler.h"

void thread_handler::log_thrd() {
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


void thread_handler::file_thrd(std::queue<std::tuple<const std::shared_ptr<std::vector<std::string>>, 
    std::chrono::time_point<std::chrono::system_clock>, int>>& file_queue,
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

void thread_handler::file1_thrd() {
    file_thrd(file1_queue, file1_mutex);
}


void thread_handler::file2_thrd() {
    file_thrd(file2_queue, file2_mutex);
}

void thread_handler::treat_finished(int numbe, std::vector<std::string>& commands_vector, 
    std::chrono::time_point<std::chrono::system_clock> start_time_millisec) {

    /// это проверочка чтоб не плодить пустых файлов если динамический блок начинается сразу за другим динамическим или по окончанию нормального или первой коммандой
    if (commands_vector.size() > 0) {
        std::vector<std::string>* fin_commands_vector_ptr = new std::vector<std::string>(std::move(commands_vector));
        std::shared_ptr<std::vector<std::string>> p1(fin_commands_vector_ptr);
        std::shared_ptr<std::vector<std::string>> p2(fin_commands_vector_ptr);
        log_writer(p1);
        file_writer(numbe, p2, start_time_millisec);
        commands_vector.clear();
    }
}

//async write log
void storage::log_writer(const std::shared_ptr<std::vector<std::string>> pVector) {
    std::lock_guard<std::mutex> guard{ log_mutex };
    log_queue.push(pVector.get());
}

void storage::push_to_file(std::queue<std::tuple<const std::shared_ptr<std::vector<std::string>>,
    std::chrono::time_point<std::chrono::system_clock>, int>>&file_queue, std::mutex& file_mutex,
    const std::shared_ptr<std::vector<std::string>> pVector, int file_number, std::chrono::time_point<std::chrono::system_clock> time) {
    std::lock_guard<std::mutex> guard{ file_mutex };
    file_queue.push(std::make_tuple(pVector, time, file_number));
}

//async write text
void storage::file_writer(int file_number, const std::shared_ptr<std::vector<std::string>> pVector,
    std::chrono::time_point<std::chrono::system_clock> time) {
    //тут я решила не лочить переменную ибо типа размер вектора больше как ориентир и нам не нужно точное значение
    if (file1_queue.size() < file2_queue.size() || ((file1_queue.size() == file2_queue.size()) && file_number % 2 == 1)) {
        push_to_file(file2_queue, file2_mutex, pVector, file_number, time);
    }
    else {
        push_to_file(file1_queue, file1_mutex, pVector, file_number, time);
    }
}