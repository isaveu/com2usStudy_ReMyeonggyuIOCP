#pragma once

#include <windows.h>
#include <time.h>
#include <string>

namespace utils {
	std::string GetDate();

	std::string GetTime();

	std::string Format(const char* format, ...);
}