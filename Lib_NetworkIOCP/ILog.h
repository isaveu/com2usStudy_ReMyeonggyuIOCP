#pragma once

#include <stdio.h>
#include <stdarg.h>

#pragma warning (disable: 4100)

const int MAX_LOG_STRING_LENGTH = 256;

enum LOG_LEVEL : short
{
	DISABLE = 0,
	ERR,
	WARN,
	INFO,
	TRACE,
	DEBUG,
};

class ILog
{
public:
	ILog() {}
	virtual ~ILog() {}

	virtual void Write(const LOG_LEVEL nType, const char* pFormat, ...)
	{
		char szText[MAX_LOG_STRING_LENGTH];

		va_list args;
		va_start(args, pFormat);
		vsprintf_s(szText, MAX_LOG_STRING_LENGTH, pFormat, args);
		va_end(args);

		switch (nType)
		{
		case LOG_LEVEL::INFO:
			Info(szText);
			break;
		case LOG_LEVEL::ERR:
			Error(szText);
			break;
		case LOG_LEVEL::WARN:
			Warn(szText);
			break;
		case LOG_LEVEL::DEBUG:
			Debug(szText);
			break;
		case LOG_LEVEL::TRACE:
			Info(szText);
			break;
		default:
			break;
		}
	}

protected:
	virtual void Error(const char* pText) = 0;
	virtual void Warn(const char* pText) = 0;
	virtual void Debug(const char* pText) = 0;
	virtual void Trace(const char* pText) = 0;
	virtual void Info(const char* pText) = 0;
};