#pragma once
#include "Script.h"
class CWindSkill0Script :
	public CScript
{
private:
	CGameObject* m_pTarget;
	bool m_bStart;
	CGameObject* m_pParticle;


	float m_fSpeed;
	Vec3 m_vDir;
	Vec3 m_vStartPos;
	float m_fLength;
	ELEMENT_TYPE m_iType;


	SKILL* m_tSkill;
	CGameObject* m_pPlayer;

public:


	CWindSkill0Script();
	virtual ~CWindSkill0Script();

	virtual void Update();
	virtual void OnCollision3DEnter(CCollider3D* _pColldier);
	
	CLONE(CWindSkill0Script)



};
