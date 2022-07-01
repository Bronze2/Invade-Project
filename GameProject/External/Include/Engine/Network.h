#pragma once
#include "Entity.h"
#include <unordered_map>

constexpr auto BUF_SIZE = 8192;
//int NPC_ID_START = 10000;
#define MAX_BUF	8192
#define MAX_PACKET_SIZE	8192

struct SocketInfo {
	SOCKET m_socket;
	SOCKADDR_IN serverAddr;
	unsigned char packet_buf[MAX_PACKET_SIZE];
	int prev_packet_data;
	int curr_packet_size;

	bool connect;
};

struct ObjectInfo {
	float x;
	float y;
	float z;

};

struct RoomInfo {
	int room_id;
	int roomMaxUser;
	int roomCurrentUser;
};

struct CLIENT {
	SocketInfo socket_info;
	int id = 0;
	bool isHost;
	CAMP_STATE camp;
	p_Vec3 pos;
	p_Vec3 rot;
	p_Vec3 dir;
};

struct OTHER_CLINET {
	int id = 0;
	bool isHost;
	CAMP_STATE camp;
	p_Vec3 pos;
	p_Vec3 rot;
	p_Vec3 dir;
};


class Network 
{
	SINGLE(Network);
public:

	void Init();
	void ProcessPacket(char* ptr);
	void ProcessData(char* net_buf, size_t io_byte);
	void RecvData();

	void send_packet(void* packet);
	void send_login_packet(char id[], char password[]);
	void send_key_down_packet(unsigned char dir, float x, float y , float z, int state);
	void send_key_up_packet(unsigned char dir, float x, float y, float z, int state);

	void send_rotation_packet(Vec3 Rot);
	void send_game_start_packet();
	void send_attack_ready_packet(int id,int state);
	void send_arrow_packet(int ArrowId, Vec3 Pos, Vec3 Rot, Vec3 Dir, float Power, CAMP_STATE camp);
	void send_move_block_packet(int Client_id, Vec3 Pos);

	//Room
	void send_make_room_packet( MATCH_TYPE match_type);
	void send_enter_room_packet(int room_id);


	bool getClientConnect() { return m_Client.socket_info.connect; };

	int getOtherClientCount() {	return m_OtherClientCount; };
	bool getHost() { return m_Client.isHost; };
	int getHostId() { return m_Client.id; };
	int getOtherClientSize() { return m_otherClients.size(); };
	unordered_map<int, OTHER_CLINET> getOtherClients() { return m_otherClients; };
	CLIENT getMainClient() { return m_Client; };
	void debug_checkclient();
	vector<RoomInfo> roomInfo;

private:
	CLIENT m_Client;
	unordered_map<int, OTHER_CLINET> m_otherClients;
	int m_OtherClientCount = 0;

	//로그인 아이디 비번 임시
	string m_loginid;
	string m_loginpw;
	int my_room_id;
	int enter_count = 1;
	int current_enter_count = 0;
};

