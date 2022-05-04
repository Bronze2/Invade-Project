#pragma once
#include "pch.h"
#include "Server.h"
#include "Thread.h"

CServer::CServer()
{
}

CServer::~CServer()
{

}

void CServer::Init()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	//맨 뒤에 flag를 overlapped로 꼭 줘야 제대로 동작
	SHARED_DATA::listenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	//bind에 사용할 server address
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT); //host to network 해서 넣어야 한다
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //모든 클라로부터 접속을 받아야 한다
	::bind(SHARED_DATA::listenSocket, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)); //그냥 bind를 쓰면 c++11에 있는 키워드로 연결된다. 따라서 앞에 ::를 붙인다.


	listen(SHARED_DATA::listenSocket, SOMAXCONN);

	//IOCP 핸들 할당, IOCP객체 만들기
	SHARED_DATA::g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	//initialize_clients();
	

	//listen소켓 등록
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(SHARED_DATA::listenSocket), SHARED_DATA::g_iocp, 999, 0);
	//원래 accept는 소켓을 리턴하는데, 이 함수는 미리 소켓을 만들어 두고 그 소켓을 클라와 연결시켜준다. 동작이 좀 다름.
	//비동기로 acceptEx호출. 
	SOCKET clientSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	EXOVER accept_over;
	ZeroMemory(&accept_over, sizeof(accept_over.over));
	accept_over.op = OP_ACCEPT;
	accept_over.c_socket = clientSocket;
	AcceptEx(SHARED_DATA::listenSocket, clientSocket, accept_over.io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.over);
	
	
	CThread::GetInst()->Init();

}


void CServer::send_packet(int user_id, void* p)
{
	char* buf = reinterpret_cast<char*>(p);

	CLIENT& user = SHARED_DATA::g_clients[user_id];

	//WSASend의 두번째 인자의 over는 recv용이라 쓰면 안된다. 새로 만들어야 한다.
	EXOVER* exover = new EXOVER;
	exover->op = OP_SEND;
	ZeroMemory(&exover->over, sizeof(exover->over));
	exover->wsabuf.buf = exover->io_buf;
	exover->wsabuf.len = buf[0];
	memcpy(exover->io_buf, buf, buf[0]);

	WSASend(user.m_socket, &exover->wsabuf, 1, NULL, 0, &exover->over, NULL);
}
void CServer::send_my_client_enter_packet(int user_id)
{
	sc_packet_login_ok p;
	p.exp = 0;
	p.hp = 0;
	p.id = user_id;
	p.level = 0;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_OK;
	p.pos.x =SHARED_DATA::g_clients[user_id].Pos.x;
	p.pos.y =SHARED_DATA::g_clients[user_id].Pos.y;
	p.pos.z =SHARED_DATA::g_clients[user_id].Pos.z;

	send_packet(user_id, &p); //&p로 주지 않으면 복사되어서 날라가니까 성능에 안좋다. 
}
void CServer::send_lobby_login_ok_packet(int user_id)
{
	sc_packet_lobby_enter p;
	p.id = user_id;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_OK;
	p.camp = SHARED_DATA::g_clients[user_id].m_camp;
	p.isHost = SHARED_DATA::g_clients[user_id].m_isHost;

	send_packet(user_id, &p); //&p로 주지 않으면 복사되어서 날라가니까 성능에 안좋다. 
}
void CServer::send_login_fail_packet()
{

}
//아이디에게, 누가 이동했는지 알려줘라
void CServer::send_move_packet(int user_id, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_KEY_DOWN;
	p.pos.x = SHARED_DATA::g_clients[mover].Pos.x;
	p.pos.y = SHARED_DATA::g_clients[mover].Pos.y;
	p.pos.z = SHARED_DATA::g_clients[mover].Pos.z;
	p.state = SHARED_DATA::g_clients[mover].animState;
	p.move_time = SHARED_DATA::g_clients[mover].m_move_time;
	send_packet(user_id, &p); //&p로 주지 않으면 복사되어서 날라가니까 성능에 안좋다. 
}
void CServer::send_move_stop_packet(int user_id, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_KEY_UP;
	p.pos.x = SHARED_DATA::g_clients[mover].Pos.x;
	p.pos.y = SHARED_DATA::g_clients[mover].Pos.y;
	p.pos.z = SHARED_DATA::g_clients[mover].Pos.z;
	p.state = SHARED_DATA::g_clients[mover].animState;
	p.move_time = SHARED_DATA::g_clients[mover].m_move_time;
	send_packet(user_id, &p); //&p로 주지 않으면 복사되어서 날라가니까 성능에 안좋다. 
}
void CServer::send_mouse_packet(int user_id, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_MOUSE;
	p.pos.x = SHARED_DATA::g_clients[mover].Rot.x;
	p.pos.y = SHARED_DATA::g_clients[mover].Rot.y;
	p.pos.z = SHARED_DATA::g_clients[mover].Rot.z;

	p.move_time = SHARED_DATA::g_clients[mover].m_move_time;

	send_packet(user_id, &p); //&p로 주지 않으면 복사되어서 날라가니까 성능에 안좋다. 
}
void CServer::send_enter_packet(int user_id, int o_id)
{
	sc_packet_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_ENTER;
	p.pos.x = SHARED_DATA::g_clients[o_id].Pos.x;
	p.pos.y = SHARED_DATA::g_clients[o_id].Pos.y;
	p.pos.z = SHARED_DATA::g_clients[o_id].Pos.z;
	strcpy_s(p.name, SHARED_DATA::g_clients[o_id].m_name);
	p.o_type = O_PLAYER;

	SHARED_DATA::g_clients[user_id].m_cLock.lock();
	SHARED_DATA::g_clients[user_id].view_list.insert(o_id);
	SHARED_DATA::g_clients[user_id].m_cLock.unlock();

	send_packet(user_id, &p); //&p로 주지 않으면 복사되어서 날라가니까 성능에 안좋다. 
}
void CServer::send_enter_lobby_packet(int user_id, int o_id)
{
	sc_packet_lobby_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_LOBBY_ENTER;
	p.camp = SHARED_DATA::g_clients[o_id].m_camp;
	p.isHost = SHARED_DATA::g_clients[o_id].m_isHost;
	send_packet(user_id, &p);
}
void CServer::send_near_packet(int client, int new_id)
{
	// new_id가 들어왔고 이 new id의 정보를 client에게 보내는 것
	sc_packet_near packet;
	packet.id = new_id;
	packet.size = sizeof(packet);
	packet.type = S2C_NEAR_PLAYER;
	packet.pos.x = SHARED_DATA::g_clients[new_id].Pos.x;
	packet.pos.y = SHARED_DATA::g_clients[new_id].Pos.y;
	packet.pos.z = SHARED_DATA::g_clients[new_id].Pos.z;

	send_packet(client, &packet);
}
void CServer::send_leave_packet(int user_id, int o_id)
{
	sc_packet_move p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_LEAVE;

	SHARED_DATA::g_clients[user_id].m_cLock.lock();
	SHARED_DATA::g_clients[user_id].view_list.erase(o_id);
	SHARED_DATA::g_clients[user_id].m_cLock.unlock();

	send_packet(user_id, &p); //&p로 주지 않으면 복사되어서 날라가니까 성능에 안좋다. 
}
void CServer::send_spawn_minion_packet(int minion_id, float x, float y, float z, MINION_ATTACK_TYPE type,CAMP_STATE camp)
{
	sc_packet_spawn_minion packet;
	packet.size = sizeof(packet);
	packet.type = S2C_SPAWN_MINION;
	if(camp == CAMP_STATE::BLUE)
		packet.camp = BLUE;
	else if (camp == CAMP_STATE::RED)
		packet.camp = RED;
	packet.id = minion_id;
	p_Vec3 pos = { x,y,z };
	packet.pos = pos;
	std::cout << packet.id << endl;
	packet.mtype = (int)type;
	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		send_packet(i, &packet);

}

