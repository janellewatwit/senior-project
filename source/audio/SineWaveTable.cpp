#pragma once
#include "SineWaveTable.h"
#include "constants.h"

namespace SoundsMagic::AudioEngine
{
float* SineWaveTable::m_table = nullptr;

SineWaveTable::SineWaveTable() : m_len(1U << WAVETABLE_LOOKUP_BITS)
{
	if (m_table == nullptr)
	{
		// populate sine wave table
		m_table = new float[m_len];
		for (uint32_t i = 0; i < m_len; i++)
			m_table[i] = sin(2.0f * 3.1415927f * i / m_len);
	}
}

const float SineWaveTable::sample(uint32_t index)
{
	index = index >> (sizeof(uint32_t) * 8U - WAVETABLE_LOOKUP_BITS);
	index &= (1U << WAVETABLE_LOOKUP_BITS) - 1U;
	return m_table[index];
}
}
