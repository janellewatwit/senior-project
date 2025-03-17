#pragma once
#include <chrono>

class Timer
{
private:
	std::chrono::high_resolution_clock clock;
	std::chrono::steady_clock::time_point m_start;
	std::chrono::steady_clock::time_point m_end;

public:
	void start();
	void end();
	uint32_t duration_ns();
	uint32_t duration_ms();
};
