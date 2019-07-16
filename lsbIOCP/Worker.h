#pragma once

#include "Thread.h"

class Worker : public Thread
{
public:
	void Run() override;
};