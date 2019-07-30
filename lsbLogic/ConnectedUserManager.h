#pragma once

#include <time.h>
#include <chrono>
#include <vector>

#include "..//lsbIOCP/Log.h"
#include "LogicMain.h"

namespace lsbLogic
{
	struct ConnectedUser
	{
		void Clear()
		{
			m_IsLoginSuccess = false;
			m_ConnectedTime = 0;
		}

		bool m_IsLoginSuccess = false;
		time_t m_ConnectedTime = 0;
	};

	class ConnectedUserManager
	{
	public:
		void Init(INetworkController* pController, const int maxSessionCount, bool IsLoginChk, Log* pLogger)
		{
			m_Log = pLogger;
			m_pController = pController;

			for (int i = 0; i < maxSessionCount; ++i)
			{
				ConnectedUserList.emplace_back(ConnectedUser());
			}

			m_IsLoginCheck = IsLoginChk;
		}

		void SetConnectSession(const int sessionIndex)
		{
			time(&ConnectedUserList[sessionIndex].m_ConnectedTime);
		}

		void SetLogin(const int sessionIndex)
		{
			ConnectedUserList[sessionIndex].m_IsLoginSuccess = true;
		}

		void SetDisConnectSession(const int sessionIndex)
		{
			ConnectedUserList[sessionIndex].Clear();
		}

		void SetLogout(const int sessionIndex)
		{
			ConnectedUserList[sessionIndex].m_IsLoginSuccess = false;
		}

		// TODO: 따로 스레드를 돌리되 처리할 내용이 있으면
		// 패킷 큐를 이용해서 패킷 처리 스레드에게 넘기기
		void LoginCheck()
		{
			if (m_IsLoginCheck == false) {
				return;
			}

			auto curTime = std::chrono::system_clock::now();
			auto diffTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - m_LatestLoginCheckTime);

			// 60밀리초 마다 검사
			if (diffTime.count() < 60)
			{
				return;
			}
			else
			{
				m_LatestLoginCheckTime = curTime;
			}

			auto curSecTime = std::chrono::system_clock::to_time_t(curTime);

			const auto maxSessionCount = (int)ConnectedUserList.size();

			if (m_LatestLogincheckIndex >= maxSessionCount) {
				m_LatestLogincheckIndex = -1;
			}

			++m_LatestLogincheckIndex;

			auto lastCheckIndex = m_LatestLogincheckIndex + 100;
			if (lastCheckIndex > maxSessionCount) {
				lastCheckIndex = maxSessionCount;
			}

			for (; m_LatestLogincheckIndex < lastCheckIndex; ++m_LatestLogincheckIndex)
			{
				auto i = m_LatestLogincheckIndex;

				if (ConnectedUserList[i].m_ConnectedTime == 0 ||
					ConnectedUserList[i].m_IsLoginSuccess == false)
				{
					continue;
				}

				auto diff = curSecTime - ConnectedUserList[i].m_ConnectedTime;
				if (diff >= 3600)
				{
					m_Log->Write(LOG_LEVEL::WARN, "%s | Login Wait Time Over. sessionIndex(%d).", __FUNCTION__, i);
					m_pController->DisconnectSocket(i);
				}
			}
		}

	private:
		ILog* m_Log;
		INetworkController* m_pController;

		std::vector<ConnectedUser> ConnectedUserList;

		bool m_IsLoginCheck = false;

		std::chrono::system_clock::time_point m_LatestLoginCheckTime = std::chrono::system_clock::now();
		int m_LatestLogincheckIndex = -1;
	};
}