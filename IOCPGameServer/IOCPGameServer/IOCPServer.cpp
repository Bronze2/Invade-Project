#pragma once
#include "pch.h"
#include <iostream>
#include "Server.h"
#include "Service.h"
//
namespace SHARED_DATA {
	HANDLE g_iocp;					//iocp 핸들
	CLIENT g_clients[MAX_USER];		//클라이언트 동접만큼 저장하는 컨테이너 필요
	unordered_map<int, MINION> g_minion;
	priority_queue<event_type> timer_queue;
	mutex timer_lock;
	SOCKET listenSocket;
	int g_minionindex;
	int current_user;
};



void main()
{
	std::cout << "Run Server" << std::endl;
	CServer::GetInst()->Init();
	CService::GetInst()->Init();

}