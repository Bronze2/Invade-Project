#include "pch.h"
#include "Network.h"
#include "SceneMgr.h"


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
		sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
		m_Client.id = my_packet->id;
		cout << "My Client ID :" << m_Client.id <<"로그인 성공"<<endl;
		CSceneMgr::GetInst()->setMainClient(m_Client.id, my_packet->x, my_packet->y, my_packet->z);
	}
	break;

	case S2C_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;
		if (id == m_Client.id) {
		}
		else {
			CSceneMgr::GetInst()->EnterClient(id, my_packet->x, my_packet->y, my_packet->z);
		}
	}
	break;
	case S2C_MOVE:
	{
		sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = my_packet->id;
		if (other_id == m_Client.id) {

		}
		else {
			
		}
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

	}
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

void Network::send_move_packet(unsigned char dir)
{
	cs_packet_move m_packet;
	m_packet.type = C2S_MOVE;
	m_packet.size = sizeof(m_packet);
	m_packet.direction = dir;
	send_packet(&m_packet);
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
