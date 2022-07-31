#pragma once
#include "pch.h"
#include "Server.h"
#include "Thread.h"
#include "MatchMaking.h"

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

	send_packet(user_id, &p); 
}

void CServer::send_current_room(int user_id, int room_id, int current_user, int max_user , string RoomName)
{
	sc_packet_current_room p;
	p.size = sizeof(p);
	p.type = S2C_CURRENT_ROOM;
	p.room_id = room_id;
	p.current_user = current_user;
	p.max_user = max_user;
	strcpy(p.roomName, RoomName.c_str());
	send_packet(user_id, &p);
}

void CServer::send_lobby_login_ok_packet(int user_id)
{
	sc_packet_lobby_enter p;
	p.id = user_id;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_OK;

	send_packet(user_id, &p); //&p로 주지 않으면 복사되어서 날라가니까 성능에 안좋다. 
}
void CServer::send_login_fail_packet()
{
	sc_packet_login_false packet;
	packet.size = sizeof(packet);
	packet.type = S2C_LOGIN_FALSE;
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
	sc_packet_mouse p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_MOUSE;
	p.Rot.x = SHARED_DATA::g_clients[mover].Rot.x;
	p.Rot.y = SHARED_DATA::g_clients[mover].Rot.y;
	p.Rot.z = SHARED_DATA::g_clients[mover].Rot.z;

	p.move_time = SHARED_DATA::g_clients[mover].m_move_time;

	send_packet(user_id, &p); //&p로 주지 않으면 복사되어서 날라가니까 성능에 안좋다. 
}
void CServer::send_enter_packet(int user_id, int o_id)
{
	sc_packet_enter p;
	p.id = SHARED_DATA::g_clients[o_id].m_id;
	p.size = sizeof(p);
	p.type = S2C_ENTER;
	p.pos.x = SHARED_DATA::g_clients[o_id].Pos.x;
	p.pos.y = SHARED_DATA::g_clients[o_id].Pos.y;
	p.pos.z = SHARED_DATA::g_clients[o_id].Pos.z;
	p.camp = SHARED_DATA::g_clients[o_id].m_camp;
	p.skill = SHARED_DATA::g_clients[o_id].skill;
	strcpy_s(p.name, SHARED_DATA::g_clients[o_id].m_name);
	p.o_type = O_PLAYER;

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
		packet.camp = CAMP_STATE::BLUE;
	else if (camp == CAMP_STATE::RED)
		packet.camp = CAMP_STATE::RED;
	packet.id = minion_id;
	p_Vec3 pos = { x,y,z };
	packet.pos = pos;
	std::cout << packet.id << endl;
	packet.mtype = (int)type;
	
	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		send_packet(i, &packet);

}

void CServer::send_move_minion_packet(int minion_id, int room_id)
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


	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == room_id)
			send_packet(cl.second.m_id, &packet);

	}

}

void CServer::send_anim_minion_packet(int minion_id, int room_id)
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


	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == room_id)
			send_packet(cl.second.m_id, &packet);

	}
	//for (int i = 0; i < SHARED_DATA::current_user; ++i)
	//	send_packet(i, &packet);

}

void CServer::send_delete_minion(int minion_id)
{
	sc_packet_delete_minion packet;
	packet.size = sizeof(packet);
	packet.type = S2C_DELETE_MINION;
	packet.id = minion_id;
	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		send_packet(i, &packet);

}


void CServer::send_delete_tower(int tower_id)
{
	sc_packet_delete_tower packet;
	packet.size = sizeof(packet);
	packet.type = S2C_DELETE_TOWER;
	packet.id = tower_id;
	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		send_packet(i, &packet);

}

void CServer::send_damage_minion(int minion_id , int hp ,CAMP_STATE camp)
{
	sc_packet_damage_minion packet;
	packet.size = sizeof(packet);
	packet.type = S2C_DAMAGE_MINION;
	packet.minion_id = minion_id;
	packet.current_hp = hp;
	packet.camp = camp;

	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		send_packet(i, &packet);

}

void CServer::send_damage_tower(int tower_id, int  hp, CAMP_STATE camp)
{
	sc_packet_damage_tower packet;
	packet.size = sizeof(packet);
	packet.type = S2C_DAMAGE_TOWER;
	packet.tower_id = tower_id;
	packet.current_hp = hp;
	packet.camp = camp;

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
	else if(type ==1)
		packet.type = S2C_PROJECTILE;
	else if (type == 2) {
		packet.type = S2C_DELETE_PROJECTILE;
	}

	packet.id = projectile_id;
	packet.pos.x = SHARED_DATA::g_bullet[projectile_id].x;
	packet.pos.y = SHARED_DATA::g_bullet[projectile_id].y;
	packet.pos.z = SHARED_DATA::g_bullet[projectile_id].z;

	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		send_packet(i, &packet);
}

