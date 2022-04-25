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
//#include "Server.h"

#include "protocol.h"
#include "Timer.h"

#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")

using namespace std;
using namespace chrono;

namespace SHARED_DATA {
	HANDLE g_iocp;					//iocp 핸들
	CLIENT g_clients[MAX_USER];		//클라이언트 동접만큼 저장하는 컨테이너 필요
	unordered_map<int, MINION> g_minion;
	priority_queue<event_type> timer_queue;
	mutex timer_lock;
	SOCKET listenSocket;
	int g_minionindex;
	int current_user;

}

namespace ATOMIC {

}

enum ENUMOP {OP_SPAWN_WAVE , OP_SPAWN, OP_MOVE ,OP_RECV , OP_SEND, OP_ACCEPT };

enum C_STATUS {ST_FREE, ST_ALLOCATED, ST_ACTIVE};


struct event_type
{
	int obj_id;
	ENUMOP event_id; //힐링, 이동 ...
	high_resolution_clock::time_point wakeup_time;
	int target_id;

	constexpr bool operator < (const event_type& left) const
	{
		return (wakeup_time > left.wakeup_time);
	}
};
mutex timer_lock;

//확장 overlapped 구조체
struct EXOVER
{
	WSAOVERLAPPED	over;
	ENUMOP			op;						//send, recv, accpet 중 무엇인지 
	char			io_buf[MAX_BUF_SIZE];	//버퍼의 위치 관리
	
	union {
		WSABUF			wsabuf;					//포인터 넣을 바에야 차라리 버퍼 자체를 넣자. 한군데 같이 두면 확장구조체를 재사용하면 전체가 재사용 된다.
		SOCKET			c_socket;
	};
};

//클라이언트 정보 저장 구조체
struct CLIENT
{
	mutex	m_cLock;
	SOCKET	m_socket;			//lock으로 보호
	int		m_id;				//lock으로 보호
	EXOVER	m_recv_over;
	int		m_prev_size; 
	char	m_packet_buf[MAX_PACKET_SIZE];		//조각난 거 받아두기 위한 버퍼
	atomic<C_STATUS> m_status;

	//게임 콘텐츠 
	Vec3 Pos;
	Vec3 dir;
	Vec3 Rot;
	int animState;
	char m_name[MAX_ID_LEN + 1];			//lock으로 보호

	unsigned  m_move_time;

	char m_camp;
	bool m_isHost;
	bool isMove = false;
	unordered_set<int> view_list; //순서가 상관없을 땐 unordered 쓰는게 더 속도가 빠르다 
};


int g_minionindex;


SOCKET listenSocket;			//서버 전체에 하나. 한번 정해지고 안바뀌니 데이터레이스 아님. 
int current_user;

int wave_count; // 5 5 6

void add_timer(int obj_id, ENUMOP op_type, int duration)
{
	timer_lock.lock();
	event_type ev{ obj_id, op_type,high_resolution_clock::now() + milliseconds(duration), 0 };
	timer_queue.emplace(ev);
	timer_lock.unlock();
}


