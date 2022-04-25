#pragma once
#include <vector>
#include <thread>
#include "define.h"
#include "externData.h"
using namespace std;
using namespace SHARED_DATA;


class CThread
{
	SINGLE(CThread);
public:
	void worker_Thread();
	void do_timer();
	void init();
	void process_packet(int user_id, char* buf);
	void packet_construct(int user_id, int io_byte);
private:
	vector<thread> worker_threads;
	thread timer_thread;
};

