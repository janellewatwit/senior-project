#pragma once
#include <cstdint>
#include <cmath>

class SineWaveTable
{
public:
	// table containing sampled sine wave
	float* const m_table;
	// bits used to index `m_table`
	const uint8_t m_lookup_bits;
	// length of `m_table`
	uint32_t m_len;

	SineWaveTable(const uint8_t lookup_bits);
	~SineWaveTable();
	const float sample(uint32_t index);
};
