#pragma once
class CThread
{
	SINGLE(CThread)
public:

	void worker_Thread();
	void do_timer();
	void Init();
	void process_packet(int user_id, char* buf);
	void packet_construct(int user_id, int io_byte);

private:
	vector<thread> worker_threads;
	thread timer_thread;
};

