#pragma once
#include "pch.h"
#include <iostream>
#include "Server.h"
#include "Service.h"
#include "SceneMgr.h"
//
namespace SHARED_DATA {
	HANDLE g_iocp;					
	CLIENT g_clients[MAX_USER];		
	unordered_map<int, MINION> g_minion;
	priority_queue<event_type> timer_queue;
	mutex timer_lock;
	SOCKET listenSocket;
	int g_minionindex;
	int current_user;
	map <int, Vec3> g_tower;
	unordered_map<int, Vec3> g_bullet;
	int g_bulletindex;
};



void main()
{
	std::cout << "Run Server" << std::endl;
	//CSceneMgr::GetInst()->Init();
	CServer::GetInst()->Init();
	CService::GetInst()->Init();

}