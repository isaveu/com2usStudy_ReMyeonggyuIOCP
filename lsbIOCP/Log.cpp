#include "Log.h"

#define MAP_ITER		first
#define MUTEX_PTR		second
#define INSERT_SUCCESS	second

Log* Log::instance = nullptr;

Log* Log::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Log();
	}
	return instance;
}

Log::Log()
{
	m_FileName = utils::GetDate();
	ChangeLogLevel(LOG_LEVEL::INFO);
}

void Log::Init(LOG_LEVEL logLevel, std::string fileName)
{
	m_FileName = fileName;
	ChangeLogLevel(logLevel);

	// map insert tip
	// https://yonmy.com/archives/9
	// performance of string key map
	// http://veblush.blogspot.com/2012/10/map-vs-unorderedmap-for-string-key.html
	auto res = Log::m_Lock.insert({ fileName, nullptr });
	if (res.INSERT_SUCCESS)
	{ 
		res.MAP_ITER->MUTEX_PTR = new std::mutex();
	}
}

void Log::ChangeLogLevel(LOG_LEVEL logLevel)
{
	m_LogLevel.store((int)logLevel);
}

LOG_LEVEL Log::GetCurrentLogLevel()
{
	return (LOG_LEVEL)m_LogLevel.load();
}

/**
 * 1. 파일 이름별로 락을 걸어주는 방법 (날짜별로 파일명이 달라지므로 시간은 제외)
 * 2. concurrent_queue로 모든 로그를 저장하고,
 *	  해당 큐에서 로그메시지를 꺼내서 써주는 백그라운드 스레드를 이용하는 방법
 *		- 이것도.. 파일별로 큐를 만들어줄 수는 있음..
 *		- 하지만 백그라운드 작업에서 로그를 얼마나 자주 쓰게하고 얼마나 쉬게 해야 하는지 고민
 * 추가로.. 파일 I/O 전후로 락을 거는 것이 옳은 행동인가..
 */

// TODO: change parameter type to ref
void Log::Write(std::string msg) { Log::Write(msg, LOG_LEVEL::INFO); }
void Log::Write(std::string msg, LOG_LEVEL logLevel)
{
	if (m_LogLevel.load() < (int)logLevel) return;

	std::string time = utils::GetTime();
	std::string date = utils::GetDate();

	std::stringstream ss;
	ss << "[" << date << "-" << time << "] " << msg;
	std::string logMsg = ss.str();

	// TODO: split files if size exceeds threshold (not by date)
	// TODO: make dir if it dosen't exist
	ss.str(std::string());
	ss << "./Log/" << m_FileName << "_" << date << ".txt";
	std::string filePath = ss.str();

	Log::FlushToFile(m_FileName, filePath, logMsg);
}

void Log::FlushToFile(std::string& fileName, std::string& filePath, std::string& logMsg)
{
	// TODO: remove file lock, add fileopen, msg write lock
	// Manage mutex variables to map by file name (except for date in file name)
	std::mutex* ioMutex;
	auto res = m_Lock.insert({ fileName, nullptr });
	if (res.INSERT_SUCCESS)
	{
		res.MAP_ITER->MUTEX_PTR = new std::mutex();
		ioMutex = res.MAP_ITER->MUTEX_PTR;
	}
	else
	{ 
		ioMutex = res.MAP_ITER->MUTEX_PTR; 
	}

	// Is iofstream thread safe ?
	// https://stackoverflow.com/questions/20211935/is-ofstream-thread-safe
	std::lock_guard<std::mutex> lock(*ioMutex);

	// Fail if there dosen't exist dir in path
	std::ofstream out(filePath, std::ios_base::out | std::ios_base::app);
	if (out.is_open()) 
	{ 
		out << logMsg; 
	}
	else 
	{ 
		std::cout << logMsg << std::endl; 
	}
}