#pragma once
#include "global.h"

namespace SHARED_DATA {
	extern HANDLE g_iocp;					
	extern CLIENT g_clients[MAX_USER];
	extern unordered_map<int, MINION> g_minion;
	extern priority_queue<event_type> timer_queue;
	extern mutex timer_lock;
	extern SOCKET listenSocket;			//���� ��ü�� �ϳ�. �ѹ� �������� �ȹٲ�� �����ͷ��̽� �ƴ�. 
	extern int g_minionindex;
	extern int current_user;
}