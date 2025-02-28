#pragma once
#include <stdint.h>
#include <string>

const char16_t* stoc16(std::string& str)
{
	char16_t* out = new char16_t[str.length()+1];
	for (uint32_t i = 0; i < str.length(); i++)
	{
		out[i] = (char16_t)str[i];
	}
	out[str.length()] = u'\0';
	return out;
}
