#include "Log.h"

#pragma warning( push )
#pragma warning( disable : 4244 )
//http://cppconlib.codeplex.com/
//http://mariusbancila.ro/blog/2013/08/25/cppconlib-a-cpp-library-for-working-with-the-windows-console/
#include "conmanip.h"
#pragma warning( pop )

using namespace conmanip;

Log::Log()
{
	m_FileName = utils::GetDate();
	ChangeLogLevel(LOG_LEVEL::DEBUG);
}

void Log::Init(LOG_LEVEL logLevel, std::string& fileName)
{
	m_FileName = fileName;
	ChangeLogLevel(logLevel);
}

void Log::ChangeLogLevel(LOG_LEVEL logLevel)
{
	m_LogLevel.store(static_cast<int>(logLevel));
}

LOG_LEVEL Log::GetCurrentLogLevel()
{
	return static_cast<LOG_LEVEL>(m_LogLevel.load());
}

void Log::Write(const LOG_LEVEL logLevel, const char* pFormat, ...)
{
	if (m_LogLevel.load() < static_cast<int>(logLevel))
	{
		return;
	}

	char szText[MAX_LOG_STRING_LENGTH];
	va_list args;
	va_start(args, pFormat);
	vsprintf_s(szText, MAX_LOG_STRING_LENGTH, pFormat, args);
	va_end(args);

	Log::Flush(szText, logLevel);
}

void Log::Flush(const char* logMsg, LOG_LEVEL logLevel)
{
	std::string time = utils::GetTime();
	std::string date = utils::GetDate();

	std::stringstream ss;
	ss << "[" << date << "-" << time << "] " << logMsg;
	std::string Msg = ss.str();

	// TODO: split files if size exceeds threshold (not by date)
	// TODO: make dir if it dosen't exist
	ss.str(std::string());
	ss << "./Log/" << m_FileName << "_" << date << ".txt";
	std::string filePath = ss.str();

	// Is iofstream thread safe ?
	// https://stackoverflow.com/questions/20211935/is-ofstream-thread-safe
	std::lock_guard<std::mutex> lock(m_Lock);

	// Fail if there dosen't exist dir in path
	std::ofstream out(filePath, std::ios_base::out | std::ios_base::app);
	if (out.is_open()) 
	{ 
		out << Msg;
	}
	else
	{
		switch (logLevel)
		{
		case LOG_LEVEL::INFO:
			Info(Msg.c_str());
			break;
		case LOG_LEVEL::ERR:
			Error(Msg.c_str());
			break;
		case LOG_LEVEL::WARN:
			Warn(Msg.c_str());
			break;
		case LOG_LEVEL::DEBUG:
			Debug(Msg.c_str());
			break;
		case LOG_LEVEL::TRACE:
			Trace(Msg.c_str());
			break;
		default:
			break;
		}
	}
}

void Log::Error(const char* pText)
{
	std::cout << settextcolor(console_text_colors::red) << "[ERROR] | " << pText << std::endl;
}

void Log::Warn(const char* pText)
{
	std::cout << settextcolor(console_text_colors::yellow) << "[WARN] | " << pText << std::endl;
}

void Log::Debug(const char* pText)
{
	std::cout << settextcolor(console_text_colors::cyan) << "[DEBUG] | " << pText << std::endl;
}

void Log::Trace(const char* pText)
{
	std::cout << settextcolor(console_text_colors::green) << "[TRACE] | " << pText << std::endl;
}

void Log::Info(const char* pText)
{
	std::cout << settextcolor(console_text_colors::white) << "[INFO] | " << pText << std::endl;
}