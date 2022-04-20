#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <vector>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <queue>
#include <unordered_map>

#include "define.h"
#include "Server.h"

#include "protocol.h"
#include "Timer.h"

#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")

using namespace std;
using namespace chrono;

enum ENUMOP {OP_SPAWN, OP_MOVE ,OP_RECV , OP_SEND, OP_ACCEPT };

enum C_STATUS {ST_FREE, ST_ALLOCATED, ST_ACTIVE};


struct event_type
{
	int obj_id;
	ENUMOP event_id; //����, �̵� ...
	high_resolution_clock::time_point wakeup_time;
	int target_id;

	constexpr bool operator < (const event_type& left) const
	{
		return (wakeup_time > left.wakeup_time);
	}
};
priority_queue<event_type> timer_queue;
mutex timer_lock;

//Ȯ�� overlapped ����ü
struct EXOVER
{
	WSAOVERLAPPED	over;
	ENUMOP			op;						//send, recv, accpet �� �������� 
	char			io_buf[MAX_BUF_SIZE];	//������ ��ġ ����
	
	union {
		WSABUF			wsabuf;					//������ ���� �ٿ��� ���� ���� ��ü�� ����. �ѱ��� ���� �θ� Ȯ�屸��ü�� �����ϸ� ��ü�� ���� �ȴ�.
		SOCKET			c_socket;
	};
};

//Ŭ���̾�Ʈ ���� ���� ����ü
struct CLIENT
{
	mutex	m_cLock;
	SOCKET	m_socket;			//lock���� ��ȣ
	int		m_id;				//lock���� ��ȣ
	EXOVER	m_recv_over;
	int		m_prev_size; 
	char	m_packet_buf[MAX_PACKET_SIZE];		//������ �� �޾Ƶα� ���� ����
	atomic<C_STATUS> m_status;

	//���� ������ 
	Vec3 Pos;
	Vec3 dir;
	Vec3 Rot;
	int animState;
	char m_name[MAX_ID_LEN + 1];			//lock���� ��ȣ

	unsigned  m_move_time;

	char m_camp;
	bool m_isHost;
	unordered_set<int> view_list; //������ ������� �� unordered ���°� �� �ӵ��� ������ 
};


unordered_map<int, MINION> g_minion;
int g_minionindex;

CLIENT g_clients[MAX_USER];		//Ŭ���̾�Ʈ ������ŭ �����ϴ� �����̳� �ʿ�

HANDLE g_iocp;					//iocp �ڵ�
SOCKET listenSocket;			//���� ��ü�� �ϳ�. �ѹ� �������� �ȹٲ�� �����ͷ��̽� �ƴ�. 
int current_user;



void add_timer(int obj_id, ENUMOP op_type, int duration)
{
	timer_lock.lock();
	event_type ev{ obj_id, op_type,high_resolution_clock::now() + milliseconds(duration), 0 };
	timer_queue.emplace(ev);
	timer_lock.unlock();
}

CTimeMgr Timer;

void do_timer()
{
	while (true)
	{
		Timer.Update();
		this_thread::sleep_for(1ms); //Sleep(1);
		while (true)
		{
			timer_lock.lock();
			if (timer_queue.empty() == true)
			{
				timer_lock.unlock();
				break;
			}
			if (timer_queue.top().wakeup_time > high_resolution_clock::now())
			{
				timer_lock.unlock();
				break;
			}
			event_type ev = timer_queue.top();
			timer_queue.pop();
			timer_lock.unlock();

			switch (ev.event_id)
			{
			case OP_MOVE:
			{	
				//cout << "MoveTimerOn" << endl;
				EXOVER* over = new EXOVER();
				over->op = ev.event_id;
				//do_move(ev.obj_id, packet->direction);
				PostQueuedCompletionStatus(g_iocp, 1, ev.obj_id, &over->over);
			}
			break;
			case OP_SPAWN:
			{
				EXOVER* over = new EXOVER();
				over->op = ev.event_id;
				PostQueuedCompletionStatus(g_iocp, 1, ev.obj_id, &over->over);
			}
				break;
		
			}
		}
	}
}



