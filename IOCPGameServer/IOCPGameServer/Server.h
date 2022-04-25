#pragma once
#include "define.h"
#include "externData.h"


#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")

using namespace std;
using namespace chrono;
using namespace SHARED_DATA;


class CServer 
{
	SINGLE(CServer)
public:
	CServer();
	~CServer();

	void Init();
	void AcceptMMServer();
	void Run();

public:
	

	//lock으로 보호받고있는 함수
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

