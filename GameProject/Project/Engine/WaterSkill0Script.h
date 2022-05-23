#pragma once
#include "Script.h"
class CWaterSkill0Script :
 public CScript
{
private:
	SKILL* m_pSkill;

	CGameObject* m_pTarget;
	bool m_bStart;
	clock_t m_tStart;
	clock_t m_tTickStart;
	clock_t m_tEnd;
	clock_t m_tInterval;
	clock_t m_tTickInterval;
	bool m_bTickCheck;
	CGameObject* m_pParticle;


	float m_fSpeed;
	Vec3 m_vDir;
	Vec3 m_vStartPos;
	float m_fTime;
	float m_fVelocityY;
	float m_fVelocityX;
	float m_fVelocityZ;
	float a;
	ELEMENT_TYPE m_iType;
	bool m_bMove;
	float m_fFallSpeed;

	float m_fyPos = 0.f;
	float m_fxPos = 0.f;

	float m_fDotValue;
	bool m_bSetDotValue = false;

	bool m_bCheckDegree;

	SKILL* m_tSkill;
	CGameObject* m_pPlayer;
	UINT m_iCount;
public:
	void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
	void Init();
	void SetSkill(SKILL* _pSkill);
	virtual void Awake();
	void SetTarget(CGameObject* _pTarget) { m_pTarget = _pTarget; }
	CWaterSkill0Script();
	virtual ~CWaterSkill0Script();
	void SetMove(bool _bMove) { m_bMove = _bMove; }
	virtual void Update();
	virtual void OnCollision3DEnter(CCollider3D* _pColldier);

	
	CLONE(CWaterSkill0Script)


};

