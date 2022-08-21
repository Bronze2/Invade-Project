#include "pch.h"
#include "Network.h"
#include "SceneMgr.h"
#include "TimeMgr.h"
#include "GameObject.h"
#include "SpawnMgr.h"


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

	ServerAddr.sin_port = htons(8012);
	//과방 192.168.207.150
	//수민 192.168.203.24
	//E320 실습실 10.30.2.115
	//긱사 포트포워딩 121.190.132.143 : 8012
	//10.30.2.19
	string ip;
	cout << "IP :";
	cin >> ip;
	ServerAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	m_Client.socket_info.serverAddr = ServerAddr;
	m_Client.socket_info.connect = false;

	connect(m_Client.socket_info.m_socket, (SOCKADDR*)&m_Client.socket_info.serverAddr, sizeof(SOCKADDR_IN));
	
	ULONG on = 1;	
	ioctlsocket(m_Client.socket_info.m_socket, FIONBIO, &on);
	
	m_Client.socket_info.connect = true;
	//std::cout << "Server Connect Sucsess" << std::endl;

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
		//m_Client.camp = my_packet->camp;
		//m_Client.isHost = my_packet->isHost;
		//cout << "My Client ID :" << m_Client.id <<
		//	"	Camp :" << (int)m_Client.camp << 
		//	"	isHost:"<< boolalpha << m_Client.isHost <<endl;
		cout << "로그인 성공" << endl;
	}
	break;
	case S2C_LOGIN_FALSE:
	{
		sc_packet_login_false* my_packet = reinterpret_cast<sc_packet_login_false*>(ptr);
		cout << "로그인 실패" << endl;
	}
	break;
	case S2C_LOBBY_ENTER:
	{
		sc_packet_lobby_enter* my_packet = reinterpret_cast<sc_packet_lobby_enter*>(ptr);
		int id = my_packet->id;
		cout << my_packet->id << endl;
		if (id == m_Client.id) {
			cout << "ISME" << endl;
		}
		else {
			m_otherClients[id].camp = my_packet->camp;
			m_otherClients[id].isHost = my_packet->isHost;
			m_otherClients[id].id = my_packet->id;
			strcpy_s(m_otherClients[id].name, my_packet->name);
			cout << "Other Client ID :" << m_otherClients[id].id <<
				"	Camp :" << (int)m_otherClients[id].camp <<
				"	isHost:" << boolalpha << m_otherClients[id].isHost << endl;
			enter_count++;

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
			m_Client.camp = my_packet->camp;
			m_Client.skill = (ELEMENT_TYPE)my_packet->skill;
		}
		else {
			m_otherClients[my_packet->id].pos.x = my_packet->pos.x;
			m_otherClients[my_packet->id].pos.y = my_packet->pos.y;
			m_otherClients[my_packet->id].pos.z = my_packet->pos.z;
			m_otherClients[my_packet->id].camp = my_packet->camp;
			m_otherClients[my_packet->id].id = my_packet->id;
			m_otherClients[my_packet->id].skill = (ELEMENT_TYPE)my_packet->skill;

			//CSceneMgr::GetInst()->net_enterClient(id, my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
		}
		current_enter_count++;
		if (current_enter_count == enter_count) {
			cout<<"방인원수 - " << current_enter_count << endl;
			CSceneMgr::GetInst()->ChangeScene(SCENE_TYPE::INGAME);

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
			//CSceneMgr::GetInst()->net_setAnimationByID(my_packet->id, my_packet->state);
			if (my_packet->state == 0) {
				CSceneMgr::GetInst()->net_setAnimationByID(my_packet->id, (int)PLAYER_STATE::WALK);
			}
			else if (my_packet->state == 1) {
				CSceneMgr::GetInst()->net_setAnimationByID(my_packet->id, (int)PLAYER_STATE::RUN);
			}

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

			CSceneMgr::GetInst()->net_setAnimationByID(my_packet->id, (int)PLAYER_STATE::IDLE);
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
		if(my_packet->camp == CAMP_STATE::RED)
			CSceneMgr::GetInst()->net_spawnMinion_red(my_packet->id, my_packet->mtype,my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
		else if(my_packet->camp == CAMP_STATE::BLUE)
			CSceneMgr::GetInst()->net_spawnMinion_blue(my_packet->id, my_packet->mtype,my_packet->pos.x, my_packet->pos.y, my_packet->pos.z);
	}
	break;
	case S2C_MOVE_MINION:
	{
		sc_packet_move_minion* my_packet = reinterpret_cast<sc_packet_move_minion*>(ptr);

		//cout << "Move ID [" << my_packet->id << "]-" << my_packet->pos.x << "," << my_packet->pos.y << ","<< my_packet->pos.z << endl;
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
		cout << "Recv Arrow" << endl;
		//오류 날수 있음 -> int 강제 변환형
		sc_packet_arrow* my_packet = reinterpret_cast<sc_packet_arrow*>(ptr);
		CSceneMgr::GetInst()->net_initArrow(my_packet->Clinetid, my_packet->Arrowid,
			Vec3(my_packet->Pos.x, my_packet->Pos.y, my_packet->Pos.z),
			Vec3(my_packet->Rot.x, my_packet->Rot.y, my_packet->Rot.z) , (int)my_packet->skill);
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
	
	
	case S2C_DAMAGE_MINION:
	{
		sc_packet_damage_minion* my_packet = reinterpret_cast<sc_packet_damage_minion*>(ptr);
		CSceneMgr::GetInst()->net_damageMinion(my_packet->minion_id, (int)my_packet->camp , my_packet->current_hp);

	}
	break;

	case S2C_DAMAGE_TOWER:
	{
		sc_packet_damage_tower* my_packet = reinterpret_cast<sc_packet_damage_tower*>(ptr);
		CSceneMgr::GetInst()->net_damageTower(my_packet->tower_id, (int)my_packet->camp, my_packet->current_hp);

	}
	break;
	case S2C_DELETE_ARROW:
	{
		//sc_packet_delete_arrow* my_packet = reinterpret_cast<sc_packet_delete_arrow*>(ptr);
		//CSceneMgr::GetInst()->net_deleteArrow(my_packet->client_id, my_packet->arrow_id);
		//CSceneMgr::GetInst()->net_DamagedByArrow(my_packet->coll_type, my_packet->coll_id, my_packet->damage);
		//cout << (int)my_packet->skill << endl;
	}
	break;

	case S2C_COLLISION_ARROW:
	{
		sc_packet_collsion_arrow* my_packet = reinterpret_cast<sc_packet_collsion_arrow*>(ptr);
		CSceneMgr::GetInst()->net_deleteArrow(my_packet->id, my_packet->arrow_id);
		CSceneMgr::GetInst()->net_DamagedByArrow((int)my_packet->coll_type, my_packet->coll_id ,500);
	}
	break;

	case S2C_SET_DAMAGE:
	{
		sc_packet_damage* my_packet = reinterpret_cast<sc_packet_damage*>(ptr);
		CSceneMgr::GetInst()->net_DamagedByArrow((int)my_packet->colltype, my_packet->id, my_packet->damage);
	}
	break;

	case S2C_ARROW_CREATE_SKILL:
	{
		sc_packet_arrow_create_skill* my_packet = reinterpret_cast<sc_packet_arrow_create_skill*>(ptr);

		Vec3 LocalPos = Vec3(my_packet->LocalPos.x, my_packet->LocalPos.y, my_packet->LocalPos.z);
		CSceneMgr::GetInst()->net_CreateSkill(LocalPos, (int)my_packet->skill, (int)my_packet->camp);
	}
	break;
	

	// 추가 --> hp == damage 이름 헷갈리지말기 hp가 받은 damage임 이름 잘못넣었음 난 바보 멍청이 똥개니까
	case S2C_DAMAGE_PLAYER:
	{
		 
		sc_damage_player* my_packet = reinterpret_cast<sc_damage_player*>(ptr);
		CSceneMgr::GetInst()->net_damagePlayer(my_packet->player_id, my_packet->hp);
		
	}
	break;

	case S2C_DELETE_PROJECTILE:
	{
		sc_packet_projectile* my_packet = reinterpret_cast<sc_packet_projectile*>(ptr);
		CSceneMgr::GetInst()->net_deletProjectile(my_packet->id);
	}
	break;

	case S2C_CURRENT_ROOM:
	{
		sc_packet_current_room* my_packet = reinterpret_cast<sc_packet_current_room*>(ptr);
		RoomInfo room;
		room.room_id = my_packet->room_id;
		room.roomCurrentUser = my_packet->current_user;
		room.roomMaxUser = my_packet->max_user;
		strcpy_s(room.room_name, my_packet->roomName);
		cout << "==방 정보==" << endl;
		cout << "RoomID : " << room.room_id << "Current :" << room.roomCurrentUser << "Max :" << room.roomMaxUser << endl;
		
		bool check = false;
		for (auto Room : roomInfo) {
			if (Room.room_id == room.room_id) check = true;
		}
		if(!check)
			roomInfo.push_back(room);
	}
	break;

	case S2C_LOBBY_TEAM:
	{
		sc_packet_lobby_team* my_packet = reinterpret_cast<sc_packet_lobby_team*>(ptr);
		
		for (int i = 0; i < 4; ++i) {
			Blue[i] = my_packet->blue[i];
			Red[i] = my_packet->red[i];


		}
			cout << "BlueTeam - " << Blue[0]<<","<< Blue[1] << ","<< Blue[2] << "," << Blue[3] << endl;
			cout << "RedTeam - " << Red[0] << "," << Red[1] << "," << Red[2] << "," << Red[3] << endl;
			isNewState = true;
			CSceneMgr::GetInst()->ChangeLobbyTeamState(Blue, Red);
			
	}
	break;

	case S2C_LOBBY_READY:
	{
		sc_packet_lobby_ready* my_packet = reinterpret_cast<sc_packet_lobby_ready*>(ptr);

		CSceneMgr::GetInst()->LobbySetReady(my_packet->id,my_packet->isReady);

	}
	break;

	case S2C_LOBBY_CHAGNE_SKILL:
	{
		sc_packet_lobby_change_skill* my_packet = reinterpret_cast<sc_packet_lobby_change_skill*>(ptr);

		CSceneMgr::GetInst()->LobbySetSkill(my_packet->id, (ELEMENT_TYPE)my_packet->skill);

	}
	break;

	case S2C_PLAYER_HELMET:
	{
		sc_packet_update_player_helmet* my_packet = reinterpret_cast<sc_packet_update_player_helmet*>(ptr);
		Vec3 LocalPos = Vec3(my_packet->LocalPos.x, my_packet->LocalPos.y, my_packet->LocalPos.z);
		Vec4 Quaternion  = Vec4(my_packet->Quaternion.x, my_packet->Quaternion.y, my_packet->Quaternion.z, my_packet->Quaternion.w);
		Vec3 LocalRot = Vec3(my_packet->LocalRot.x, my_packet->LocalRot.y, my_packet->LocalRot.z);
		Vec3 RevolutionRot = Vec3(my_packet->RevolutionRot.x, my_packet->RevolutionRot.y, my_packet->RevolutionRot.z);

		CSceneMgr::GetInst()->net_playerHelmetUpdate(my_packet->id, LocalPos, Quaternion, LocalRot, RevolutionRot);
	}
	break;

	case S2C_UPDATE_ARROW:
	{
		sc_packet_update_arrow_move* my_packet = reinterpret_cast<sc_packet_update_arrow_move*>(ptr);
		Vec3 LocalPos = Vec3(my_packet->LocalPos.x, my_packet->LocalPos.y, my_packet->LocalPos.z);
		Vec4 Quaternion = Vec4(my_packet->Quaternion.x, my_packet->Quaternion.y, my_packet->Quaternion.z, my_packet->Quaternion.w);

		CSceneMgr::GetInst()->net_updateArrow(my_packet->id, my_packet->arrow_id, LocalPos, Quaternion);
	}
	break;


	case S2C_CREATE_BOX:
	{
		sc_packet_createbox* my_packet = reinterpret_cast<sc_packet_createbox*>(ptr);

		CSpawnMgr::GetInst()->net_CreateBox(my_packet->id , my_packet->buf);
	}
	break;

	case S2C_ARROW_SKILL:
	{
		sc_packet_arrowskill* my_packet = reinterpret_cast<sc_packet_arrowskill*>(ptr);

		CSceneMgr::GetInst()->net_arrowSkill(my_packet->id, (int)my_packet->skill);
	}
	break;

	case S2C_ARROW_PARTICLE:
	{
		sc_packet_arrowskill* my_packet = reinterpret_cast<sc_packet_arrowskill*>(ptr);

		CSceneMgr::GetInst()->net_arrowParticle(my_packet->id, (int)my_packet->skill);
	}
	break;

	case S2C_PLAYER_RESPAWN:
	{
		sc_packet_playerSpawn* my_packet = reinterpret_cast<sc_packet_playerSpawn*>(ptr);

		CSceneMgr::GetInst()->net_playerRespawn(my_packet->id, Vec3(my_packet->Pos.x, my_packet->Pos.y, my_packet->Pos.z));
	}
	break;
	case S2C_END_GAME:
	{
		sc_packet_endgame* my_packet = reinterpret_cast<sc_packet_endgame*>(ptr);
		CSceneMgr::GetInst()->net_change_result();
	}
	break;
	

	case S2C_CHAT_MSG:
	{
		sc_chat_msg* my_packet = reinterpret_cast<sc_chat_msg*>(ptr);
		
		// 인덱스가 0 ~ 12;
		// 12번째에서 항상   11
		if (ChatisMax == true) {
			chatCount = 12;
			for (int i = 0; i < 12; ++i) {
				strcpy_s(veiwChatItmes[i], veiwChatItmes[i + 1]);
			}
			strcpy_s(veiwChatItmes[chatCount], my_packet->msg);
		}
		else {
			strcpy_s(veiwChatItmes[chatCount], my_packet->msg);
			chatCount++;
			if (chatCount > 12) {
				chatCount = 12;
				ChatisMax = true;
			}
		}
	}
	break;
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

void Network::send_lobby_ready(bool _isReady)
{
	cs_packet_lobby_ready m_packet;
	m_packet.type = C2S_LOBBY_READY;
	m_packet.size = sizeof(m_packet);
	m_packet.isReady = _isReady;
	send_packet(&m_packet);
}

void Network::send_lobby_change_skill(ELEMENT_TYPE skill) 
{
	cs_packet_lobby_change_skill m_packet;
	m_packet.type = C2S_LOBBY_CHAGNE_SKILL;
	m_packet.size = sizeof(m_packet);
	m_packet.skill = (PACKET_ELEMENT_TYPE)skill;
	send_packet(&m_packet);
}

void Network::send_enter_room_packet(int room_id)
{
	cs_packet_enter_room m_packet;
	m_packet.type = C2S_ENTER_ROOM;
	m_packet.size = sizeof(m_packet);
	m_packet.room_id = room_id;
	m_packet.clinet_id = m_Client.id;

	my_room_id = room_id;
	send_packet(&m_packet);
	cout << "Send Login Packet" << endl;

}


void Network::send_login_packet(char id[], char password[])
{
	sc_packet_check_login m_packet;
	m_packet.type = C2S_LOGIN;
	m_packet.size = sizeof(m_packet);
	m_packet.loginid = id;
	m_packet.loginpw = password;
	send_packet(&m_packet);
	cout << "Send Login Packet" << endl;
}

void Network::send_move_block_packet(int Client_id, Vec3 Pos)
{
	cs_packet_move_block m_packet;
	m_packet.type = C2S_MOVE_BLOCK;
	m_packet.size = sizeof(m_packet);
	m_packet.id = Client_id;
	m_packet.Pos.x = Pos.x;
	m_packet.Pos.y = Pos.y;
	m_packet.Pos.z = Pos.z;
	send_packet(&m_packet);
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

void Network::send_make_room_packet(MATCH_TYPE match_type, char RoomName[255])
{

	m_Client.isHost = true;
	cs_packet_make_room m_packet;
	m_packet.type = C2S_MAKE_ROOM;
	m_packet.size = sizeof(m_packet);
	m_packet.room_id = m_Client.id;
	m_packet.match = match_type;
	strcpy_s(m_packet.roomName, RoomName);
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


//(int ArrowId, Vec3 Pos, Vec3 Rot, Vec3 Dir, float Power, CAMP_STATE camp, PACKET_SKILL skill)
void Network::send_arrow_packet(int id, PACKET_SKILL skill)
{
	cs_packet_arrow m_packet;
	m_packet.type = C2S_CREATE_ARROW;
	m_packet.size = sizeof(m_packet);
	cout << "화살 send Client "<< id << endl;
	m_packet.Clinet_id = id;
	m_packet.Arrow_id = 0;
	m_packet.Pos.x =0;
	m_packet.Pos.y =0;
	m_packet.Pos.z =0;
	m_packet.Rot.x =0;
	m_packet.Rot.y =0;
	m_packet.Rot.z =0;
	m_packet.Dir.x =0;
	m_packet.Dir.y =0;
	m_packet.Dir.z =0;
	m_packet.Power =0;
	m_packet.room_id = my_room_id;
	m_packet.camp = CAMP_STATE::BLUE;
	m_packet.skill = skill;
	send_packet(&m_packet);

}

void Network::send_search_room_pacekt()
{
	cout << "갱신" << endl;
	cs_packet_find_room m_packet;
	m_packet.type = C2S_FIND_ROOM;
	m_packet.size = sizeof(m_packet);

	send_packet(&m_packet);
}

void Network::send_player_helemt(int id, Vec3 LocalPos, Vec4 Quaternion, Vec3 LocalRot, Vec3 RevolutionRot)
{
	cs_packet_player_helmet m_packet;
	m_packet.type = C2S_PLAYER_HELMET;
	m_packet.size = sizeof(m_packet);
	m_packet.id = id;
	m_packet.LocalPos.x = LocalPos.x;
	m_packet.LocalPos.y = LocalPos.y;
	m_packet.LocalPos.z = LocalPos.z;

	m_packet.Quaternion.x = Quaternion.x;
	m_packet.Quaternion.y = Quaternion.y;
	m_packet.Quaternion.z = Quaternion.z;
	m_packet.Quaternion.w = Quaternion.w;

	m_packet.LocalRot.x = LocalRot.x;
	m_packet.LocalRot.y = LocalRot.y;
	m_packet.LocalRot.z = LocalRot.z;

	m_packet.RevolutionRot.x = RevolutionRot.x;
	m_packet.RevolutionRot.y = RevolutionRot.y;
	m_packet.RevolutionRot.z = RevolutionRot.z;
	send_packet(&m_packet);
}

void Network::send_arrow_create_skill(Vec3 LocalPos , PACKET_SKILL skill)
{
	cs_packet_arrow_create_skill m_packet;
	m_packet.type = C2S_ARROW_CREATE_SKILL;
	m_packet.size = sizeof(m_packet);
	m_packet.LocalPos.x = LocalPos.x;
	m_packet.LocalPos.y = LocalPos.y;
	m_packet.LocalPos.z = LocalPos.z;
	m_packet.skill = skill;

	send_packet(&m_packet);
}

void Network::send_update_arrow_move(int arrow_id , Vec3 LocalPos, Vec4 Quaternion)
{
	cs_packet_update_arrow_move m_packet;
	m_packet.type = C2S_UPDATE_ARROW_MOVE;
	m_packet.size = sizeof(m_packet);
	m_packet.arrow_id = arrow_id;
	m_packet.LocalPos.x = LocalPos.x;
	m_packet.LocalPos.y = LocalPos.y;
	m_packet.LocalPos.z = LocalPos.z;

	m_packet.Quaternion.x = Quaternion.x;
	m_packet.Quaternion.y = Quaternion.y;
	m_packet.Quaternion.z = Quaternion.z;
	m_packet.Quaternion.w = Quaternion.w;
	send_packet(&m_packet);

}

void Network::send_collision_arrow(int arrow_id, int coll_id, PACKET_COLLTYPE coll_type, CAMP_STATE camp)
{
	cs_packet_collsion_arrow m_packet;
	m_packet.type = C2S_COLLISION_ARROW;
	m_packet.size = sizeof(m_packet);
	m_packet.arrow_id = arrow_id;
	m_packet.coll_id = coll_id;
	m_packet.coll_type = coll_type;
	m_packet.camp = camp;
	send_packet(&m_packet);

}

void Network::send_set_damage(int id, int damage, PACKET_COLLTYPE coll_type, CAMP_STATE camp)
{
	cs_packet_damage m_packet;
	m_packet.type = C2S_SET_DAMAGE;
	m_packet.size = sizeof(m_packet);
	m_packet.id = id;
	m_packet.damage = damage;
	m_packet.colltype = coll_type;
	m_packet.camp = camp;
	send_packet(&m_packet);
}


void Network::send_chat_msg(char msg[100])
{
	cs_chat_msg m_packet;
	m_packet.type = C2S_CHAT_MSG;
	m_packet.size = sizeof(m_packet);
	strcpy_s(m_packet.msg, msg);
	cout <<"SEND CHAT - " <<m_packet.msg << endl;
	send_packet(&m_packet);
}

void Network::send_create_Box(int id)
{
	cs_packet_createbox m_packet;
	m_packet.type = C2S_CREATE_BOX;
	m_packet.size = sizeof(m_packet);
	m_packet.id = id;
	send_packet(&m_packet);

}

void Network::send_arrow_skill(int id, PACKET_SKILL skill)
{
	cs_packet_arrowskill m_packet;
	m_packet.type = C2S_ARROW_SKILL;
	m_packet.size = sizeof(m_packet);
	m_packet.id = id;
	m_packet.skill = skill;
	send_packet(&m_packet);
}


void Network::send_arrow_particle(int id, PACKET_SKILL skill)
{
	cs_packet_arrowskill m_packet;
	m_packet.type = C2S_ARROW_PARTICLE;
	m_packet.size = sizeof(m_packet);
	m_packet.id = id;
	m_packet.skill = skill;
	send_packet(&m_packet);
}

void Network::send_player_die(int id)
{
	cs_packet_playerdie m_packet;
	m_packet.type = C2S_PLAYER_DIE;
	m_packet.size = sizeof(m_packet);
	m_packet.id = id;
	send_packet(&m_packet);
	cout << "죽음패킷 전송" << endl;
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