//Projectile 추가
void CServer::send_damage_player(int player_id, int hp)
{
	sc_damage_player packet;
	packet.size = sizeof(packet);
	packet.type = S2C_DAMAGE_PLAYER;
	packet.player_id = player_id;
	packet.hp = hp;

	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == SHARED_DATA::g_clients[player_id].room_id) {
			send_packet(cl.second.m_id, &packet);
		}
	}

}

void CServer::send_create_arrow_packet(int client_id, int arrow_id, Vec3 Pos, Vec3 Rot, PACKET_SKILL skill)
{
	sc_packet_arrow packet;
	packet.size = sizeof(packet);
	packet.type = S2C_CREATE_ARROW;
	packet.Clinetid = client_id;
	packet.Arrowid = arrow_id;
	packet.Pos.x = Pos.x;
	packet.Pos.y = Pos.y;
	packet.Pos.z = Pos.z;

	packet.Rot.x = Rot.x;
	packet.Rot.y = Rot.y;
	packet.Rot.z = Rot.z;
	packet.skill = skill;

	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == SHARED_DATA::g_clients[client_id].room_id) {
			if (cl.second.m_id == client_id) 
				continue;
			else
				send_packet(cl.second.m_id, &packet);
		}

	}

	//for (int i = 0; i < SHARED_DATA::current_user; ++i)
	//	if (i == client_id) continue;
	//	else send_packet(i, &packet);
}

void CServer::send_update_arrow(int client_id, int arrow_id, p_Vec3 LocalPos, p_Vec4 Quaternion)
{
	sc_packet_update_arrow_move packet;
	packet.size = sizeof(packet);
	packet.id = client_id;
	packet.arrow_id = arrow_id;
	packet.type = S2C_UPDATE_ARROW;
	packet.LocalPos = LocalPos;
	packet.Quaternion = Quaternion;


	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.m_id == client_id) continue;
		else if (cl.second.room_id == SHARED_DATA::g_clients[client_id].room_id) {
			send_packet(cl.second.m_id, &packet);
		}
	}
}

void CServer::send_collision_arrow(int client_id, int arrow_id, int coll_id, PACKET_COLLTYPE coll_type)
{
	sc_packet_collsion_arrow packet;
	packet.size = sizeof(packet);
	packet.type = S2C_COLLISION_ARROW;
	packet.id = client_id;
	packet.arrow_id = arrow_id;
	packet.coll_id = coll_id;
	packet.coll_type = coll_type;


	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.m_id == client_id) continue;
		else if (cl.second.room_id == SHARED_DATA::g_clients[client_id].room_id) {
			send_packet(cl.second.m_id, &packet);
		}
	}
}

void CServer::send_chat_msg(int client_id, char msg[100])
{
	sc_chat_msg packet;
	packet.size = sizeof(packet);
	packet.type = S2C_CHAT_MSG;
	strcpy_s(packet.msg, msg);
	cout << packet.msg << endl;

	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == SHARED_DATA::g_clients[client_id].room_id) {
			send_packet(cl.second.m_id, &packet);
		}
	}
}



void CServer::send_setDamage(int client_id, int coll_id, int damage, PACKET_COLLTYPE coll_type)
{
	sc_packet_damage packet;
	packet.size = sizeof(packet);
	packet.type = S2C_SET_DAMAGE;
	packet.id = coll_id;
	packet.damage = damage;
	packet.colltype = coll_type;


	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.m_id == client_id) continue;
		else if (cl.second.room_id == SHARED_DATA::g_clients[client_id].room_id) {
			send_packet(cl.second.m_id, &packet);
		}
	}
}

void CServer::send_arrow_create_skill(int client_id ,p_Vec3 LocalPos, PACKET_SKILL skill )
{
	sc_packet_arrow_create_skill packet;
	packet.size = sizeof(packet);
	packet.type = S2C_ARROW_CREATE_SKILL;
	packet.LocalPos = LocalPos;
	packet.skill = skill;
	packet.camp = SHARED_DATA::g_clients[client_id].m_camp;
	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.m_id == client_id) continue;
		else if (cl.second.room_id == SHARED_DATA::g_clients[client_id].room_id) {
			send_packet(cl.second.m_id, &packet);
		}
	}
}


void CServer::send_update_animation(int client_id, int state)
{
	sc_packet_change_anim packet;
	packet.size = sizeof(packet);
	packet.type = S2C_CHANGE_ANIM;
	packet.id = client_id;
	packet.state = state;
	for (int i = 0; i < SHARED_DATA::current_user; ++i)
		if (i == client_id) continue;
		else send_packet(i, &packet);
}