void do_timer()
{
	while (true)
	{
		//Timer.Update();
		//cout << Timer.GetDeltaTime() << endl;
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




bool is_near(int a, int b)
{

	return true;
}

void do_move(int user_id, int direction)
{
	Vec3 vPos;
	g_clients[user_id].Pos += g_clients[user_id].dir * 30.f;
	send_move_packet(user_id, user_id);
	for (int i = 0; i < current_user; ++i) {
		if (i == user_id) continue;
		else
			send_move_packet(i, user_id);
	}
}

void do_move_stop(int user_id, int direction)
{
	Vec3 vPos;
	send_move_stop_packet(user_id, user_id);
	for (int i = 0; i < current_user; ++i) {
		if (i == user_id) continue;
		else
			send_move_stop_packet(i, user_id);
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

		else if (i != user_id)		//나에게는 보내지 않는다.
		{
			cout << "Enter Other Client" << endl;

			send_enter_lobby_packet(user_id, i);
			send_enter_lobby_packet(i, user_id); //니가 나를 보면 나도 너를 본다
		}
	}
	cout << "Enter	" << endl;
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

void initialize_clients()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		//이건 멀티쓰레드로 돌기 전에, 싱글쓰레드로 돌아가는 함수여서 lock을 거는 의미가 없음. 
		g_clients[i].m_status = ST_FREE;
		g_clients[i].m_id = i;
	}
}


void disconnect(int user_id)
{
	send_leave_packet(user_id, user_id); //나는 나에게 보내기
	
	g_clients[user_id].m_cLock.lock();
	g_clients[user_id].m_status = ST_ALLOCATED;	//처리 되기 전에 FREE하면 아직 떠나는 뒷처리가 안됐는데 새 접속을 받을 수 있음

	closesocket(g_clients[user_id].m_socket);

	for (auto &cl : g_clients)
	{
		if (cl.m_id == user_id) continue;
		//cl.m_cLock.lock();
		if (ST_ACTIVE == cl.m_status)
			send_leave_packet(cl.m_id, user_id);
		//cl.m_cLock.unlock();
	}
	g_clients[user_id].m_status = ST_FREE;	//다 처리했으면 FREE
	g_clients[user_id].m_cLock.unlock();
}

//패킷 재조립 - 유저에게, 바이트만큼의 데이터가 왔다
void packet_construct(int user_id, int io_byte)
{
	CLIENT &curr_user = g_clients[user_id];
	EXOVER &recv_over = curr_user.m_recv_over;

	int rest_byte = io_byte;		//이만큼 남은걸 처리해줘야 한다
	char *p = recv_over.io_buf;		//처리해야할 데이터의 포인터가 필요하다
	int packet_size = 0;			//이게 0이라는 것은 이전에 처리하던 패킷이 없다는 것 
	
	if (curr_user.m_prev_size != 0)	
		packet_size = curr_user.m_packet_buf[0]; //재조립을 기다기는 패킷 사이즈
	
	while (rest_byte > 0)	//처리해야할 데이터가 남아있으면 처리해야한다.
	{
		if (0 == packet_size)	packet_size = p[0];

		//나머지 데이터로 패킷을 만들 수 있나 없나 확인
		if (packet_size <= rest_byte + curr_user.m_prev_size)
		{
			memcpy(curr_user.m_packet_buf + curr_user.m_prev_size, p, packet_size - curr_user.m_prev_size);		//만들어서 처리한 데이터 크기만큼 패킷 사이즈에서 빼주기
			
			p += packet_size - curr_user.m_prev_size;
			rest_byte -= packet_size - curr_user.m_prev_size;
			packet_size = 0;														//이 패킷은 이미 처리를 했고 다음 패킷 사이즈는 모름.

			process_packet(user_id, curr_user.m_packet_buf);
			
			curr_user.m_prev_size = 0;

		}
		else	//패킷 하나를 만들 수 없다면 버퍼에 복사해두고 포인터와 사이즈 증가
		{
			memcpy(curr_user.m_packet_buf + curr_user.m_prev_size, p, rest_byte); //남은 데이터 몽땅 받는데, 지난번에 받은 데이터가 남아있을 경우가 있으니, 그 뒤에 받아야한다.
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

		CLIENT& cl = g_clients[user_id]; //타이핑 줄이기 위해
		
		switch (exover->op)
		{
		case OP_RECV:			//받은 패킷 처리 -> overlapped구조체 초기화 -> recv
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
		case OP_SEND:			//구조체 delete
			if (0 == io_byte)
				disconnect(user_id);

			delete exover;
			break;

		case OP_ACCEPT:			//CreateIoCompletionPort으로 클라소켓 iocp에 등록 -> 초기화 -> recv -> accept 다시(다중접속)
		{
			int user_id = -1;
			for (int i = 0; i < MAX_USER; ++i)
			{
				lock_guard<mutex> gl{ g_clients[i].m_cLock }; //이렇게 하면 unlock이 필요 없다. 이 블록에서 빠져나갈때 unlock을 자동으로 해준다.
				if (ST_FREE == g_clients[i].m_status)
				{
					g_clients[i].m_status = ST_ALLOCATED;
					user_id = i;
					break;
				}
			}

			//main에서 소켓을 worker스레드로 옮겨오기 위해 listen소켓은 전역변수로, client소켓은 멤버로 가져왔다.
			SOCKET clientSocket = exover->c_socket;

			if (-1 == user_id)
				closesocket(clientSocket); // send_login_fail_packet();
			else
			{
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, user_id, 0);

				//g_clients[user_id].m_id = user_id; 멀쓰에서 하는게 아니고 초기화 할때 한번 해줘야 함 처음에 한번.
				g_clients[user_id].m_prev_size = 0; //이전에 받아둔 조각이 없으니 0
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

			//소켓 초기화 후 다시 accept
			clientSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			exover->c_socket = clientSocket; //새로 받는 소켓을 넣어준다. 안그러면 클라들이 같은 소켓을 공유한다.
			ZeroMemory(&exover->over, sizeof(exover->over)); //accept_over를 exover라는 이름으로 받았으니 exover를 재사용
			AcceptEx(listenSocket, clientSocket, exover->io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
		}
			break;
		case OP_MOVE:
		{
			
		}
			break;

		case OP_SPAWN_WAVE:
		{
			g_minion[g_minionindex].Pos.x = 50.f + g_minionindex * 100;
			g_minion[g_minionindex].Pos.y = 0.f;
			g_minion[g_minionindex].Pos.z = 4150.f;

			//send_spawn_minion_packet(g_minionindex-1, g_minionindex, wave_count);
			g_minionindex++;
			//주기적인 스폰
			add_timer(user_id, OP_SPAWN, 1000);
		}
		break;
		case OP_SPAWN:
		{
			g_minion[g_minionindex].Pos.x = 50.f + g_minionindex * 100;
			g_minion[g_minionindex].Pos.y = 0.f;
			g_minion[g_minionindex].Pos.z = 4150.f;
			
			//send_spawn_minion_packet(g_minionindex);			
			g_minionindex++;
			//주기적인 스폰
			add_timer(user_id, OP_SPAWN, 10000);
		}
		break;
		}
	}

}

void main()
{

//	Server* m_Sever = Server::GetInstance();
//	Timer.Init();

	//네트워크 초기화
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	//맨 뒤에 flag를 overlapped로 꼭 줘야 제대로 동작
	listenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	//bind에 사용할 server address
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT); //host to network 해서 넣어야 한다
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //모든 클라로부터 접속을 받아야 한다
	::bind(listenSocket, reinterpret_cast<SOCKADDR *>(&serverAddr), sizeof(serverAddr)); //그냥 bind를 쓰면 c++11에 있는 키워드로 연결된다. 따라서 앞에 ::를 붙인다.


	listen(listenSocket, SOMAXCONN);

	//IOCP 핸들 할당, IOCP객체 만들기
	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	initialize_clients();


	//listen소켓 등록
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(listenSocket), g_iocp, 999, 0);
	//원래 accept는 소켓을 리턴하는데, 이 함수는 미리 소켓을 만들어 두고 그 소켓을 클라와 연결시켜준다. 동작이 좀 다름.
	//비동기로 acceptEx호출. 
	SOCKET clientSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	EXOVER accept_over; 
	ZeroMemory(&accept_over, sizeof(accept_over.over));
	accept_over.op = OP_ACCEPT;
	accept_over.c_socket = clientSocket;
	AcceptEx(listenSocket, clientSocket, accept_over.io_buf , NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.over);
	
	//스레드 만들기
	vector <thread> worker_threads;
	for (int i = 0; i < 10; ++i)
		worker_threads.emplace_back(worker_Thread);
	//메인 종료 전 모든 스레드 종료 기다리기
	thread timer_thread{ do_timer };
	
	for (auto &th : worker_threads) th.join();
	timer_thread.join();
}