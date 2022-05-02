#pragma once

constexpr int MAX_ID_LEN = 50;
constexpr int MAX_STR_LEN = 255;
constexpr int MAX_PACKET_SIZE = 255;
constexpr auto MAX_BUF_SIZE = 1024;
constexpr auto MAX_USER = 10000;
constexpr auto VIEW_RADIUS = 600;

#define WORLD_WIDTH		400
#define WORLD_HEIGHT	400

#define SERVER_PORT		9000

//받는 입장에서는 패킷 type이 날아온다. 보내는 입장에서는 이걸 담아 보내야 알려줄 수 있다.
#define C2S_LOGIN		1
#define C2S_KEY_DOWN	2
#define C2S_KEY_UP		3
#define C2S_MOUSE		4
#define C2S_GAMESTART	5

#define S2C_LOGIN_OK		1
#define S2C_KEY_DOWN		2
#define S2C_KEY_UP			3
#define S2C_ENTER			4
#define S2C_LEAVE			5
#define S2C_NEAR_PLAYER		6

#define S2C_MOUSE			6
#define S2C_SPAWN_MINION	7
#define S2C_LOBBY_ENTER		8
#define S2C_MOVE_MINION		9
#define S2C_ANIM_MINION		10



struct p_Vec3 {
	float x;
	float y;
	float z;
};



#pragma pack(push ,1)


 
struct sc_packet_login_ok {
	char size;
	char type;
	int id;
	p_Vec3 pos;
	short hp;
	short level;
	int	exp;
};

struct sc_packet_move {
	char size;
	char type;
	int id;
	p_Vec3 pos;
	int state;
	unsigned move_time;
};

struct sc_packet_mouse {
	char size;
	char type;
	int id;
	p_Vec3 Rot;
	unsigned move_time;
};

struct sc_packet_near {
	char size;
	char type;
	int id;
	p_Vec3 pos;
	unsigned move_time;
};

//OBJECT TYPE
constexpr unsigned char O_PLAYER = 0;
constexpr unsigned char O_NPC = 1;

struct sc_packet_enter {
	char size;
	char type;
	int id;
	char name[MAX_ID_LEN];
	char o_type;
	p_Vec3 pos;

};

struct sc_packet_leave {
	char size;
	char type;
	int id;
};

struct cs_packet_login {
	char	size;
	char	type;
	char	name[MAX_ID_LEN];
};

constexpr unsigned char D_UP = 0;
constexpr unsigned char D_DOWN = 1;
constexpr unsigned char D_LEFT = 2;
constexpr unsigned char D_RIGHT = 3;

struct cs_packet_move {
	char	size;
	char	type;
	char	direction;
	float dir_x;
	float dir_y;
	float dir_z;
	p_Vec3 dir;
	int state;
	unsigned move_time;
};

struct cs_packet_mouse {
	char	size;
	char	type;
	p_Vec3 Rot;
	unsigned move_time;
};

constexpr unsigned char RED = 0;
constexpr unsigned char BLUE = 1;


constexpr unsigned char warrior = 0;
constexpr unsigned char ranger = 1;
constexpr unsigned char bomb = 2;

struct sc_packet_spawn_minion {
	char	size;
	char	type;
	int		id;
	char    camp;
	int		mtype;
	p_Vec3  pos;
	p_Vec3  dir;
	p_Vec3  rot;
};



struct sc_packet_move_minion {
	char size;
	char type;
	int  id;
	p_Vec3 pos;
	p_Vec3 rot;
	int state;
};


////////////////// Lobby ///////////////////////////////////

struct sc_packet_lobby_enter {
	char size;
	char type;
	char camp;
	int id;
	bool isHost;
};

struct cs_packet_lobby_enter {
	char size;
	char type;
};

struct sc_packet_lobby_ready_info {
	char size;
	char type;
	int id;
	bool isReady;
};

struct cs_packet_lobby_ready_info {
	char size;
	char type;
	int id;
	bool isReady;
};

struct sc_packet_lobby_gamestart {
	char size;
	char type;
};

struct cs_packet_lobby_gamestart {
	char size;
	char type;
	int id;
};







#pragma pack (pop)