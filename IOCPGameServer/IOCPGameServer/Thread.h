#pragma once
#include <vector>
#include <thread>
#include "define.h"
using namespace std;


class CThread
{
	SINGLE(CThread);
public:
	void worker_Thread();
	void do_timer();
	void init();
private:
	vector<thread> worker_threads;
	thread timer_thread;
};

