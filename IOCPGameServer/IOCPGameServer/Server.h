#pragma once
#include "define.h"
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

enum ENUMOP { OP_SPAWN, OP_MOVE, OP_RECV, OP_SEND, OP_ACCEPT };
enum C_STATUS { ST_FREE, ST_ALLOCATED, ST_ACTIVE };

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
	bool isMove = false;
	unordered_set<int> view_list; //������ ������� �� unordered ���°� �� �ӵ��� ������ 
};





class CServer 
{
	SINGLE(CServer)
public:
	CServer();
	~CServer();

	void Init();
	void AcceptMMServer();
	void Run();

	priority_queue<event_type> timer_queue;
	mutex timer_lock;
	unordered_map<int, MINION> g_minion;
	int g_minionindex;

	CLIENT g_clients[MAX_USER];		//Ŭ���̾�Ʈ ������ŭ �����ϴ� �����̳� �ʿ�

	HANDLE g_iocp;					//iocp �ڵ�
	SOCKET listenSocket;			//���� ��ü�� �ϳ�. �ѹ� �������� �ȹٲ�� �����ͷ��̽� �ƴ�. 
	int current_user;
public:
	

	//lock���� ��ȣ�ް��ִ� �Լ�
	void send_packet(int user_id, void* p);

	void send_my_client_enter_packet(int user_id);

	void send_lobby_login_ok_packet(int user_id);

	void send_login_fail_packet();

	void send_move_packet(int user_id, int mover);

	void send_move_stop_packet(int user_id, int mover);
	
	void send_mouse_packet(int user_id, int mover);

	void send_enter_packet(int user_id, int o_id);

	void send_enter_lobby_packet(int user_id, int o_id);

	void send_near_packet(int client, int new_id);

	void send_leave_packet(int user_id, int o_id);

	void send_spawn_minion_packet(int minion_id);


private:

};

