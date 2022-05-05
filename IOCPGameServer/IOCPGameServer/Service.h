#pragma once
class CService
{
	SINGLE(CService)
public:

	void add_timer(int obj_id, ENUMOP op_type, int duration);
	bool is_near(int a, int b);
	void do_move(int user_id, int direction);
	void do_move_stop(int user_id, int direction);
	void enter_game(int user_id);
	void enter_lobby(int user_id, char name[]);
	void do_Rotation(int user_id);
	void initialize_clients();
	void disconnect(int user_id);
	void update_minion();
	void update_minionindex(int minion_id);
	void update_arrow();
	//void spawn_minion(int id, Vec3 pos);
	void Init();

private:

};




