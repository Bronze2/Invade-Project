#include "Thread.h"
////스레드 만들기
//vector <thread> worker_threads;
//for (int i = 0; i < 10; ++i)
//	worker_threads.emplace_back(worker_Thread);
////메인 종료 전 모든 스레드 종료 기다리기
//thread timer_thread{ do_timer };
//
//for (auto& th : worker_threads) th.join();
//timer_thread.join();

void CThread::init()
{
	for (int i = 0; i < 10; ++i)
		worker_threads.emplace_back(CThread::worker_Thread());
	timer_thread = std::thread([&]() { CThread::do_timer(); });
}

void CThread::do_timer()
{
	while (true)
	{
		this_thread::sleep_for(1ms); //Sleep(1);
		while (true)
		{
			timer_lock.lock();
			if (timer_queue.empty() == true)
			{
				timer_lock.unlock();
				break;
			}
			if (timer_queue.top().wakeup_time > high_resolution_clock::now())
			{
				timer_lock.unlock();
				break;
			}
			event_type ev = timer_queue.top();
			timer_queue.pop();
			timer_lock.unlock();

			switch (ev.event_id)
			{
			case OP_MOVE:
			{
			}
			break;
			case OP_SPAWN:
			{
				EXOVER* over = new EXOVER();
				over->op = ev.event_id;
				PostQueuedCompletionStatus(g_iocp, 1, ev.obj_id, &over->over);
			}
			break;

			}
		}
	}
}

void worker_Thread()
{
	while (true) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);

		EXOVER* exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);

		CLIENT& cl = g_clients[user_id]; //타이핑 줄이기 위해

		switch (exover->op)
		{
		case OP_RECV:			//받은 패킷 처리 -> overlapped구조체 초기화 -> recv
		{
			if (0 == io_byte)
			{
				disconnect(user_id);
				if (OP_SEND == exover->op)
					delete exover;
			}
			else
			{
				packet_construct(user_id, io_byte);
				ZeroMemory(&cl.m_recv_over.over, 0, sizeof(cl.m_recv_over.over));
				DWORD flags = 0;
				WSARecv(cl.m_socket, &cl.m_recv_over.wsabuf, 1, NULL, &flags, &cl.m_recv_over.over, NULL);;
			}
			break;
		}
		case OP_SEND:			//구조체 delete
			if (0 == io_byte)
				disconnect(user_id);

			delete exover;
			break;

		case OP_ACCEPT:			//CreateIoCompletionPort으로 클라소켓 iocp에 등록 -> 초기화 -> recv -> accept 다시(다중접속)
		{
			int user_id = -1;
			for (int i = 0; i < MAX_USER; ++i)
			{
				lock_guard<mutex> gl{ g_clients[i].m_cLock }; //이렇게 하면 unlock이 필요 없다. 이 블록에서 빠져나갈때 unlock을 자동으로 해준다.
				if (ST_FREE == g_clients[i].m_status)
				{
					g_clients[i].m_status = ST_ALLOCATED;
					user_id = i;
					break;
				}
			}

			//main에서 소켓을 worker스레드로 옮겨오기 위해 listen소켓은 전역변수로, client소켓은 멤버로 가져왔다.
			SOCKET clientSocket = exover->c_socket;

			if (-1 == user_id)
				closesocket(clientSocket); // send_login_fail_packet();
			else
			{
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, user_id, 0);

				//g_clients[user_id].m_id = user_id; 멀쓰에서 하는게 아니고 초기화 할때 한번 해줘야 함 처음에 한번.
				g_clients[user_id].m_prev_size = 0; //이전에 받아둔 조각이 없으니 0
				g_clients[user_id].m_socket = clientSocket;

				ZeroMemory(&g_clients[user_id].m_recv_over.over, 0, sizeof(g_clients[user_id].m_recv_over.over));
				g_clients[user_id].m_recv_over.op = OP_RECV;
				g_clients[user_id].m_recv_over.wsabuf.buf = g_clients[user_id].m_recv_over.io_buf;
				g_clients[user_id].m_recv_over.wsabuf.len = MAX_BUF_SIZE;

				if (user_id == 0)
					g_clients[user_id].m_isHost = true;

				if (user_id % 2 == 0)
					g_clients[user_id].m_camp = RED;
				else
					g_clients[user_id].m_camp = BLUE;

				g_clients[user_id].view_list.clear();

				DWORD flags = 0;
				WSARecv(clientSocket, &g_clients[user_id].m_recv_over.wsabuf, 1, NULL, &flags, &g_clients[user_id].m_recv_over.over, NULL);
				cout << user_id << endl;
				current_user++;


			}

			//소켓 초기화 후 다시 accept
			clientSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			exover->c_socket = clientSocket; //새로 받는 소켓을 넣어준다. 안그러면 클라들이 같은 소켓을 공유한다.
			ZeroMemory(&exover->over, sizeof(exover->over)); //accept_over를 exover라는 이름으로 받았으니 exover를 재사용
			AcceptEx(listenSocket, clientSocket, exover->io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
		}
		break;
		case OP_MOVE:
		{

		}
		break;

		case OP_SPAWN_WAVE:
		{
			g_minion[g_minionindex].Pos.x = 50.f + g_minionindex * 100;
			g_minion[g_minionindex].Pos.y = 0.f;
			g_minion[g_minionindex].Pos.z = 4150.f;

			send_spawn_minion_packet(g_minionindex - 1, g_minionindex, wave_count);
			g_minionindex++;
			//주기적인 스폰
			add_timer(user_id, OP_SPAWN, 1000);
		}
		break;
		case OP_SPAWN:
		{
			g_minion[g_minionindex].Pos.x = 50.f + g_minionindex * 100;
			g_minion[g_minionindex].Pos.y = 0.f;
			g_minion[g_minionindex].Pos.z = 4150.f;

			//send_spawn_minion_packet(g_minionindex);			
			g_minionindex++;
			//주기적인 스폰
			add_timer(user_id, OP_SPAWN, 10000);
		}
		break;
		}
	}

}