void CServer::send_move_arrow_packet(int client_id, int arrow_id, Vec3 Pos, Vec3 Rot, PACKET_SKILL skill)
{
	//cout << "Move Send ID -" << arrow_id << endl;
	sc_packet_arrow packet;
	packet.size = sizeof(packet);
	packet.type = S2C_MOVE_ARROW;

	packet.Clinetid = client_id;
	packet.Arrowid = arrow_id;
	packet.Pos.x = Pos.x;
	packet.Pos.y = Pos.y;
	packet.Pos.z = Pos.z;

	packet.Rot.x = Rot.x;
	packet.Rot.y = Rot.y;
	packet.Rot.z = Rot.z;
	packet.skill = skill;


	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == SHARED_DATA::g_clients[client_id].room_id)
			send_packet(cl.second.m_id, &packet);

	}
	//for (int i = 0; i < SHARED_DATA::current_user; ++i)
	//	send_packet(i, &packet);
}

void CServer::send_delete_arrow_packet(int clinet_id, int arrow_id, int coll_type, int coll_id, int damage, PACKET_SKILL skill)
{
	//0 자연사
	//1 -> Player
	//2 -> Monster
	//3 -> Tower

	sc_packet_delete_arrow packet;
	packet.size = sizeof(packet);
	packet.type = S2C_DELETE_ARROW;
	packet.client_id = clinet_id;
	packet.arrow_id = arrow_id;
	packet.coll_type = coll_type;
	packet.coll_id = coll_id;
	packet.damage = damage;
	packet.skill = skill;
	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == SHARED_DATA::g_clients[clinet_id].room_id)
			send_packet(cl.second.m_id, &packet);

	}
}

void CServer::send_update_player_helmet(int id, int room_id ,p_Vec3 LocalPos, p_Vec4 Quaternion, p_Vec3 LocalRot, p_Vec3 RevolutionRot)
{
	sc_packet_update_player_helmet packet;
	packet.size = sizeof(packet);
	packet.id = id;
	packet.type = S2C_PLAYER_HELMET;
	packet.LocalPos = LocalPos;
	packet.Quaternion = Quaternion;
	packet.LocalRot = LocalRot;
	packet.RevolutionRot = RevolutionRot;


	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.m_id == id) continue;
		else if (cl.second.room_id == room_id) {
			send_packet(cl.second.m_id, &packet);
		}
	}
}

///

void CServer::send_lobby_team_packet(int room_id, int client_id)
{
	sc_packet_lobby_team packet;
	packet.size = sizeof(packet);
	packet.type = S2C_LOBBY_TEAM;


	for (int i = 0; i < 4; ++i) {
		packet.blue[i] = 999;
		packet.red[i] = 999;
	}
	for (int i = 0; i < CMatchMaking::GetInst()->getLobbyTeamBlue(room_id).size(); ++i) {
		packet.blue[i] = CMatchMaking::GetInst()->getLobbyTeamBlue(room_id)[i];
	}
	for (int i = 0; i < CMatchMaking::GetInst()->getLobbyTeamRed(room_id).size(); ++i) {
		packet.red[i] = CMatchMaking::GetInst()->getLobbyTeamRed(room_id)[i];
	}

	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == room_id)
			send_packet(cl.second.m_id, &packet);

	}
}

void CServer::send_lobby_ready_pacekt(int room_id, bool _isReady, int id)
{
	sc_packet_lobby_ready packet;
	packet.size = sizeof(packet);
	packet.type = S2C_LOBBY_READY;
	packet.isReady = _isReady;
	packet.id = id;
	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == room_id)
			send_packet(cl.second.m_id, &packet);
	}
}

void CServer::send_lobby_change_skill_pacekt(int room_id, PACKET_ELEMENT_TYPE skill, int id)
{
	sc_packet_lobby_change_skill packet;
	packet.size = sizeof(packet);
	packet.type = S2C_LOBBY_CHAGNE_SKILL;
	packet.skill = skill;
	packet.id = id;
	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == room_id)
			send_packet(cl.second.m_id, &packet);
	}
}

#include <random>
#include <ctime>
#include <functional>

void CServer::send_create_box(int room_id, int id)
{

	mt19937 engine((unsigned int)time(NULL));
	uniform_int_distribution<int> distributionBuff(0, 2);

	auto generator = bind(distributionBuff, engine);
	int buff = generator();

	sc_packet_createbox packet;
	packet.size = sizeof(packet);
	packet.type = S2C_CREATE_BOX;
	packet.id = id;
	packet.buf = buff;
	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == room_id)
			send_packet(cl.second.m_id, &packet);
	}
}


void CServer::send_arrow_skill(int user_id, PACKET_SKILL skill)
{
	sc_packet_arrowskill packet;
	packet.size = sizeof(packet);
	packet.type = S2C_ARROW_SKILL;
	packet.id = user_id;
	packet.skill = skill;

	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.m_id == user_id) continue;
		if (cl.second.room_id == SHARED_DATA::g_clients[user_id].room_id)
			send_packet(cl.second.m_id, &packet);
	}
}
