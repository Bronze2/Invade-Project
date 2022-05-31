#pragma once
#include "pch.h"
#include "Thread.h"
#include "Service.h"
#include "SceneMgr.h"
#include "EventMgr.h"
#include "TimeMgr.h"
#include "Server.h"

void CThread::Init()
{
	for (int i = 0; i < 10; ++i)
		worker_threads.emplace_back(std::thread([&]() {CThread::worker_Thread(); }));
	timer_thread = std::thread([&]() {CThread::do_timer(); });
	for (auto& thread : worker_threads)
		thread.join();
	timer_thread.join();
}


void CThread::packet_construct(int user_id, int io_byte)
{
	CLIENT& curr_user = SHARED_DATA::g_clients[user_id];
	EXOVER& recv_over = curr_user.m_recv_over;

	int rest_byte = io_byte;		//�̸�ŭ ������ ó������� �Ѵ�
	char* p = recv_over.io_buf;		//ó���ؾ��� �������� �����Ͱ� �ʿ��ϴ�
	int packet_size = 0;			//�̰� 0�̶�� ���� ������ ó���ϴ� ��Ŷ�� ���ٴ� �� 

	if (curr_user.m_prev_size != 0)
		packet_size = curr_user.m_packet_buf[0]; //�������� ��ٱ�� ��Ŷ ������

	while (rest_byte > 0)	//ó���ؾ��� �����Ͱ� ���������� ó���ؾ��Ѵ�.
	{
		if (0 == packet_size)	packet_size = p[0];

		//������ �����ͷ� ��Ŷ�� ���� �� �ֳ� ���� Ȯ��
		if (packet_size <= rest_byte + curr_user.m_prev_size)
		{
			memcpy(curr_user.m_packet_buf + curr_user.m_prev_size, p, packet_size - curr_user.m_prev_size);		//���� ó���� ������ ũ�⸸ŭ ��Ŷ ������� ���ֱ�

			p += packet_size - curr_user.m_prev_size;
			rest_byte -= packet_size - curr_user.m_prev_size;
			packet_size = 0;														//�� ��Ŷ�� �̹� ó���� �߰� ���� ��Ŷ ������� ��.

			process_packet(user_id, curr_user.m_packet_buf);

			curr_user.m_prev_size = 0;

		}
		else	//��Ŷ �ϳ��� ���� �� ���ٸ� ���ۿ� �����صΰ� �����Ϳ� ������ ����
		{
			memcpy(curr_user.m_packet_buf + curr_user.m_prev_size, p, rest_byte); //���� ������ ���� �޴µ�, �������� ���� �����Ͱ� �������� ��찡 ������, �� �ڿ� �޾ƾ��Ѵ�.
			curr_user.m_prev_size += rest_byte;
			rest_byte = 0;
			p += rest_byte;
		}
	}
}


void CThread::do_timer()
{
	while (true)
	{

		//CEventMgr::GetInst()->Clear();
		//CSceneMgr::GetInst()->Update();
		//CEventMgr::GetInst()->Update();

		//this_thread::sleep_for(1ms); //Sleep(1);
		while (true)
		{
			SHARED_DATA::timer_lock.lock();
			if (SHARED_DATA::timer_queue.empty() == true)
			{
				SHARED_DATA::timer_lock.unlock();
				break;
			}
			if (SHARED_DATA::timer_queue.top().wakeup_time > std::chrono::high_resolution_clock::now())
			{
				SHARED_DATA::timer_lock.unlock();
				break;
			}
			event_type ev = SHARED_DATA::timer_queue.top();
			SHARED_DATA::timer_queue.pop();
			SHARED_DATA::timer_lock.unlock();

			switch (ev.event_id)
			{
			case OP_MOVE:
			{
			}
			break;
			case OP_UPDATE:
			{
				EXOVER* over = new EXOVER();
				over->op = ev.event_id;;
				PostQueuedCompletionStatus(SHARED_DATA::g_iocp, 1, ev.obj_id, &over->over);
			}
			break;

			}
		}
	}
}

