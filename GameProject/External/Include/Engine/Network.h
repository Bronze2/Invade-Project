#pragma once
#include "Entity.h"

#include "..\..\..\IOCPGameServer\IOCPGameServer\protocol.h"
constexpr auto BUF_SIZE = 200;
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

struct CLIENT {
	SocketInfo socket_info;
	int id = 0;

};


class Network : public CEntity
{
public:
	Network();

	void Initialize();
	void ClientInit();
	//void CreateSocket();
	void ProcessPacket(char* ptr);
	void ProcessData(char* net_buf, size_t io_byte);
	void RecvData();

	void send_packet(void* packet);
	void send_login_packet();
	void send_move_packet(unsigned char dir);

	bool getClientConnect() { return m_Client.socket_info.connect; };
private:
	CLIENT m_Client;
	CLIENT test;
	SOCKET test_socket;
};

