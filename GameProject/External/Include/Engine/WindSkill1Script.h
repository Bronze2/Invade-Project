#pragma once
#include "Script.h"
class CWindSkill1Script :
	public CScript
{
private:
	bool m_bTickCheck;
	CGameObject* m_pParticle;
	float m_fLength;


	float m_fSpeed;
	Vec3 m_vDir;
	Vec3 m_vStartPos;
	float m_fTime;


	bool m_bStart;
	ELEMENT_TYPE m_iType;


	SKILL* m_tSkill;
public:
	CWindSkill1Script();
	virtual ~CWindSkill1Script();

	void SetStartPos(const Vec3& _Pos) { m_vStartPos = _Pos; }
	virtual void Update();

	virtual void OnCollision3DEnter(CCollider3D* _pColldier);
	CLONE(CWindSkill1Script)


};