void CThread::worker_Thread()
{
	while (true) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(SHARED_DATA::g_iocp, &io_byte, &key, &over, INFINITE);

		EXOVER* exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);
		if (user_id == 999)
		{
			cout << "check" << endl;
		}
		
		CLIENT& cl = SHARED_DATA::g_clients[user_id]; //Ÿ���� ���̱� ����
		 
		switch (exover->op)
		{
		case OP_RECV:			//���� ��Ŷ ó�� -> overlapped����ü �ʱ�ȭ -> recv
		{
			if (0 == io_byte)
			{
				CService::GetInst()->disconnect(user_id);
				if (OP_SEND == exover->op)
					delete exover;
			}
			else
			{
				packet_construct(user_id, io_byte);
				ZeroMemory(&cl.m_recv_over.over, 0, sizeof(cl.m_recv_over.over));
				DWORD flags = 0;
				WSARecv(cl.m_socket, &cl.m_recv_over.wsabuf, 1, NULL, &flags, &cl.m_recv_over.over, NULL);;
			}
			break;
		}
		case OP_SEND:			//����ü delete
			if (0 == io_byte)
				CService::GetInst()->disconnect(user_id);

			delete exover;
			break;

		case OP_ACCEPT:			//CreateIoCompletionPort���� Ŭ����� iocp�� ��� -> �ʱ�ȭ -> recv -> accept �ٽ�(��������)
		{
			int user_id = -1;
			for (int i = 0; i < MAX_USER; ++i)
			{
				lock_guard<mutex> gl{ SHARED_DATA::g_clients[i].m_cLock }; //�̷��� �ϸ� unlock�� �ʿ� ����. �� ��Ͽ��� ���������� unlock�� �ڵ����� ���ش�.
				if (ST_FREE == SHARED_DATA::g_clients[i].m_status)
				{
					SHARED_DATA::g_clients[i].m_status = ST_ALLOCATED;
					user_id = i;
					break;
				}
			}

			//for (auto& cl : SHARED_DATA::g_clients) {
			//	lock_guard<mutex> gl{ cl.second.m_cLock };
			//	if (ST_FREE == cl.second.m_status)
			//	{
			//		cl.second.m_status = ST_ALLOCATED;
			//		user_id = cl.second;
			//		break;
			//	}
			//}
			//main���� ������ worker������� �Űܿ��� ���� listen������ ����������, client������ ����� �����Դ�.
			SOCKET clientSocket = exover->c_socket;

			if (-1 == user_id)
				closesocket(clientSocket); // send_login_fail_packet();
			else
			{
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), SHARED_DATA::g_iocp, user_id, 0);

				//g_clients[user_id].m_id = user_id; �־����� �ϴ°� �ƴϰ� �ʱ�ȭ �Ҷ� �ѹ� ����� �� ó���� �ѹ�.
				SHARED_DATA::g_clients[user_id].m_prev_size = 0; //������ �޾Ƶ� ������ ������ 0
				SHARED_DATA::g_clients[user_id].m_socket = clientSocket;

				ZeroMemory(&SHARED_DATA::g_clients[user_id].m_recv_over.over, 0, sizeof(SHARED_DATA::g_clients[user_id].m_recv_over.over));
				SHARED_DATA::g_clients[user_id].m_recv_over.op = OP_RECV;
				SHARED_DATA::g_clients[user_id].m_recv_over.wsabuf.buf = SHARED_DATA::g_clients[user_id].m_recv_over.io_buf;
				SHARED_DATA::g_clients[user_id].m_recv_over.wsabuf.len = MAX_BUF_SIZE;

				SHARED_DATA::g_clients[user_id].m_id = user_id;
				if (user_id == 0)
					SHARED_DATA::g_clients[user_id].m_isHost = true;

				if (user_id % 2 == 0) {
					SHARED_DATA::g_clients[user_id].m_camp = RED;
					SHARED_DATA::g_clients[user_id].Pos = Vec3(0,0,1125);

				}
				else {
					SHARED_DATA::g_clients[user_id].Pos = Vec3(0, 0, 5800);
					SHARED_DATA::g_clients[user_id].m_camp = BLUE;
				}
				//SHARED_DATA::g_clients[user_id].view_list.clear();

				DWORD flags = 0;
				WSARecv(clientSocket, &SHARED_DATA::g_clients[user_id].m_recv_over.wsabuf, 1, NULL, &flags, &SHARED_DATA::g_clients[user_id].m_recv_over.over, NULL);
				SHARED_DATA::current_user++;
				cout << " USERINDEX" << endl;
				SHARED_DATA::g_clients.erase(999);
				for (auto& cl : SHARED_DATA::g_clients)
				{
					cout << "["<<cl.first<< "]--";
				}
				cout << endl;
			}

			//���� �ʱ�ȭ �� �ٽ� accept
			clientSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			exover->c_socket = clientSocket; //���� �޴� ������ �־��ش�. �ȱ׷��� Ŭ����� ���� ������ �����Ѵ�.
			ZeroMemory(&exover->over, sizeof(exover->over)); //accept_over�� exover��� �̸����� �޾����� exover�� ����
			AcceptEx(SHARED_DATA::listenSocket, clientSocket, exover->io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
			cout << "Client Accept Clear" << endl;
		}
		break;
		case OP_MOVE:
		{

		}
		break;

		case OP_UPDATE:
		{		
			CTimeMgr::GetInst()->Update();
			CEventMgr::GetInst()->Clear();
			CSceneMgr::GetInst()->Update();
			CEventMgr::GetInst()->Update();
			//CService::GetInst()->update_minion();

			CService::GetInst()->add_timer(user_id, OP_UPDATE, 33);	//30fps
		}
		break;

		}
	}

}


