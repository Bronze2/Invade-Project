#pragma once
using namespace std;

enum ENUMOP { OP_UPDATE ,OP_MOVE, OP_RECV, OP_SEND, OP_ACCEPT };

enum C_STATUS { ST_FREE, ST_ALLOCATED, ST_ACTIVE };

struct event_type
{
	int obj_id;
	ENUMOP event_id; //힐링, 이동 ...
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
	ENUMOP			op;						//send, recv, accpet 중 무엇인지 
	char			io_buf[MAX_BUF_SIZE];	//버퍼의 위치 관리

	union {
		WSABUF			wsabuf;					
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

namespace SHARED_DATA {
	extern HANDLE g_iocp;					//iocp 핸들
	extern CLIENT g_clients[MAX_USER];		//클라이언트 동접만큼 저장하는 컨테이너 필요
	extern unordered_map<int, MINION> g_minion;
	extern priority_queue<event_type> timer_queue;
	extern mutex timer_lock;
	extern SOCKET listenSocket;
	extern int g_minionindex;
	extern int current_user;

};
//struct tResolution
//{
//	float fWidth;
//	float fHeight;
//};
//struct tGlobalValue {
//	tResolution vResolution;
//	float fDT;
//	float fAccTime;
//};


