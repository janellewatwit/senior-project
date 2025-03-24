#include "Logger.h"

namespace SoundsMagic::Util
{
Logger::Logger(const char* logfile)
{
	m_file.open(logfile);
}

Logger::~Logger()
{
	m_file.close();
}

std::ofstream& Logger::stream()
{
	return m_file;
}
}
