#pragma once
#include "Script.h"

#include "Mesh.h"
#include "Material.h"

class CCrossHairScript
	: public CScript
{
private:
	Vec3 m_vDir;
	Vec3 m_vPos;

	CGameObject* m_pMainCam;

public:
	void Init();
	virtual void Update();

	const Vec3& GetDir() { return m_vDir; }
	const Vec3& GetPos() { return m_vPos; }

	void SetMainCam(CGameObject* _cam) { m_pMainCam = _cam; }

public:
	CCrossHairScript();
	virtual ~CCrossHairScript();

	CLONE(CCrossHairScript);
};

