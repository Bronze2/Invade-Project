#pragma once
#define MAX_SPAWN 2
#define BOX_SPWANTIME 5

#include <random>
#include <ctime>
#include <iostream>
#include <functional>

enum class BOX_SITUATION {

	eSPAWN,
	eDELETE,
	eWAIT,

};
struct SPAWN_LOCATE {
	bool bSpawn;
	Vec3 vPos;
};

class CSpawnMgr
{

private:
	vector<SPAWN_LOCATE> m_vSpawn;
	UINT m_uiSum;

	BOX_SITUATION m_bFirstBox;
	BOX_SITUATION m_bSecondBox;
	std::chrono::system_clock::time_point m_tFirstStart;
	std::chrono::system_clock::time_point m_tSecondStart;

	mt19937 engine;

public:

	void net_CreateBox(int id, int buff);
	vector<SPAWN_LOCATE> GetSpawnArray() { return m_vSpawn; }
	void SetSpawnFalse(int _id) { m_vSpawn[_id].bSpawn = false; }
	void Init();
	void Update();
	void CheckSum();
	void SetRandom();


	void SubSum() {
		if (m_uiSum < 0)
			assert(nullptr);
		m_uiSum -= 1;
		if (BOX_SITUATION::eSPAWN != m_bFirstBox) {
			m_bSecondBox = BOX_SITUATION::eDELETE;
		}
		else {
			m_bFirstBox = BOX_SITUATION::eDELETE;
		}
	}
	void AddSum() {
		if (m_uiSum > MAX_SPAWN)
			assert(nullptr);

		m_uiSum += 1;
	}
	SINGLE(CSpawnMgr)


};
