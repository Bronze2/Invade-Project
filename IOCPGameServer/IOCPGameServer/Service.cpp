#pragma once
#include "pch.h"
#include "Service.h"
#include "Server.h"
void CService::Init()
{

}
void CService::add_timer(int obj_id, ENUMOP op_type, int duration)
{
	SHARED_DATA::timer_lock.lock();
	event_type ev{ obj_id, op_type,std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(duration), 0 };
	SHARED_DATA::timer_queue.emplace(ev);
	SHARED_DATA::timer_lock.unlock();
}

void CService::update_minion()
{
	for (int i = 0; i < 10; ++i) {


			SHARED_DATA::g_minion[i].m_cLock.lock();
			CServer::GetInst()->send_move_minion_packet(i);
			SHARED_DATA::g_minion[i].m_cLock.unlock();

	}
	//CServer::GetInst()->send_move_minion_packet(3);
}
void CService::update_minionindex(int minion_id)
{
	//	CServer::GetInst()->send_move_minion_packet(i);
	//CServer::GetInst()->send_move_minion_packet(0);
}
void CService::update_arrow()
{

}


bool CService::is_near(int a, int b)
{

	return true;
}

void CService::do_move(int user_id, int direction)
{
	SHARED_DATA::g_clients[user_id].Pos += SHARED_DATA::g_clients[user_id].dir * 30.f;
	SHARED_DATA::g_clients[user_id].Pos.y = 0;
	//CServer::GetInst()->send_move_packet(user_id, user_id);
	for (int i = 0; i < SHARED_DATA::current_user; ++i) {
		//if (i == user_id) continue;
		//else
			CServer::GetInst()->send_move_packet(i, user_id);
	}
}

void CService::do_move_stop(int user_id, int direction)
{
	Vec3 vPos;
	CServer::GetInst()->send_move_stop_packet(user_id, user_id);
	for (int i = 0; i < SHARED_DATA::current_user; ++i) {
		if (i == user_id) continue;
		else
			CServer::GetInst()->send_move_stop_packet(i, user_id);
	}
}

void CService::enter_game(int user_id)
{
	//g_clients[user_id].m_cLock.lock();
	CServer::GetInst()->send_enter_packet(user_id, user_id);
	//g_clients[user_id].m_cLock.unlock();


	for (int i = 0; i < SHARED_DATA::current_user; ++i) {
		if (i != user_id)
			CServer::GetInst()->send_enter_packet(i, user_id);
	}
}

void CService::enter_lobby(int user_id, char name[])
{
	SHARED_DATA::g_clients[user_id].m_cLock.lock();
	CServer::GetInst()->send_lobby_login_ok_packet(user_id);
	SHARED_DATA::g_clients[user_id].m_status = ST_ACTIVE;
	SHARED_DATA::g_clients[user_id].m_cLock.unlock();

	for (int i = 0; i < user_id + 1; i++)
	{
		if (user_id == i) continue;

		else if (i != user_id)		//나에게는 보내지 않는다.
		{
			cout << "Enter Other Client" << endl;

			CServer::GetInst()->send_enter_lobby_packet(user_id, i);
			CServer::GetInst()->send_enter_lobby_packet(i, user_id); //니가 나를 보면 나도 너를 본다
		}
	}
	cout << "Enter	" << endl;
}

void CService::do_Rotation(int user_id)
{
	CServer::GetInst()->send_mouse_packet(user_id, user_id);
	for (int i = 0; i < SHARED_DATA::current_user; ++i) {
		if (i == user_id) continue;
		else
			CServer::GetInst()->send_mouse_packet(i, user_id);
	}

}

void CService::initialize_clients()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		//이건 멀티쓰레드로 돌기 전에, 싱글쓰레드로 돌아가는 함수여서 lock을 거는 의미가 없음. 
		SHARED_DATA::g_clients[i].m_status = ST_FREE;
		SHARED_DATA::g_clients[i].m_id = i;
	}
}


void CService::disconnect(int user_id)
{
	CServer::GetInst()->send_leave_packet(user_id, user_id); //나는 나에게 보내기

	SHARED_DATA::g_clients[user_id].m_cLock.lock();
	SHARED_DATA::g_clients[user_id].m_status = ST_ALLOCATED;	//처리 되기 전에 FREE하면 아직 떠나는 뒷처리가 안됐는데 새 접속을 받을 수 있음

	closesocket(SHARED_DATA::g_clients[user_id].m_socket);

	for (auto& cl : SHARED_DATA::g_clients)
	{
		if (cl.m_id == user_id) continue;
		//cl.m_cLock.lock();
		if (ST_ACTIVE == cl.m_status)
			CServer::GetInst()->send_leave_packet(cl.m_id, user_id);
		//cl.m_cLock.unlock();
	}
	SHARED_DATA::g_clients[user_id].m_status = ST_FREE;	//다 처리했으면 FREE
	SHARED_DATA::g_clients[user_id].m_cLock.unlock();
}

CService::CService() {};
CService::~CService() {};