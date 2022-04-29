#pragma once

class CServer 
{
	SINGLE(CServer)
public:
	void Init();
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

	void send_spawn_minion_packet(int minion_id , float x, float y , float z, CAMP_STATE camp);

	void send_move_minion_packet(int minion_id);

private:

};

