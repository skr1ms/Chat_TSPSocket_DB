#pragma once
#include <iostream>
#include <fstream>
#include <thread>
#include <shared_mutex>
#include <chrono>
#include <string>
class Logger {
public:
	Logger(const std::string& _fname, const std::string& _input_str);
	~Logger();
	void insertLog();
	void getInfoAboutMessage();
private:
	std::string input_string;
	std::string filename;
	std::shared_mutex m;
	std::ofstream fs;
};