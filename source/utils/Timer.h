#pragma once
#include <chrono>

namespace SoundsMagic::Util
{
class Timer
{
private:
	std::chrono::high_resolution_clock clock;
	std::chrono::steady_clock::time_point m_start;
	std::chrono::steady_clock::time_point m_end;

public:
	void start();
	void end();
	int64_t duration_ns();
	int64_t duration_ms();
};
}
