#pragma once
#include <chrono>
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <vector>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <unordered_map>
#include "protocol.h"
using namespace std;

enum ENUMOP { OP_SPAWN_WAVE, OP_SPAWN, OP_MOVE, OP_RECV, OP_SEND, OP_ACCEPT };

enum C_STATUS { ST_FREE, ST_ALLOCATED, ST_ACTIVE };

struct event_type
{
	int obj_id;
	ENUMOP event_id; //����, �̵� ...
	std::chrono::high_resolution_clock::time_point wakeup_time;
	int target_id;

	constexpr bool operator < (const event_type& left) const
	{
		return (wakeup_time > left.wakeup_time);
	}
};

struct EXOVER
{
	WSAOVERLAPPED	over;
	ENUMOP			op;						//send, recv, accpet �� �������� 
	char			io_buf[MAX_BUF_SIZE];	//������ ��ġ ����

	union {
		WSABUF			wsabuf;					
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