#include "Database.h";
void CThread::process_packet(int user_id, char* buf)
{
	switch (buf[1]) //[0]�� size
	{
	case C2S_LOGIN:
	{
		sc_packet_check_login* packet = reinterpret_cast<sc_packet_check_login*>(buf);
		cout << "Recv Login Packet Client " << endl;
		wstring t_userid;
		wstring t_userpw;
		t_userid.assign(packet->loginid.begin(), packet->loginid.end());
		t_userpw.assign(packet->loginpw.begin(), packet->loginpw.end());

		if (CDataBase::GetInst()->CheckAdminLogin(t_userid, t_userpw))
		{
			CService::GetInst()->enter_lobby(user_id);
		}
		else {
			CServer::GetInst()->send_login_fail_packet();
		}
		
	}
	break;
	case C2S_KEY_DOWN:
	{	cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);
		SHARED_DATA::g_clients[user_id].m_move_time = packet->move_time;
		SHARED_DATA::g_clients[user_id].dir.x = packet->dir.x;
		SHARED_DATA::g_clients[user_id].dir.y = packet->dir.y;
		SHARED_DATA::g_clients[user_id].dir.z = packet->dir.z;
		SHARED_DATA::g_clients[user_id].animState = packet->state;
		CService::GetInst()->do_move(user_id, packet->direction);
	}
	break;
	case C2S_KEY_UP:
	{	cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);
		SHARED_DATA::g_clients[user_id].m_move_time = packet->move_time;
		SHARED_DATA::g_clients[user_id].dir.x = packet->dir.x;
		SHARED_DATA::g_clients[user_id].dir.y = packet->dir.y;
		SHARED_DATA::g_clients[user_id].dir.z = packet->dir.z;
		SHARED_DATA::g_clients[user_id].animState = packet->state;
		CService::GetInst()->do_move_stop(user_id, packet->direction);
	}
	break;
	case C2S_MOUSE:
	{
		cs_packet_mouse* packet = reinterpret_cast<cs_packet_mouse*>(buf);
		SHARED_DATA::g_clients[user_id].m_move_time = packet->move_time;
		SHARED_DATA::g_clients[user_id].Rot.x = packet->Rot.x;
		SHARED_DATA::g_clients[user_id].Rot.y = packet->Rot.y;
		SHARED_DATA::g_clients[user_id].Rot.z = packet->Rot.z;

		CService::GetInst()->do_Rotation(user_id);
	}
	break;
	case C2S_GAMESTART:
	{
		cout << "C2S_GAMESTRAT" << endl;
		cs_packet_lobby_gamestart* packet = reinterpret_cast<cs_packet_lobby_gamestart*>(buf);
		if (packet->id == user_id) {
			if (SHARED_DATA::g_clients[user_id].m_isHost) {
				SHARED_DATA::g_clients[0].Pos = Vec3(0, 0, 1125);
				SHARED_DATA::g_clients[1].Pos = Vec3(0, 0, 5800);

				for (auto& cl : SHARED_DATA::g_clients) {
					CService::GetInst()->enter_game(cl.second.m_id);
				}
				//for (int i = 0; i < SHARED_DATA::current_user; ++i) {
				//	CService::GetInst()->enter_game(i);
				//}
				
				//�÷��̾� ���� �� �̴Ͼ� ���� ����
				CSceneMgr::GetInst()->Init();
				CService::GetInst()->add_timer(user_id, OP_UPDATE, 10);
			}

		}

	}
	case C2S_ATTACK_READY:
	{	
		cs_packet_attack_ready* packet = reinterpret_cast<cs_packet_attack_ready*>(buf);
		CServer::GetInst()->send_update_animation(packet->id, packet->state);
	}
	break;
	case C2S_CREATE_ARROW:
	{
		cs_packet_arrow* packet = reinterpret_cast<cs_packet_arrow*>(buf);
		CSceneMgr::GetInst()->InitArrowByPlayerId(packet->Clinet_id, packet->Arrow_id,
			Vec3(packet->Pos.x, packet->Pos.y, packet->Pos.z), Vec3(packet->Rot.x, packet->Rot.y, packet->Rot.z),
			Vec3(packet->Dir.x, packet->Dir.y, packet->Dir.z), packet->Power, CAMP_STATE::BLUE);
	}
	break;
	case C2S_MOVE_BLOCK:
	{
		cs_packet_move_block* packet = reinterpret_cast<cs_packet_move_block*>(buf);
		SHARED_DATA::g_clients[packet->id].Pos.x = packet->Pos.x;
		SHARED_DATA::g_clients[packet->id].Pos.y = 0;
		SHARED_DATA::g_clients[packet->id].Pos.z = packet->Pos.z;
	}
	break;
	default:
		cout << "unknown packet type error \n";


		//DebugBreak(); 
		//exit(-1);
	}
}


CThread::CThread() {};
CThread::~CThread() {};