#pragma once

#include <windows.h>

enum OpType { WRITE, READ, CONNECT };

typedef struct _OVERLAPPED_EX
{
	OVERLAPPED		overlapped;
	int				optype;

	size_t			length;			// Length of the actual data to send
	size_t			maxLength;		// socket buffer max length
	char*			datas;			// Used to hold data to send, direct new/delete or refer from pool

	INT32			requesterID;	// [요청자가 건네는 구분값] 현재 connectSocket을 위해 사용될 값. 그외 다양한 목적으로 사용할 수 있음

	_OVERLAPPED_EX() : overlapped(), length(0), optype(OpType::WRITE), requesterID(0), maxLength(0), datas(NULL) {}

	_OVERLAPPED_EX(size_t maxLength) : overlapped(), length(0), optype(OpType::WRITE), requesterID(0), maxLength(maxLength), datas(NULL)
	{
		if (0 < maxLength)
			datas = new char[maxLength];
	}

	~_OVERLAPPED_EX()
	{
		if (NULL != datas)
			delete[] datas;
	}

	void initHeader()
	{
		ZeroMemory(&overlapped, sizeof(overlapped));
		length = 0;
		optype = OpType::WRITE;
		requesterID = 0;
	}
} OVERLAPPED_EX;
