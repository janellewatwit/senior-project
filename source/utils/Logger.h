#pragma once
#include <fstream>

namespace SoundsMagic::Util
{
class Logger
{
private:
	std::ofstream m_file;

public:
	Logger(const char* logfile);
	~Logger();
	std::ofstream& stream();
};
}
