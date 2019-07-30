#include <windows.h>
#include <time.h>

#include "Utils.h"

// 시간 처리
// https://araikuma.tistory.com/597
// https://m.blog.naver.com/PostView.nhn?blogId=igazoa2&logNo=220271308803&proxyReferer=https%3A%2F%2Fwww.google.com%2F
std::string utils::GetDate()
{
	char szdate[256] = { 0, };

	time_t now;
	time(&now);
	tm date;
	localtime_s(&date, &now);
	strftime(szdate, 256, "%Y_%m_%d", &date);

	return std::string(szdate);
}

std::string utils::GetTime()
{
	char sztime[256] = { 0, };
	_strtime_s(sztime, 256);

	return std::string(sztime);
}

// 가변인자
// https://dojang.io/mod/page/view.php?id=577
// https://m.blog.naver.com/PostView.nhn?blogId=sinarn&logNo=130180639399&proxyReferer=https%3A%2F%2Fwww.google.com%2F
// https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2008/d3xd30zz(v=vs.90)
// https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2008/kb57fad8(v=vs.90)
std::string utils::Format(const char* format, ...)
{
	char szbuff[512] = { 0, };

	va_list marker;
	va_start(marker, format);
	_vsnprintf_s(szbuff + strlen(szbuff), 512 - strlen(szbuff), _TRUNCATE, format, marker);
	va_end(marker);

	return std::string(szbuff);
}

// Bit converter
template<typename T>
T* utils::BytesToType(char* const pBytes, const int index)
{
	return reinterpret_cast<T*>(pBytes + index);
}

template<typename T>
char* utils::TypeToBytes(T* const pData)
{
	return reinterpret_cast<char*>(pData);
}