//lock���� ��ȣ�ް��ִ� �Լ�
void send_packet(int user_id, void *p)
{
	char* buf = reinterpret_cast<char*>(p);

	CLIENT &user = g_clients[user_id];

	//WSASend�� �ι�° ������ over�� recv���̶� ���� �ȵȴ�. ���� ������ �Ѵ�.
	EXOVER *exover = new EXOVER;
	exover->op = OP_SEND;
	ZeroMemory(&exover->over, sizeof(exover->over));
	exover->wsabuf.buf = exover->io_buf;
	exover->wsabuf.len = buf[0];
	memcpy(exover->io_buf, buf, buf[0]);

	WSASend(user.m_socket, &exover->wsabuf, 1, NULL, 0, &exover->over, NULL);
}
void send_my_client_enter_packet(int user_id)
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

void send_lobby_login_ok_packet(int user_id)
{
	sc_packet_lobby_enter p;
	p.id = user_id;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_OK;
	p.camp = g_clients[user_id].m_camp;
	p.isHost = g_clients[user_id].m_isHost;

	send_packet(user_id, &p); //&p�� ���� ������ ����Ǿ ���󰡴ϱ� ���ɿ� ������. 
}


void send_login_fail_packet()
{

}

//���̵𿡰�, ���� �̵��ߴ��� �˷����
void send_move_packet(int user_id, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_MOVE;
	p.pos.x = g_clients[mover].Pos.x;
	p.pos.y = g_clients[mover].Pos.y;
	p.pos.z = g_clients[mover].Pos.z;
	p.state = g_clients[mover].animState;
	p.move_time = g_clients[mover].m_move_time;

	send_packet(user_id, &p); //&p�� ���� ������ ����Ǿ ���󰡴ϱ� ���ɿ� ������. 
}

void send_mouse_packet(int user_id, int mover)
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


void send_enter_packet(int user_id, int o_id)
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

void send_enter_lobby_packet(int user_id, int o_id)
{
	sc_packet_lobby_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_LOBBY_ENTER;
	p.camp = g_clients[o_id].m_camp;
	p.isHost = g_clients[o_id].m_isHost;
	send_packet(user_id, &p);
}


void send_near_packet(int client, int new_id)
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
void send_leave_packet(int user_id, int o_id)
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

void send_spawn_minion_packet(int minion_id)
{
	sc_packet_spawn_minion packet;
	packet.size = sizeof(packet);
	packet.type = S2C_SPAWN_MINION;
	packet.camp = BLUE;
	packet.id = minion_id;
	packet.pos = g_minion[minion_id].Pos;
	packet.dir = g_minion[minion_id].Dir;
	packet.rot = g_minion[minion_id].Rot;

	for (int i = 0; i < current_user; ++i)
		send_packet(i, &packet);

}

bool is_near(int a, int b)
{

	return true;
}

void do_move(int user_id, int direction)
{
	Vec3 vPos;
	g_clients[user_id].Pos += g_clients[user_id].dir * 2000.f * Timer.GetDeltaTime();

	//�����صΰ� ����. �ٵ� �� ���Ŀ� ��߳����� �װ� �����ؾ��Ѵ�. ������ �������� ��������
	g_clients[user_id].m_cLock.lock();
	unordered_set<int> old_vl = g_clients[user_id].view_list;
	g_clients[user_id].m_cLock.unlock();
	
	unordered_set<int> new_vl;
	for (auto &cl : g_clients)
	{
		if (ST_ACTIVE != cl.m_status) continue;
		if (cl.m_id == user_id) continue;
		if (true == is_near(cl.m_id, user_id))
			new_vl.insert(cl.m_id);
	}

	//�ؿ��� view list�� �˷��ִϱ� �����״� �� �˷���. �׷��� ���� ���� ������ �̵��� �˷���� ��.
	send_move_packet(user_id, user_id);
	for (int i = 0; i < current_user; ++i) {
		if (i == user_id) continue;
		else
			send_move_packet(i, user_id);
	}

}

