#include "Timer.h"

namespace SoundsMagic::Util
{
void Timer::start()
{
	m_start = clock.now();
}

void Timer::end()
{
	m_end = clock.now();
}

int64_t Timer::duration_ns()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start).count();
}

int64_t Timer::duration_ms()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count();
}
}
