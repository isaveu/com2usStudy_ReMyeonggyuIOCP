#pragma once

#include <atomic>
#include <string>
#include <mutex>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Utils.h"

enum LOG_LEVEL { DISABLE, INFO, WARN, ERR, TRACE, DEBUG };

class Log
{
public:
	static Log* GetInstance();

	void Init(LOG_LEVEL logLevel, std::string fileName);

	void ChangeLogLevel(LOG_LEVEL logLevel);

	LOG_LEVEL GetCurrentLogLevel();

	void Write(std::string msg);
	void Write(std::string msg, LOG_LEVEL logLevel);

private:
	void FlushToFile(std::string& fileName, std::string& filePath, std::string& logMsg);

private:
	// typedef vs using
	// https://unikys.tistory.com/381
	using mapStrMutex = std::map<std::string, std::mutex*>;
	mapStrMutex			m_Lock;
	std::atomic<int>	m_LogLevel;
	std::string			m_FileName;

	// static member
	// https://is03.tistory.com/18
	static Log* instance;

private:
	Log();
};