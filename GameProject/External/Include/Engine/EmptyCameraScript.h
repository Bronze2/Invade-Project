#pragma once
#include "Script.h"

class CEmptyCameraScript
	: public CScript
{
private:
	Vec3 m_vShootPos;
	Vec3 m_vShootDir;

	bool m_bCheckStartMousePoint;
	float m_fDegree;

public:
	CEmptyCameraScript();
	virtual ~CEmptyCameraScript();

	void Init();
	virtual void Update();

	Vec3& GetShootPos() { return m_vShootPos; }
	Vec3& GetShootDir() { return m_vShootDir; }

	CLONE(CEmptyCameraScript);
};