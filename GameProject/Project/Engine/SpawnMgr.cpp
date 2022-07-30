#include "pch.h"
#include "SpawnMgr.h"


void CSpawnMgr::Init()
{
	SPAWN_LOCATE spawn1;
	spawn1.bSpawn = false;
	spawn1.vPos = Vec3(0.f, 0.f, 100.f);
	m_vSpawn.push_back(spawn1);

	SPAWN_LOCATE spawn2;
	spawn2.bSpawn = false;
	spawn2.vPos = Vec3(0.f, 0.f, 200.f);
	m_vSpawn.push_back(spawn2);

	SPAWN_LOCATE spawn3;
	spawn3.bSpawn = false;
	spawn3.vPos = Vec3(0.f, 0.f, 300.f);
	m_vSpawn.push_back(spawn3);

	SPAWN_LOCATE spawn4;
	spawn4.bSpawn = false;
	spawn4.vPos = Vec3(0.f, 0.f, 400.f);
	m_vSpawn.push_back(spawn4);
	SPAWN_LOCATE spawn5;
	spawn5.bSpawn = false;
	spawn5.vPos = Vec3(0.f, 0.f, 500.f);
	m_vSpawn.push_back(spawn5);
	SPAWN_LOCATE spawn6;
	spawn6.bSpawn = false;
	spawn6.vPos = Vec3(0.f, 0.f, 600.f);
	m_vSpawn.push_back(spawn6);
	SPAWN_LOCATE spawn7;
	spawn7.bSpawn = false;
	spawn7.vPos = Vec3(0.f, 0.f, 700.f);
	m_vSpawn.push_back(spawn7);

	SPAWN_LOCATE spawn8;
	spawn8.bSpawn = false;
	spawn8.vPos = Vec3(0.f, 0.f, 800.f);
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
			auto generator = bind(distribution, engine);
//			auto generator = rand() % 7;
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
			auto generator = bind(distribution, engine);
//			auto generator = rand() % 7;
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