void enter_game(int user_id)
{	
	//g_clients[user_id].m_cLock.lock();
	send_enter_packet(user_id, user_id);
	//g_clients[user_id].m_cLock.unlock();


	for (int i = 0; i < current_user; ++i) {
		if (i != user_id)
			send_enter_packet(i, user_id);
	}
}

void enter_lobby(int user_id, char name[])
{
	g_clients[user_id].m_cLock.lock();
	send_lobby_login_ok_packet(user_id);
	g_clients[user_id].m_status = ST_ACTIVE;
	g_clients[user_id].m_cLock.unlock();

	for (int i = 0; i < user_id + 1; i++)
	{
		if (user_id == i) continue;

		else if (i != user_id)		//�����Դ� ������ �ʴ´�.
		{
			cout << "Enter Other Client" << endl;

			send_enter_lobby_packet(user_id, i);
			send_enter_lobby_packet(i, user_id); //�ϰ� ���� ���� ���� �ʸ� ����
		}
	}
	cout << "Enter" << endl;
}

void do_Rotation(int user_id)
{
	send_mouse_packet(user_id, user_id);
	for (int i = 0; i < current_user; ++i) {
		if (i == user_id) continue;
		else
			send_mouse_packet(i, user_id);
	}

}

void process_packet(int user_id, char* buf)
{
	switch (buf[1]) //[0]�� size
	{
	case C2S_LOGIN:
	{	
		cs_packet_login *packet = reinterpret_cast<cs_packet_login*>(buf);
		cout << "Recv Login Packet Client " << endl;
		enter_lobby(user_id, packet->name);
	}
		break;
	case C2S_MOVE:
	{	cs_packet_move *packet = reinterpret_cast<cs_packet_move*>(buf);
		g_clients[user_id].m_move_time = packet->move_time;
		g_clients[user_id].dir.x = packet->dir.x;
		g_clients[user_id].dir.y = packet->dir.y;
		g_clients[user_id].dir.z = packet->dir.z;
		g_clients[user_id].animState = packet->state;
		//add_timer(user_id, OP_MOVE, 1000);
		do_move(user_id, packet->direction);
	}
		break;
	case C2S_MOUSE:
	{
		cs_packet_mouse *packet = reinterpret_cast<cs_packet_mouse*>(buf);
		g_clients[user_id].m_move_time = packet->move_time;
		g_clients[user_id].Rot.x = packet->Rot.x;
		g_clients[user_id].Rot.y = packet->Rot.y;
		g_clients[user_id].Rot.z = packet->Rot.z;

		do_Rotation(user_id);
	}
		break;
	case C2S_GAMESTART:
	{
		cout << "C2S_GAMESTRAT" << endl;
		cs_packet_lobby_gamestart *packet = reinterpret_cast<cs_packet_lobby_gamestart*>(buf);
		if (packet->id == user_id) {
			if (g_clients[user_id].m_isHost){
				for (int i = 0; i < current_user; ++i) {
					enter_game(i);
				}

				//�÷��̾� ���� �� �̴Ͼ� ���� ����
				add_timer(user_id, OP_SPAWN, 3000);
			}
		}

	}
		break;
	default:
		cout << "unknown packet type error \n";
		
		
		//DebugBreak(); 
		//exit(-1);
	}
}

void initialize_clients()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		//�̰� ��Ƽ������� ���� ����, �̱۾������ ���ư��� �Լ����� lock�� �Ŵ� �ǹ̰� ����. 
		g_clients[i].m_status = ST_FREE;
		g_clients[i].m_id = i;
	}
}


void disconnect(int user_id)
{
	send_leave_packet(user_id, user_id); //���� ������ ������
	
	g_clients[user_id].m_cLock.lock();
	g_clients[user_id].m_status = ST_ALLOCATED;	//ó�� �Ǳ� ���� FREE�ϸ� ���� ������ ��ó���� �ȵƴµ� �� ������ ���� �� ����

	closesocket(g_clients[user_id].m_socket);

	for (auto &cl : g_clients)
	{
		if (cl.m_id == user_id) continue;
		//cl.m_cLock.lock();
		if (ST_ACTIVE == cl.m_status)
			send_leave_packet(cl.m_id, user_id);
		//cl.m_cLock.unlock();
	}
	g_clients[user_id].m_status = ST_FREE;	//�� ó�������� FREE
	g_clients[user_id].m_cLock.unlock();
}

