#pragma once

#include <string>

namespace utils {
	std::string GetDate();

	std::string GetTime();

	std::string Format(const char* format, ...);

	// Bit converter
	template<typename T>
	T* BytesToType(char* const pBytes, const int index);

	template<typename T>
	char* TypeToBytes(T* const pData);
}