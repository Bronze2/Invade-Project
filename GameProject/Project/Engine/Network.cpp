#include "pch.h"
#include "Network.h"
#include "SceneMgr.h"
#include "TimeMgr.h"


Network::Network() {}
Network::~Network() {}


void Network::Init()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {

	//에러코드 삽입하기
	}
	BOOL optval = TRUE;

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;

	m_Client.socket_info.m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(m_Client.socket_info.m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));

	ServerAddr.sin_port = htons(SERVER_PORT);
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_Client.socket_info.serverAddr = ServerAddr;
	m_Client.socket_info.connect = false;

	connect(m_Client.socket_info.m_socket, (SOCKADDR*)&m_Client.socket_info.serverAddr, sizeof(SOCKADDR_IN));
	
	ULONG on = 1;	
	ioctlsocket(m_Client.socket_info.m_socket, FIONBIO, &on);
	
	m_Client.socket_info.connect = true;
	std::cout << "Server Connect Sucsess" << std::endl;


}

void Network::RecvData()
{
	char buf[BUF_SIZE];
	int retval = recv(m_Client.socket_info.m_socket, buf, BUF_SIZE, 0);
	if (retval == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return;
	}
	else if (retval > 0)
		ProcessData(buf, retval);

}


void Network::ProcessData(char* net_buf, size_t io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[BUF_SIZE];

	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (io_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}
}