//��Ŷ ������ - ��������, ����Ʈ��ŭ�� �����Ͱ� �Դ�
void packet_construct(int user_id, int io_byte)
{
	CLIENT &curr_user = g_clients[user_id];
	EXOVER &recv_over = curr_user.m_recv_over;

	int rest_byte = io_byte;		//�̸�ŭ ������ ó������� �Ѵ�
	char *p = recv_over.io_buf;		//ó���ؾ��� �������� �����Ͱ� �ʿ��ϴ�
	int packet_size = 0;			//�̰� 0�̶�� ���� ������ ó���ϴ� ��Ŷ�� ���ٴ� �� 
	
	if (curr_user.m_prev_size != 0)	
		packet_size = curr_user.m_packet_buf[0]; //�������� ��ٱ�� ��Ŷ ������
	
	while (rest_byte > 0)	//ó���ؾ��� �����Ͱ� ���������� ó���ؾ��Ѵ�.
	{
		if (0 == packet_size)	packet_size = p[0];

		//������ �����ͷ� ��Ŷ�� ���� �� �ֳ� ���� Ȯ��
		if (packet_size <= rest_byte + curr_user.m_prev_size)
		{
			memcpy(curr_user.m_packet_buf + curr_user.m_prev_size, p, packet_size - curr_user.m_prev_size);		//���� ó���� ������ ũ�⸸ŭ ��Ŷ ������� ���ֱ�
			
			p += packet_size - curr_user.m_prev_size;
			rest_byte -= packet_size - curr_user.m_prev_size;
			packet_size = 0;														//�� ��Ŷ�� �̹� ó���� �߰� ���� ��Ŷ ������� ��.

			process_packet(user_id, curr_user.m_packet_buf);
			
			curr_user.m_prev_size = 0;

		}
		else	//��Ŷ �ϳ��� ���� �� ���ٸ� ���ۿ� �����صΰ� �����Ϳ� ������ ����
		{
			memcpy(curr_user.m_packet_buf + curr_user.m_prev_size, p, rest_byte); //���� ������ ���� �޴µ�, �������� ���� �����Ͱ� �������� ��찡 ������, �� �ڿ� �޾ƾ��Ѵ�.
			curr_user.m_prev_size += rest_byte;
			rest_byte = 0;
			p += rest_byte;
		}
	}
}

