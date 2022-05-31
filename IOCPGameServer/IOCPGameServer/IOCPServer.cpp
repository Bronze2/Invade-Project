#pragma once
#include "pch.h"
#include <iostream>
#include "Server.h"
#include "Service.h"
#include "SceneMgr.h"
#include "DataBase.h"
//
namespace SHARED_DATA {
	HANDLE g_iocp;					
	//CLIENT g_clients[MAX_USER];		
	unordered_map <int, CLIENT> g_clients;
	
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
	std::cout << "Run DB" << std::endl;
	CDataBase::GetInst()->Init();
	std::cout << "Run Server" << std::endl;
	//CDataBase::GetInst()->CreateAdmin(L"extest", L"ext11");
	//CDataBase::GetInst()->DoubleCheckAdmin(L"extes1t");
	//CDataBase::GetInst()->CheckAdminLogin(L"sangjun", L"sjpw");



	CServer::GetInst()->Init();

	CService::GetInst()->Init();

}