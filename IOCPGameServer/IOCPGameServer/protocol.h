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

//�޴� ���忡���� ��Ŷ type�� ���ƿ´�. ������ ���忡���� �̰� ��� ������ �˷��� �� �ִ�.
#define C2S_LOGIN	1
#define C2S_MOVE	2

#define S2C_LOGIN_OK		1
#define S2C_MOVE			2
#define S2C_ENTER			3
#define S2C_LEAVE			4
#define S2C_NEAR_PLAYER		5


#pragma pack(push ,1)
 
struct sc_packet_login_ok {
	char size;
	char type;
	int id;
	short x;
	short y;
	short z;
	short hp;
	short level;
	int	exp;
};

struct sc_packet_move {
	char size;
	char type;
	int id;
	short x, y, z;
	unsigned move_time;
};

struct sc_packet_near {
	char size;
	char type;
	int id;
	short x, y, z;
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
	short x, y, z;

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
	unsigned move_time;
};

#pragma pack (pop)