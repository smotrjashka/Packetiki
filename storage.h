#pragma once
#include <memory>
#include <queue>

struct storage {

public:


	std::queue<const std::vector<std::string>*> log_queue;
	std::queue<std::tuple<const std::shared_ptr<std::vector<std::string>>, std::chrono::time_point<std::chrono::system_clock>, int>> file1_queue;
	std::queue<std::tuple<const std::shared_ptr<std::vector<std::string>>, std::chrono::time_point<std::chrono::system_clock>, int>> file2_queue;

	std::mutex log_mutex;
	std::mutex file1_mutex;
	std::mutex file2_mutex;


};