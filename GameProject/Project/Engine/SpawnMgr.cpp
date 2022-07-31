#include "pch.h"
#include "SpawnMgr.h"


void CSpawnMgr::Init()
{
	SPAWN_LOCATE spawn1;
	spawn1.bSpawn = false;
	spawn1.vPos = Vec3(2000.f, 0.f, 2600.f);
	m_vSpawn.push_back(spawn1);

	SPAWN_LOCATE spawn2;
	spawn2.bSpawn = false;
	spawn2.vPos = Vec3(-200.f, 0.f, 1700.f);
	m_vSpawn.push_back(spawn2);

	SPAWN_LOCATE spawn3;
	spawn3.bSpawn = false;
	spawn3.vPos = Vec3(-160.f, 0.f, 10800.f);
	m_vSpawn.push_back(spawn3);

	SPAWN_LOCATE spawn4;
	spawn4.bSpawn = false;
	spawn4.vPos = Vec3(3900.f, 0.f, 13000.f);
	m_vSpawn.push_back(spawn4);

	SPAWN_LOCATE spawn5;
	spawn5.bSpawn = false;
	spawn5.vPos = Vec3(-1600.f, 0.f, 4500.f);
	m_vSpawn.push_back(spawn5);

	SPAWN_LOCATE spawn6;
	spawn6.bSpawn = false;
	spawn6.vPos = Vec3(1500.f, 0.f, 9800.f);
	m_vSpawn.push_back(spawn6);

	SPAWN_LOCATE spawn7;
	spawn7.bSpawn = false;
	spawn7.vPos = Vec3(-380.f, 0.f, 7580.f);
	m_vSpawn.push_back(spawn7);

	SPAWN_LOCATE spawn8;
	spawn8.bSpawn = false;
	spawn8.vPos = Vec3(2500.f, 0.f, 16000.f);
	m_vSpawn.push_back(spawn8);

	engine = mt19937((unsigned int)time(NULL));
	uniform_int_distribution<int> distribution(0, 7);
	auto generator = bind(distribution, engine);

	while (m_uiSum < 2)
	{
		int a = generator();
		if (!m_vSpawn[a].bSpawn)
		{
			m_vSpawn[a].bSpawn = true;
			m_uiSum += 1;
			CreateBoxObject(m_vSpawn[a].vPos, a);
		}
	}

}

void CSpawnMgr::Update()
{
	CheckSum();

}

void CSpawnMgr::CheckSum()
{
	if (BOX_SITUATION::eDELETE == m_bFirstBox) {
		m_bFirstBox = BOX_SITUATION::eWAIT;
		m_tFirstStart = std::chrono::system_clock::now();
	}
	if (BOX_SITUATION::eWAIT == m_bFirstBox) {
		if (CoolTimeCheck(m_tFirstStart, BOX_SPWANTIME)) {
			m_bFirstBox = BOX_SITUATION::eSPAWN;
			uniform_int_distribution<int> distribution(0, 7);
			engine = mt19937((unsigned int)time(NULL));
			auto generator = bind(distribution, engine);
			while (true)
			{
				int a = generator();
				if (!m_vSpawn[a].bSpawn)
				{
					m_vSpawn[a].bSpawn = true;
					m_uiSum += 1;
					CreateBoxObject(m_vSpawn[a].vPos, a);
					break;
				}
			}
		}
	}

	if (BOX_SITUATION::eDELETE == m_bSecondBox) {
		m_bSecondBox = BOX_SITUATION::eWAIT;
		m_tSecondStart = std::chrono::system_clock::now();
	}
	if (BOX_SITUATION::eWAIT == m_bSecondBox) {
		if (CoolTimeCheck(m_tSecondStart, BOX_SPWANTIME)) {
			m_bSecondBox = BOX_SITUATION::eSPAWN;
			uniform_int_distribution<int> distribution(0, 7);
			engine = mt19937((unsigned int)time(NULL));
			auto generator = bind(distribution, engine);
			while (true)
			{
				int a = generator();
				if (!m_vSpawn[a].bSpawn)
				{
					m_vSpawn[a].bSpawn = true;
					m_uiSum += 1;
					CreateBoxObject(m_vSpawn[a].vPos, a);
					break;
				}
			}
		}
	}
}



CSpawnMgr::CSpawnMgr() {

}
CSpawnMgr::~CSpawnMgr() {}