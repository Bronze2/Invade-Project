#pragma once
constexpr int MAX_ID_LEN = 50;
constexpr int MAX_STR_LEN = 255;
constexpr int MAX_PACKET_SIZE = 255;
constexpr auto MAX_BUF_SIZE = 1024;
constexpr auto MAX_USER = 10000;
constexpr auto VIEW_RADIUS = 600;

#define WORLD_WIDTH		400
#define WORLD_HEIGHT	400

#define SERVER_PORT		8012

//받는 입장에서는 패킷 type이 날아온다. 보내는 입장에서는 이걸 담아 보내야 알려줄 수 있다.
#define C2S_LOGIN		1
#define C2S_KEY_DOWN	2
#define C2S_KEY_UP		3
#define C2S_MOUSE		4
#define C2S_GAMESTART	5
#define C2S_CREATE_ARROW  6
#define C2S_ATTACK_READY 7
#define C2S_MOVE_BLOCK 8
#define C2S_MAKE_ROOM 9
#define C2S_ENTER_ROOM 10
#define C2S_LOBBY_READY 11
#define C2S_LOBBY_CHAGNE_SKILL 12
#define C2S_PLAYER_HELMET 13
#define C2S_UPDATE_ARROW_MOVE 14
#define C2S_COLLISION_ARROW 15


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
#define S2C_ROT_TOWER		11
#define S2C_CREATE_PROJECTILE		12
#define S2C_PROJECTILE		13
#define S2C_CREATE_ARROW		14
#define S2C_MOVE_ARROW		15
#define S2C_CHANGE_ANIM		16
#define S2C_DELETE_MINION	17
#define S2C_DELETE_ARROW	18
#define S2C_DELETE_PROJECTILE	19
#define S2C_LOGIN_FALSE 20
#define S2C_CURRENT_ROOM 21
#define S2C_LOBBY_TEAM 22
#define S2C_LOBBY_READY 23
#define S2C_LOBBY_CHAGNE_SKILL 24
#define S2C_LOBBY2INGAME 25
#define S2C_PLAYER_HELMET 26
#define S2C_UPDATE_ARROW 27
#define S2C_COLLISION_ARROW 28



enum class  MATCH_TYPE { TWO = 0, THREE, FOUR ,END };
enum class PACKET_SKILL { E_WATER = 0, Z_WATER, E_FIRE,  Z_FIRE, E_THUNDER, Z_TUNDER, E_DARK, Z_DARK, E_WIND,Z_WIND ,NONE};
enum class PACKET_COLLTYPE { PLAYER = 0 , MONSTER =1 , TOWER =2 , NEXSUS =3 , WALL = 4 };
enum class PACKET_ELEMENT_TYPE {

	START = -1,
	WATER = 0,
	FIRE = 1,
	DARK = 2,
	THUNDER = 3,
	WIND = 4
};
struct p_Vec3 {
	float x;
	float y;
	float z;
};

struct p_Vec4 {
	float x;
	float y;
	float z;
	float w;
};

enum class CAMP_STATE {
	RED,
	BLUE,
};

#pragma pack(push ,1)


struct sc_packet_check_login {
	char size;
	char type;
	std::string loginid;
	std::string loginpw;

	//char loginid[MAX_ID_LEN];
	//char loginpw[MAX_ID_LEN];
};


