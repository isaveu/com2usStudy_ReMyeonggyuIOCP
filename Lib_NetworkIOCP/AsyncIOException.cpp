#include "AsyncIOException.h"

// String formatter function (simillar with printf parameters)
std::string FormatErrorMessage(DWORD error, const std::string& msg)
{
	static const auto BUFFERLENGTH = 1024;
	std::vector<char> buf(BUFFERLENGTH);
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, error, 0, buf.data(), BUFFERLENGTH - 1, 0);
	return std::string(buf.data()) + " (" + msg + ")";
}

void ThrowErrorIf(bool expression, DWORD error, const std::string& msg)
{
	if (expression) {
		throw AsyncIOException(error, msg);
	}
}

void ThrowLastErrorIf(bool expression, const std::string& msg)
{
	if (expression) {
		throw AsyncIOException(GetLastError(), msg);
	}
}