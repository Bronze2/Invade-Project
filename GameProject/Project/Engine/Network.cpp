#include "pch.h"
#include "Network.h"


Network::Network()
{
	Initialize();
}


void Network::Initialize()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {

	//에러코드 삽입하기
	}
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
		sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
		m_Client.id = my_packet->id;
		//avatar.move(my_packet->x, my_packet->y);
	}
	break;

	case S2C_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;

		if (id == m_Client.id) {
		}
		else {
			//if (id < NPC_ID_START)
			//	npcs[id] = OBJECT{ *pieces, 64, 0, 64, 64 };
			//else
			//	npcs[id] = OBJECT{ *pieces, 0, 0, 64, 64 };
			//strcpy_s(npcs[id].name, my_packet->name);
			//npcs[id].set_name(my_packet->name);
			//npcs[id].move(my_packet->x, my_packet->y);
		}
	}
	break;
	//case S2C_MOVE:
	//{
	//	sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
	//	int other_id = my_packet->id;
	//	if (other_id == g_myid) {
	//		avatar.move(my_packet->x, my_packet->y);
	//		g_left_x = my_packet->x - (SCREEN_WIDTH / 2);
	//		g_top_y = my_packet->y - (SCREEN_HEIGHT / 2);
	//	}
	//	else {
	//		if (0 != npcs.count(other_id))
	//			npcs[other_id].move(my_packet->x, my_packet->y);
	//	}
	//}
	//break;

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

	}
}

void Network::CreateSocket()
{
	BOOL optval = TRUE;
	int retval = 0;
	ULONG on = 1;

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;


	m_Client.m_socket.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(m_Client.m_socket.socket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));

	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(SERVER_PORT);
	ioctlsocket(m_Client.m_socket.socket, FIONBIO, &on);
	m_Client.m_socket.connect = true;
}
