#pragma once
#include "Script.h"
class CThunderSkill1Script :
	public CScript
{
private:
	SKILL* m_pSkill;

	CGameObject* m_pTarget;

	clock_t m_tStart;
	clock_t m_tTickStart;
	clock_t m_tEnd;
	clock_t m_tInterval;
	clock_t m_tTickInterval;
	bool m_bTickCheck;
	UINT m_iLayerIdx;
	bool m_bStart = false;
	CAMP_STATE m_eCampState;

	bool isMain = true;
public:

	void SetIsMain(bool m_isMain) { isMain = m_isMain; }
	void SetTarget(CGameObject* _pTarget) { m_pTarget = _pTarget; }
	void Collision();
	CThunderSkill1Script();
	virtual ~CThunderSkill1Script();

	void SetLayer(const UINT& _iLayerIdx) { m_iLayerIdx = _iLayerIdx; }

	virtual void Update();
	virtual void OnCollision3DEnter(CCollider3D* _pColldier);

	void SetCampState(const CAMP_STATE& _eState) { m_eCampState = _eState; }
	CLONE(CThunderSkill1Script)


};

