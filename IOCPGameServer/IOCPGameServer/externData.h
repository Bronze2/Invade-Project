#pragma once
#include "global.h"

namespace SHARED_DATA {
	extern HANDLE g_iocp;					
	extern CLIENT g_clients[MAX_USER];
	extern unordered_map<int, MINION> g_minion;
	extern priority_queue<event_type> timer_queue;
	extern mutex timer_lock;
	extern SOCKET listenSocket;			//서버 전체에 하나. 한번 정해지고 안바뀌니 데이터레이스 아님. 
	extern int g_minionindex;
	extern int current_user;
}