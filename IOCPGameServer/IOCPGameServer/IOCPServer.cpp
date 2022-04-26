#pragma once
#include "pch.h"
#include <iostream>
#include "Server.h"
#include "Service.h"
//
namespace SHARED_DATA {
	HANDLE g_iocp;					//iocp �ڵ�
	CLIENT g_clients[MAX_USER];		//Ŭ���̾�Ʈ ������ŭ �����ϴ� �����̳� �ʿ�
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