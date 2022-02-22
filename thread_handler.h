#pragma once

#include <queue>
#include <tuple>
#include <memory>

namespace thread_handler {

	void log_thrd();
	void file_thrd(std::queue<std::tuple<const std::shared_ptr<std::vector<std::string>>,
		std::chrono::time_point<std::chrono::system_clock>, int>>&file_queue,
		std::mutex& file_mutex);
	void file1_thrd();
	void file2_thrd();
	void treat_finished(int numbe, std::vector<std::string>& commands_vector,
		std::chrono::time_point<std::chrono::system_clock> start_time_millisec);
	void log_writer();
	void push_to_file(std::queue<std::tuple<const std::shared_ptr<std::vector<std::string>>,
		std::chrono::time_point<std::chrono::system_clock>, int>>&file_queue, std::mutex& file_mutex,
		const std::shared_ptr<std::vector<std::string>> pVector, int file_number,
		std::chrono::time_point<std::chrono::system_clock> time);
	void file_writer(int file_number, const std::shared_ptr<std::vector<std::string>> pVector,
		std::chrono::time_point<std::chrono::system_clock> time);
}