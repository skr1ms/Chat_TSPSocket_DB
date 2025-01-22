#include "logger.h"
Logger::Logger(const std::string &_fname, const std::string &_input_str) : filename(_fname), input_string(_input_str)
{
	fs.open(filename, std::ios::app);
	if (!fs)
	{
		std::cout << "The file could not be opened for writing." << std::endl;
		exit(1);
	}
	std::thread t(&Logger::insertLog, this);
	std::thread t1(&Logger::getInfoAboutMessage, this);
	t.join();
	t1.join();
}
Logger::~Logger()
{
	if (fs.is_open())
	{
		fs.close();
	}
}

void Logger::insertLog()
{
	m.lock();
	// std::this_thread::sleep_for(std::chrono::seconds(2));
	fs << input_string << "\n";
	std::cout << "Message successfully logged: \n";
	m.unlock();
}
void Logger::getInfoAboutMessage()
{
	m.lock_shared();
	std::ifstream inputFile(filename);
	std::string line;
	while (std::getline(inputFile, line))
	{
		std::cout << line << " " << "\n";
	}
	inputFile.close();
	m.unlock_shared();
}