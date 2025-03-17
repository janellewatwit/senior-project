#pragma once
#include <fstream>

class Logger
{
private:
	std::ofstream m_file;

public:
	Logger(const char* logfile);
	~Logger();
	std::ofstream& stream();
};
