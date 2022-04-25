#include "Server.h"

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

	//�� �ڿ� flag�� overlapped�� �� ��� ����� ����
	listenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	//bind�� ����� server address
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT); //host to network �ؼ� �־�� �Ѵ�
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //��� Ŭ��κ��� ������ �޾ƾ� �Ѵ�
	::bind(listenSocket, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)); //�׳� bind�� ���� c++11�� �ִ� Ű����� ����ȴ�. ���� �տ� ::�� ���δ�.


	listen(listenSocket, SOMAXCONN);

	//IOCP �ڵ� �Ҵ�, IOCP��ü �����
	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	//initialize_clients();


	//listen���� ���
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(listenSocket), g_iocp, 999, 0);
	//���� accept�� ������ �����ϴµ�, �� �Լ��� �̸� ������ ����� �ΰ� �� ������ Ŭ��� ��������ش�. ������ �� �ٸ�.
	//�񵿱�� acceptExȣ��. 
	SOCKET clientSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	EXOVER accept_over;
	ZeroMemory(&accept_over, sizeof(accept_over.over));
	accept_over.op = OP_ACCEPT;
	accept_over.c_socket = clientSocket;
	AcceptEx(listenSocket, clientSocket, accept_over.io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.over);


}


void CServer::send_packet(int user_id, void* p)
{
	char* buf = reinterpret_cast<char*>(p);

	CLIENT& user = g_clients[user_id];

	//WSASend�� �ι�° ������ over�� recv���̶� ���� �ȵȴ�. ���� ������ �Ѵ�.
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
	p.pos.x = g_clients[user_id].Pos.x;
	p.pos.y = g_clients[user_id].Pos.y;
	p.pos.z = g_clients[user_id].Pos.z;

	send_packet(user_id, &p); //&p�� ���� ������ ����Ǿ ���󰡴ϱ� ���ɿ� ������. 
}
void CServer::send_lobby_login_ok_packet(int user_id)
{
	sc_packet_lobby_enter p;
	p.id = user_id;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_OK;
	p.camp = g_clients[user_id].m_camp;
	p.isHost = g_clients[user_id].m_isHost;

	send_packet(user_id, &p); //&p�� ���� ������ ����Ǿ ���󰡴ϱ� ���ɿ� ������. 
}
void CServer::send_login_fail_packet()
{

}
//���̵𿡰�, ���� �̵��ߴ��� �˷����
void CServer::send_move_packet(int user_id, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_KEY_DOWN;
	p.pos.x = g_clients[mover].Pos.x;
	p.pos.y = g_clients[mover].Pos.y;
	p.pos.z = g_clients[mover].Pos.z;
	p.state = g_clients[mover].animState;
	p.move_time = g_clients[mover].m_move_time;
	send_packet(user_id, &p); //&p�� ���� ������ ����Ǿ ���󰡴ϱ� ���ɿ� ������. 
}
void CServer::send_move_stop_packet(int user_id, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_KEY_UP;
	p.pos.x = g_clients[mover].Pos.x;
	p.pos.y = g_clients[mover].Pos.y;
	p.pos.z = g_clients[mover].Pos.z;
	p.state = g_clients[mover].animState;
	p.move_time = g_clients[mover].m_move_time;
	send_packet(user_id, &p); //&p�� ���� ������ ����Ǿ ���󰡴ϱ� ���ɿ� ������. 
}
void CServer::send_mouse_packet(int user_id, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_MOUSE;
	p.pos.x = g_clients[mover].Rot.x;
	p.pos.y = g_clients[mover].Rot.y;
	p.pos.z = g_clients[mover].Rot.z;

	p.move_time = g_clients[mover].m_move_time;

	send_packet(user_id, &p); //&p�� ���� ������ ����Ǿ ���󰡴ϱ� ���ɿ� ������. 
}
void CServer::send_enter_packet(int user_id, int o_id)
{
	sc_packet_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_ENTER;
	p.pos.x = g_clients[o_id].Pos.x;
	p.pos.y = g_clients[o_id].Pos.y;
	p.pos.z = g_clients[o_id].Pos.z;
	strcpy_s(p.name, g_clients[o_id].m_name);
	p.o_type = O_PLAYER;

	g_clients[user_id].m_cLock.lock();
	g_clients[user_id].view_list.insert(o_id);
	g_clients[user_id].m_cLock.unlock();

	send_packet(user_id, &p); //&p�� ���� ������ ����Ǿ ���󰡴ϱ� ���ɿ� ������. 
}
void CServer::send_enter_lobby_packet(int user_id, int o_id)
{
	sc_packet_lobby_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_LOBBY_ENTER;
	p.camp = g_clients[o_id].m_camp;
	p.isHost = g_clients[o_id].m_isHost;
	send_packet(user_id, &p);
}
void CServer::send_near_packet(int client, int new_id)
{
	// new_id�� ���԰� �� new id�� ������ client���� ������ ��
	sc_packet_near packet;
	packet.id = new_id;
	packet.size = sizeof(packet);
	packet.type = S2C_NEAR_PLAYER;
	packet.pos.x = g_clients[new_id].Pos.x;
	packet.pos.y = g_clients[new_id].Pos.y;
	packet.pos.z = g_clients[new_id].Pos.z;

	send_packet(client, &packet);
}
void CServer::send_leave_packet(int user_id, int o_id)
{
	sc_packet_move p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_LEAVE;

	g_clients[user_id].m_cLock.lock();
	g_clients[user_id].view_list.erase(o_id);
	g_clients[user_id].m_cLock.unlock();

	send_packet(user_id, &p); //&p�� ���� ������ ����Ǿ ���󰡴ϱ� ���ɿ� ������. 
}
void CServer::send_spawn_minion_packet(int minion_id)
{
	//sc_packet_spawn_minion packet;
	//packet.size = sizeof(packet);
	//packet.type = S2C_SPAWN_MINION;
	//packet.camp = BLUE;
	//packet.id = minion_id;
	//packet.pos = g_minion[minion_id].Pos;
	//packet.dir = g_minion[minion_id].Dir;
	//packet.rot = g_minion[minion_id].Rot;

	//for (int i = 0; i < current_user; ++i)
	//	send_packet(i, &packet);

}