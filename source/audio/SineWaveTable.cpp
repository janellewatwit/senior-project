#pragma once
#include "SineWaveTable.h"

SineWaveTable::SineWaveTable(const uint8_t lookup_bits) :
	m_lookup_bits(lookup_bits),
	m_len(1U << lookup_bits),
	m_table(new float[m_len])
{
	// populate sine wave table
	for (uint32_t i = 0; i < m_len; i++)
		m_table[i] = sin(2.0f * 3.1415927f * i / m_len);
}

SineWaveTable::~SineWaveTable()
{
	delete[] m_table;
}

const float SineWaveTable::sample(uint32_t index)
{
	index = index >> (sizeof(uint32_t) * 8U - m_lookup_bits);
	index &= (1U << m_lookup_bits) - 1U;
	return m_table[index];
}