void Network::ProcessPacket(char* ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case S2C_LOGIN_OK:
	{
		sc_packet_lobby_enter* my_packet = reinterpret_cast<sc_packet_lobby_enter*>(ptr);
		m_Client.id = my_packet->id;
		m_Client.camp = my_packet->camp;
		m_Client.isHost = my_packet->isHost;
		cout << "My Client ID :" << m_Client.id <<
			"	Camp :" << (int)m_Client.camp << 
			"	isHost:"<< boolalpha << m_Client.isHost <<endl;
	}
	break;
	case S2C_LOBBY_ENTER:
	{
		sc_packet_lobby_enter* my_packet = reinterpret_cast<sc_packet_lobby_enter*>(ptr);
		int id = my_packet->id;
		cout << my_packet->id << endl;
		if (id == m_Client.id) {
		}
		else {
			m_otherClients[id].camp = my_packet->camp;
			m_otherClients[id].isHost = my_packet->isHost;
			m_otherClients[id].id = my_packet->id;

			cout << "Other Client ID :" << m_otherClients[id].id <<
				"	Camp :" << (int)m_otherClients[id].camp <<
				"	isHost:" << boolalpha << m_otherClients[id].isHost << endl;
		}
		debug_checkclient();
	}
	break;
	case S2C_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;
		if (id == m_Client.id) {
			cout << " MAIN ENTER" << endl;
			m_Client.pos.x = my_packet->pos.x;
			m_Client.pos.y = my_packet->pos.y;
			m_Client.pos.z = my_packet->pos.z;
			CSceneMgr::GetInst()->EnterGame();
		}
		else {
			m_otherClients[my_packet->id].pos.x = my_packet->pos.x;
			m_otherClients[my_packet->id].pos.y = my_packet->pos.y;
			m_otherClients[my_packet->id].pos.z = my_packet->pos.z;
			//CSceneMgr::GetInst()->net_enterClient(id, my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
		}
	}
	break;
	case S2C_KEY_DOWN:
	{
		sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = my_packet->id;
		if (other_id == m_Client.id) {
			//핑체크
			//CTimeMgr::GetInst()->SetNetworkEnd();
			
			CSceneMgr::GetInst()->net_setLerpMoveByID(my_packet->id, my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
		}
		else {
			CSceneMgr::GetInst()->net_setLerpMoveByID(my_packet->id, my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
			CSceneMgr::GetInst()->net_setAnimationByID(my_packet->id, my_packet->state);
		}
	}
	break;

	case S2C_KEY_UP:
	{
		//std::cout << "KEY_UP STATE" << std::endl;

		sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = my_packet->id;
		if (other_id == m_Client.id) {
			CSceneMgr::GetInst()->net_setLerpMoveByID(my_packet->id, my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
		}
		else {
			CSceneMgr::GetInst()->net_setLerpMoveByID(my_packet->id, my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
			CSceneMgr::GetInst()->net_setAnimationByID(my_packet->id, my_packet->state);
		}
	}
	break;

	case S2C_MOUSE:
	{
		sc_packet_mouse* my_packet = reinterpret_cast<sc_packet_mouse*>(ptr);
		int other_id = my_packet->id;
		if (other_id == m_Client.id) {
			//CSceneMgr::GetInst()->net_setRotationByID(my_packet->id, my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
		}
		else {
			CSceneMgr::GetInst()->net_setRotationByID(my_packet->id, my_packet->Rot.x, my_packet->Rot.y, my_packet->Rot.z);
		}
	}
	break;

	case S2C_SPAWN_MINION:
	{
		sc_packet_spawn_minion* my_packet = reinterpret_cast<sc_packet_spawn_minion*>(ptr);
		if(my_packet->camp == RED)
			CSceneMgr::GetInst()->net_spawnMinion_red(my_packet->id, my_packet->mtype,my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
		else if(my_packet->camp ==  BLUE)
			CSceneMgr::GetInst()->net_spawnMinion_blue(my_packet->id, my_packet->mtype,my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
		cout << "spawn id: " << my_packet->id <<endl;
	}
	break;
	case S2C_MOVE_MINION:
	{
		sc_packet_move_minion* my_packet = reinterpret_cast<sc_packet_move_minion*>(ptr);
		CSceneMgr::GetInst()->net_moveMinion(my_packet->id, my_packet->pos.x, my_packet->pos.y, my_packet->pos.z,
			my_packet->rot.x, my_packet->rot.y, my_packet->rot.z,my_packet->state);
	}
	break;
	case S2C_ANIM_MINION:
	{
		sc_packet_move_minion* my_packet = reinterpret_cast<sc_packet_move_minion*>(ptr);
		CSceneMgr::GetInst()->net_animMinion(my_packet->id, my_packet->pos.x, my_packet->pos.y, my_packet->pos.z,
			my_packet->rot.x, my_packet->rot.y, my_packet->rot.z, my_packet->state);
	}
	break;

	case S2C_ROT_TOWER:
	{
		sc_packet_rot_tower* my_packet = reinterpret_cast<sc_packet_rot_tower*>(ptr);
		CSceneMgr::GetInst()->net_setRotTower(my_packet->id, Vec3(my_packet->rot.x, my_packet->rot.y, my_packet->rot.z));
	}
	break;

	case S2C_PROJECTILE:
	{
		sc_packet_projectile* my_packet = reinterpret_cast<sc_packet_projectile*>(ptr);
		CSceneMgr::GetInst()->net_moveProjectile(my_packet->id, Vec3(my_packet->pos.x, my_packet->pos.y, my_packet->pos.z));
	}
	break;

	case S2C_CREATE_PROJECTILE:
	{
		sc_packet_projectile* my_packet = reinterpret_cast<sc_packet_projectile*>(ptr);
		CSceneMgr::GetInst()->net_spawnProjectile(my_packet->id, Vec3(my_packet->pos.x, my_packet->pos.y, my_packet->pos.z));
	}
	break;

	case S2C_CREATE_ARROW:
	{
		sc_packet_arrow* my_packet = reinterpret_cast<sc_packet_arrow*>(ptr);
		CSceneMgr::GetInst()->net_initArrow(my_packet->Clinetid, my_packet->Arrowid,
			Vec3(my_packet->Pos.x, my_packet->Pos.y, my_packet->Pos.z),
			Vec3(my_packet->Rot.x, my_packet->Rot.y, my_packet->Rot.z));
	}
	break;

	case S2C_MOVE_ARROW:
	{
		sc_packet_arrow* my_packet = reinterpret_cast<sc_packet_arrow*>(ptr);
		CSceneMgr::GetInst()->net_moveArrow(my_packet->Clinetid, my_packet->Arrowid,
			Vec3(my_packet->Pos.x, my_packet->Pos.y, my_packet->Pos.z),
			Vec3(my_packet->Rot.x, my_packet->Rot.y, my_packet->Rot.z));
	}
	break;

	case S2C_CHANGE_ANIM:
	{
		sc_packet_change_anim* my_packet = reinterpret_cast<sc_packet_change_anim*>(ptr);
		CSceneMgr::GetInst()->net_animUpdate(my_packet->id,my_packet->state);
	}
	break;

	case S2C_DELETE_MINION:
	{
		sc_packet_delete_minion* my_packet = reinterpret_cast<sc_packet_delete_minion*>(ptr);
		CSceneMgr::GetInst()->net_deleteMinion(my_packet->id);
	}
	break;

	case S2C_DELETE_ARROW:
	{
		sc_packet_delete_arrow* my_packet = reinterpret_cast<sc_packet_delete_arrow*>(ptr);
		CSceneMgr::GetInst()->net_deleteArrow(my_packet->client_id, my_packet->arrow_id);
		CSceneMgr::GetInst()->net_DamagedByArrow(my_packet->coll_type, my_packet->coll_id, my_packet->damage);
	}
	break;
	//case S2C_LEAVE:
	//{
	//	sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
	//	int other_id = my_packet->id;
	//	if (other_id == g_myid) {
	//		avatar.hide();
	//	}
	//	else {
	//		if (0 != npcs.count(other_id))
	//			npcs[other_id].hide();
	//	}
	//}
	//break;

	//case S2C_NEAR_PLAYER:
	//{
	//	sc_packet_near* my_packet = reinterpret_cast<sc_packet_near*>(ptr);
	//	int id = my_packet->id;

	//	if (id == g_myid) {
	//		avatar.move(my_packet->x, my_packet->y);
	//		avatar.show();
	//	}
	//	/*else if (id < MAX_USER) {
	//		players[id].move(my_packet->x, my_packet->y);
	//		players[id].show();
	//	}*/
	//}
	//break;

	//default:
	//	printf("Unknown PACKET type [%d]\n", ptr[1]);

	};
}

void Network::send_packet(void* buffer)
{
	char* packet = reinterpret_cast<char*>(buffer);
	int packet_size = (BYTE)packet[0];
	int retval = send(m_Client.socket_info.m_socket, packet, packet_size, 0);
	if (retval == SOCKET_ERROR)
	{
		cout << "오류오류" << endl;
	}

}

void Network::send_login_packet()
{
	sc_packet_login_ok m_packet;
	m_packet.type = C2S_LOGIN;
	m_packet.size = sizeof(m_packet);
	send_packet(&m_packet);
	cout << "Send Login Packet" << endl;
}

void Network::send_key_down_packet(unsigned char dir, float x, float y , float z,int state)
{
	//cs 수정 필요 지금귀찮;
	cs_packet_move m_packet;
	m_packet.type = C2S_KEY_DOWN;
	m_packet.size = sizeof(m_packet);
	m_packet.direction = dir;
	m_packet.dir.x = x;
	m_packet.dir.y = y;
	m_packet.dir.z = z;
	m_packet.state = state;
	send_packet(&m_packet);
//	CTimeMgr::GetInst()->SetNetworkStart();
}

void Network::send_key_up_packet(unsigned char dir, float x, float y, float z, int state)
{
	cs_packet_move m_packet;
	m_packet.type = C2S_KEY_UP;
	m_packet.size = sizeof(m_packet);
	m_packet.direction = dir;
	m_packet.dir.x = x;
	m_packet.dir.y = y;
	m_packet.dir.z = z;
	m_packet.state = state;
	send_packet(&m_packet);
}


void Network::send_attack_ready_packet(int id, int state)
{
	cs_packet_attack_ready m_packet;
	m_packet.type = C2S_ATTACK_READY;
	m_packet.size = sizeof(m_packet);
	m_packet.id = id;
	m_packet.state = state;
	send_packet(&m_packet);
}

void Network::send_rotation_packet(Vec3 Rot)
{
	cs_packet_mouse m_packet;
	m_packet.type = C2S_MOUSE;
	m_packet.size = sizeof(m_packet);
	m_packet.Rot.x = Rot.x;
	m_packet.Rot.y = Rot.y;
	m_packet.Rot.z = Rot.z;
	send_packet(&m_packet);
}

void Network::send_game_start_packet()
{
	cs_packet_lobby_gamestart m_packet;
	m_packet.type = C2S_GAMESTART;
	m_packet.size = sizeof(m_packet);
	m_packet.id = m_Client.id;
	send_packet(&m_packet);
	cout << "Send Enter Packet" << endl;

}
void Network::send_arrow_packet(int ArrowId, Vec3 Pos, Vec3 Rot, Vec3 Dir, float Power)
{
	cs_packet_arrow m_packet;
	m_packet.type = C2S_CREATE_ARROW;
	m_packet.size = sizeof(m_packet);
	m_packet.Clinet_id = m_Client.id;
	m_packet.Arrow_id = ArrowId;
	m_packet.Pos.x = Pos.x;
	m_packet.Pos.y = Pos.y;
	m_packet.Pos.z = Pos.z;
	m_packet.Rot.x = Rot.x;
	m_packet.Rot.y = Rot.y;
	m_packet.Rot.z = Rot.z;
	m_packet.Dir.x = Dir.x;
	m_packet.Dir.y = Dir.y;
	m_packet.Dir.z = Dir.z;
	m_packet.Power = Power;
	send_packet(&m_packet);

}

void Network::debug_checkclient()
{
	for (auto iter = m_otherClients.begin(); iter != m_otherClients.end(); ++iter) {
		cout << "[" << iter->first << ","
			<< iter->second.id << "]\n";
	}
}

//void Network::CreateSocket()
//{
//	BOOL optval = TRUE;
//	int retval = 0;
//	ULONG on = 1;
//
//	SOCKADDR_IN ServerAddr;
//	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
//	ServerAddr.sin_family = AF_INET;
//
//
//	m_Client.socket_info.m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	setsockopt(m_Client.socket_info.m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));
//	cout << "1.1" << endl;
//	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//	ServerAddr.sin_port = htons(SERVER_PORT);
//	retval = connect(m_Client.socket_info.m_socket, (SOCKADDR*)&m_Client.socket_info.serverAddr, sizeof(SOCKADDR_IN));
//	ioctlsocket(m_Client.socket_info.m_socket, FIONBIO, &on);
//	m_Client.socket_info.connect = true;
//}
