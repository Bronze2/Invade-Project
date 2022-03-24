#pragma once
#include "Singleton.h"


class Server : public Singleton<Server>
{
public:
	Server();
	~Server();

	void Initialize();
	void AcceptMMServer();
	void Run();


public:
	

private:

};