struct sc_packet_current_room {
	char size;
	char type;
	int room_id;
	int max_user;
	int current_user;

};


 
struct sc_packet_login_ok {
	char size;
	char type;
	int id;
	p_Vec3 pos;
	short hp;
	short level;
	int	exp;
};
struct sc_packet_login_false {
	char size;
	char type;
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

struct cs_packet_player_helmet {
	char size;
	char type;
	int id;
	p_Vec3 LocalPos;
	p_Vec4 Quaternion;
	p_Vec3 LocalRot;
	p_Vec3 RevolutionRot;

};

struct sc_packet_update_player_helmet {
	char size;
	char type;
	int id;
	p_Vec3 LocalPos;
	p_Vec4 Quaternion;
	p_Vec3 LocalRot;
	p_Vec3 RevolutionRot;

};

struct cs_packet_update_arrow_move
{
	char size;
	char type;
	int arrow_id;
	p_Vec3 LocalPos;
	p_Vec4 Quaternion;
};

struct sc_packet_update_arrow_move
{
	char size;
	char type;
	int id;
	int arrow_id;
	p_Vec3 LocalPos;
	p_Vec4 Quaternion;
};

struct cs_packet_collsion_arrow
{
	char size;
	char type;
	int arrow_id;
	int coll_id;
	CAMP_STATE camp;
	PACKET_COLLTYPE coll_type;
};


struct sc_packet_collsion_arrow
{
	char size;
	char type;
	int id;
	int arrow_id;
	int coll_id;
	PACKET_COLLTYPE coll_type;
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
	PACKET_ELEMENT_TYPE skill;
	p_Vec3 pos;
	//char camp;
	CAMP_STATE camp;
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

struct sc_packet_change_anim {
	char	size;
	char	type;
	int id;
	int state;
};


constexpr unsigned char D_UP = 0;
constexpr unsigned char D_DOWN = 1;
constexpr unsigned char D_LEFT = 2;
constexpr unsigned char D_RIGHT = 3;

struct cs_packet_make_room {
	char size;
	char type;
	int room_id;
	MATCH_TYPE match;
};

struct cs_packet_enter_room {
	char size;
	char type;
	int room_id;
	int clinet_id;
};

struct cs_packet_move_block {
	char	size;
	char	type;
	int id;
	p_Vec3 Pos;
	unsigned move_time;
};


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

struct cs_packet_attack_ready {
	char	size;
	char	type;
	int id;
	int state;
	//unsigned move_time;
};



struct cs_packet_arrow {
	char	size;
	char	type;
	int Clinet_id;
	int Arrow_id;
	float Power;
	p_Vec3 Pos;
	p_Vec3 Rot;
	p_Vec3 Dir;
	int room_id;
	//char camp;
	CAMP_STATE camp;
	PACKET_SKILL skill;

	unsigned move_time;
};

constexpr unsigned char BLUE = 0;
constexpr unsigned char RED = 1;


constexpr unsigned char warrior = 0;
constexpr unsigned char ranger = 1;
constexpr unsigned char bomb = 2;

struct sc_packet_spawn_minion {
	char	size;
	char	type;
	int		id;
	//char    camp;
	CAMP_STATE camp;

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

struct sc_packet_delete_minion {
	char size;
	char type;
	int id;
};

struct sc_packet_delete_arrow {
	char size;
	char type;
	int client_id;
	int arrow_id;
	int coll_type;
	int coll_id;
	int damage;
	PACKET_SKILL skill;
};


struct sc_packet_rot_tower {
	char size;
	char type;
	int  id;
	p_Vec3 rot;
};

struct sc_packet_projectile {
	char size;
	char type;
	int  id;
	p_Vec3 pos;
};

struct sc_packet_arrow {
	char size;
	char type;
	int  Clinetid;
	int  Arrowid;
	p_Vec3 Pos;
	p_Vec3 Rot;
	PACKET_SKILL skill;
};



////////////////// Lobby ///////////////////////////////////



struct cs_packet_lobby_ready {
	char size;
	char type;
	bool isReady;
};

struct sc_packet_lobby_ready {
	char size;
	char type;
	int id;
	bool isReady;
};

struct cs_packet_lobby_change_skill {
	char size;
	char type;
	PACKET_ELEMENT_TYPE skill;
};

struct sc_packet_lobby_change_skill {
	char size;
	char type;
	int id;
	PACKET_ELEMENT_TYPE skill;
};


struct sc_packet_lobby_team {
	char size;
	char type;
	int blue[4];
	int red[4];
};

struct sc_packet_lobby_enter {
	char size;
	char type;
	//char camp;
	CAMP_STATE camp;

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

struct sc_packet_lobby2ingame {
	char size;
	char type;
	int id;

};







#pragma pack (pop)