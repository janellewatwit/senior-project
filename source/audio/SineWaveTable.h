#pragma once
#include <cstdint>
#include <cmath>

namespace SoundsMagic::AudioEngine
{
class SineWaveTable
{
public:
	// table containing sampled sine wave
	static float* m_table;
	// length of `m_table`
	uint32_t m_len;

	SineWaveTable();
	const float sample(uint32_t index);
};
}