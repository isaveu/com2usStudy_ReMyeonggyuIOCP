#pragma once

#include <windows.h>

#include <vector>
#include <string>
#include <stdexcept>

std::string FormatErrorMessage(DWORD error, const std::string& msg);
void ThrowLastErrorIf(bool expression, const std::string& msg);
void ThrowErrorIf(bool expression, DWORD error, const std::string& msg);

// windows 예외 처리
// https://docs.microsoft.com/ko-kr/cpp/cpp/how-to-interface-between-exceptional-and-non-exceptional-code?view=vs-2019
class AsyncIOException : public std::runtime_error
{
public:
	AsyncIOException(DWORD error, const std::string& msg)
		: std::runtime_error(FormatErrorMessage(error, msg)), m_error(error) {}
	DWORD GetErrorCode() const { return m_error; }

private:
	DWORD m_error;
};