void worker_Thread()
{
	while (true) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);

		EXOVER *exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);

		CLIENT& cl = g_clients[user_id]; //Ÿ���� ���̱� ����
		
		switch (exover->op)
		{
		case OP_RECV:			//���� ��Ŷ ó�� -> overlapped����ü �ʱ�ȭ -> recv
		{
			if (0 == io_byte)
			{
				disconnect(user_id);
				if (OP_SEND == exover->op)
					delete exover;
			}
			else
			{
				packet_construct(user_id, io_byte);
				ZeroMemory(&cl.m_recv_over.over, 0, sizeof(cl.m_recv_over.over));
				DWORD flags = 0;
				WSARecv(cl.m_socket, &cl.m_recv_over.wsabuf, 1, NULL, &flags, &cl.m_recv_over.over, NULL);;
			}
			break;
		}
		case OP_SEND:			//����ü delete
			if (0 == io_byte)
				disconnect(user_id);

			delete exover;
			break;

		case OP_ACCEPT:			//CreateIoCompletionPort���� Ŭ����� iocp�� ��� -> �ʱ�ȭ -> recv -> accept �ٽ�(��������)
		{
			int user_id = -1;
			for (int i = 0; i < MAX_USER; ++i)
			{
				lock_guard<mutex> gl{ g_clients[i].m_cLock }; //�̷��� �ϸ� unlock�� �ʿ� ����. �� ��Ͽ��� ���������� unlock�� �ڵ����� ���ش�.
				if (ST_FREE == g_clients[i].m_status)
				{
					g_clients[i].m_status = ST_ALLOCATED;
					user_id = i;
					break;
				}
			}

			//main���� ������ worker������� �Űܿ��� ���� listen������ ����������, client������ ����� �����Դ�.
			SOCKET clientSocket = exover->c_socket;

			if (-1 == user_id)
				closesocket(clientSocket); // send_login_fail_packet();
			else
			{
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, user_id, 0);

				//g_clients[user_id].m_id = user_id; �־����� �ϴ°� �ƴϰ� �ʱ�ȭ �Ҷ� �ѹ� ����� �� ó���� �ѹ�.
				g_clients[user_id].m_prev_size = 0; //������ �޾Ƶ� ������ ������ 0
				g_clients[user_id].m_socket = clientSocket;

				ZeroMemory(&g_clients[user_id].m_recv_over.over, 0, sizeof(g_clients[user_id].m_recv_over.over));
				g_clients[user_id].m_recv_over.op = OP_RECV;
				g_clients[user_id].m_recv_over.wsabuf.buf = g_clients[user_id].m_recv_over.io_buf;
				g_clients[user_id].m_recv_over.wsabuf.len = MAX_BUF_SIZE;

				if (user_id == 0)
					g_clients[user_id].m_isHost = true;

				if (user_id % 2 == 0)
					g_clients[user_id].m_camp = RED;
				else
					g_clients[user_id].m_camp = BLUE;

				g_clients[user_id].view_list.clear();

				DWORD flags = 0;
				WSARecv(clientSocket, &g_clients[user_id].m_recv_over.wsabuf, 1, NULL, &flags, &g_clients[user_id].m_recv_over.over, NULL);
				cout << user_id << endl;
				current_user++;


			}

			//���� �ʱ�ȭ �� �ٽ� accept
			clientSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			exover->c_socket = clientSocket; //���� �޴� ������ �־��ش�. �ȱ׷��� Ŭ����� ���� ������ �����Ѵ�.
			ZeroMemory(&exover->over, sizeof(exover->over)); //accept_over�� exover��� �̸����� �޾����� exover�� ����
			AcceptEx(listenSocket, clientSocket, exover->io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
		}
			break;
		case OP_MOVE:
		{

		}
			break;

		case OP_SPAWN:
		{
			g_minion[g_minionindex].Pos.x = 50.f + g_minionindex * 100;
			g_minion[g_minionindex].Pos.y = 0.f;
			g_minion[g_minionindex].Pos.z = 4150.f;
			
			send_spawn_minion_packet(g_minionindex);			
			g_minionindex++;
			//�ֱ����� ����
			add_timer(user_id, OP_SPAWN, 10000);
		}
		break;
		}
	}

}

void main()
{

	Server* m_Sever = Server::GetInstance();
	Timer.Init();

	//��Ʈ��ũ �ʱ�ȭ
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
	::bind(listenSocket, reinterpret_cast<SOCKADDR *>(&serverAddr), sizeof(serverAddr)); //�׳� bind�� ���� c++11�� �ִ� Ű����� ����ȴ�. ���� �տ� ::�� ���δ�.


	listen(listenSocket, SOMAXCONN);

	//IOCP �ڵ� �Ҵ�, IOCP��ü �����
	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	initialize_clients();


	//listen���� ���
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(listenSocket), g_iocp, 999, 0);
	//���� accept�� ������ �����ϴµ�, �� �Լ��� �̸� ������ ����� �ΰ� �� ������ Ŭ��� ��������ش�. ������ �� �ٸ�.
	//�񵿱�� acceptExȣ��. 
	SOCKET clientSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	EXOVER accept_over; 
	ZeroMemory(&accept_over, sizeof(accept_over.over));
	accept_over.op = OP_ACCEPT;
	accept_over.c_socket = clientSocket;
	AcceptEx(listenSocket, clientSocket, accept_over.io_buf , NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.over);
	
	//������ �����
	vector <thread> worker_threads;
	for (int i = 0; i < 10; ++i)
		worker_threads.emplace_back(worker_Thread);
	//���� ���� �� ��� ������ ���� ��ٸ���
	thread timer_thread{ do_timer };
	
	for (auto &th : worker_threads) th.join();
	timer_thread.join();

	/*while(1) {
		Timer.Update();
	}*/
}