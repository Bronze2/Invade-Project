#include <windows.h>
#include <WS2tcpip.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <unordered_map>
#include <chrono>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;
using namespace chrono;

#include "..\..\..\IOCPGameServer\IOCPGameServer\protocol.h"

#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")

constexpr auto BUF_SIZE = 200;
//int NPC_ID_START = 10000;

struct SocketInfo {
	SOCKET socket;
	SOCKADDR_IN serverAddr;
	unsigned char packet_buf[MAX_PACKET_SIZE];
	int prev_packet_data;
	int curr_packet_size;

	bool connect;
};

struct CLIENT {
	int id;
	SocketInfo m_socket;
};


#pragma once
class Network
{
public:
	Network();

	void Initialize();
	void ClientInit();
	void CreateSocket();
	void ProcessPacket(char* ptr);
	void ProcessData(char* net_buf, size_t io_byte);
	void send_packet(void* packet);
	void send_move_packet(unsigned char dir);


	CLIENT m_Client;
private:
};