void CServer::send_move_minion_packet(int minion_id)
{
	sc_packet_move_minion packet;
	packet.size = sizeof(packet);
	packet.type = S2C_MOVE_MINION;
	packet.id = minion_id;
	packet.pos.x = SHARED_DATA::g_minion[minion_id].Pos.x;
	packet.pos.y = SHARED_DATA::g_minion[minion_id].Pos.y;
	packet.pos.z = SHARED_DATA::g_minion[minion_id].Pos.z;

	packet.rot.x = SHARED_DATA::g_minion[minion_id].Rot.x;
	packet.rot.y = SHARED_DATA::g_minion[minion_id].Rot.y;
	packet.rot.z = SHARED_DATA::g_minion[minion_id].Rot.z;
	packet.state = (int)SHARED_DATA::g_minion[minion_id].State;
	//cout << "["<<packet.state<<"]" << endl;
	//cout <<"id[" << packet.id <<"]:"<< packet.pos.x << "," << packet.pos.y << "," << packet.pos.z << endl;

	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		send_packet(i, &packet);

}

void CServer::send_anim_minion_packet(int minion_id)
{
	sc_packet_move_minion packet;
	packet.size = sizeof(packet);
	packet.type = S2C_ANIM_MINION;
	packet.id = minion_id;
	packet.pos.x = SHARED_DATA::g_minion[minion_id].Pos.x;
	packet.pos.y = SHARED_DATA::g_minion[minion_id].Pos.y;
	packet.pos.z = SHARED_DATA::g_minion[minion_id].Pos.z;

	packet.rot.x = SHARED_DATA::g_minion[minion_id].Rot.x;
	packet.rot.y = SHARED_DATA::g_minion[minion_id].Rot.y;
	packet.rot.z = SHARED_DATA::g_minion[minion_id].Rot.z;
	packet.state = (int)SHARED_DATA::g_minion[minion_id].State;

	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		send_packet(i, &packet);

}

void CServer::send_rot_tower_packet(int tower_id)
{
	sc_packet_rot_tower packet;
	packet.size = sizeof(packet);
	packet.type = S2C_ROT_TOWER;
	packet.id = tower_id;
	packet.rot.x = SHARED_DATA::g_tower[tower_id].x;
	packet.rot.y = SHARED_DATA::g_tower[tower_id].y;
	packet.rot.z = SHARED_DATA::g_tower[tower_id].z;

	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		send_packet(i, &packet);
}

void CServer::send_projectile_packet(int projectile_id, int type)
{
	sc_packet_projectile packet;
	packet.size = sizeof(packet);
	if(type == 0)
		packet.type = S2C_CREATE_PROJECTILE;
	else
		packet.type = S2C_PROJECTILE;

	packet.id = projectile_id;
	packet.pos.x = SHARED_DATA::g_bullet[projectile_id].x;
	packet.pos.y = SHARED_DATA::g_bullet[projectile_id].y;
	packet.pos.z = SHARED_DATA::g_bullet[projectile_id].z;

	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		send_packet(i, &packet);
}