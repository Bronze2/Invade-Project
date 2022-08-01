#pragma once

class CServer 
{
	SINGLE(CServer)
public:
	void Init();
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

	void send_spawn_minion_packet(int minion_id , float x, float y , float z, MINION_ATTACK_TYPE type ,CAMP_STATE camp);

	void send_move_minion_packet(int minion_id, int room_id);

	void send_anim_minion_packet(int minion_id, int room_id);

	void send_rot_tower_packet(int tower_id);

	void send_projectile_packet(int bullet_id, int type);

	void send_create_arrow_packet(int client_id, int arrow_id, Vec3 Pos, Vec3 Rot, PACKET_SKILL skill);

	void send_move_arrow_packet(int client_id, int arrow_id, Vec3 Pos, Vec3 Rot, PACKET_SKILL skill);
	
	void send_update_animation(int client_id, int state);
	
	void send_delete_minion(int minion_id);

	void send_delete_arrow_packet(int clinet_id, int arrow_id, int coll_type,int coll_id ,int damage, PACKET_SKILL skill);

	void send_current_room(int user_id, int room_id, int max_user, int current_user, string RoomName);

	void send_lobby_team_packet(int room_id, int client_id);

	void send_lobby_ready_pacekt(int room_id, bool _isReady , int id);

	void send_lobby_change_skill_pacekt(int room_id, PACKET_ELEMENT_TYPE skill, int id);

	void send_update_player_helmet(int id,int room_id, p_Vec3 LocalPos, p_Vec4 Quaternion, p_Vec3 LocalRot, p_Vec3 RevolutionRot);

	void send_update_arrow(int client_id, int arrow_id, p_Vec3 LocalPos, p_Vec4 Quaternion);

	void send_collision_arrow(int client_id, int arrow_id, int coll_id, PACKET_COLLTYPE coll_type);

	void send_arrow_create_skill(int client_id, p_Vec3 LocalPos, PACKET_SKILL skill);

	void send_setDamage(int client_id, int coll_id, int damage, PACKET_COLLTYPE coll_type);

	void send_damage_minion(int minion_id, int hp, CAMP_STATE camp);

	void send_delete_tower(int tower_id);

	void send_damage_tower(int tower_id,int  hp, CAMP_STATE camp);

	void send_chat_msg(int clinet_id, char msg[100]);

	void send_damage_player(int player_id, int hp);

	void send_create_box(int room_id, int id);

	void send_arrow_skill(int client_id, int user_id, PACKET_SKILL skill);
	
	void send_arrow_particle(int user_id, PACKET_SKILL skill);


	void send_player_spawn(int user_id , Vec3 Pos);

	void send_end_game(int index);

private:

};

