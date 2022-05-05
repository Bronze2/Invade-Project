#pragma once
using namespace std;

enum ENUMOP { OP_UPDATE ,OP_MOVE, OP_RECV, OP_SEND, OP_ACCEPT };

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

struct ARROW {
	int m_id;
	Vec3 Pos;
	Vec3 Rot;
	Vec3 Dir;
	float Speed;
	bool enable_shoot = false;
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
	map <int, ARROW> g_arrow;
	char m_camp;
	bool m_isHost;
	bool isMove = false;
	unordered_set<int> view_list; //������ ������� �� unordered ���°� �� �ӵ��� ������ 
};

struct MINION {
	mutex	m_cLock;
	int m_id;
	p_Vec3 Pos;
	p_Vec3 Dir;
	p_Vec3 Rot;
	MINION_STATE State;
	MINION_ATTACK_TYPE Type;
	CAMP_STATE Camp;
	float m_attack_max_time = 100;
	float m_attack_current_time = 0;
	bool m_during_attack = false;
};



namespace SHARED_DATA {
	extern HANDLE g_iocp;					//iocp �ڵ�
	extern CLIENT g_clients[MAX_USER];		//Ŭ���̾�Ʈ ������ŭ �����ϴ� �����̳� �ʿ�
	extern unordered_map<int, MINION> g_minion;
	extern priority_queue<event_type> timer_queue;
	extern mutex timer_lock;
	extern SOCKET listenSocket;
	extern int g_minionindex;
	extern int current_user;
	extern map <int, Vec3> g_tower;
	extern unordered_map<int, Vec3> g_bullet;
	extern int g_bulletindex;
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


