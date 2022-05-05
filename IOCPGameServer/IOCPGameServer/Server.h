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

	void send_spawn_minion_packet(int minion_id , float x, float y , float z, MINION_ATTACK_TYPE type ,CAMP_STATE camp);

	void send_move_minion_packet(int minion_id);

	void send_anim_minion_packet(int minion_id);

	void send_rot_tower_packet(int tower_id);

	void send_projectile_packet(int bullet_id, int type);

	void send_create_arrow_packet(int client_id, int arrow_id, Vec3 Pos, Vec3 Rot);

	void send_move_arrow_packet(int client_id, int arrow_id, Vec3 Pos, Vec3 Rot);
	
	void send_update_animation(int client_id, int state);


private:

};

