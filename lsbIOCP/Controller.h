#pragma once

#include "IServer.h"

class Controller : public IServerController
{
public:
	DWORD postRecv() override;
	DWORD postSend() override;
	DWORD disconnectSocket() override;
	virtual DWORD connectSocket() override;
	virtual DWORD registerSokcet() override;
};