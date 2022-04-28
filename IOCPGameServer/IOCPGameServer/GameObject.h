#pragma once
#include "Entity.h"

class CComponent;
class CTransform;
class CCollider2D;
class CCollider3D;
class CAnimator2D;
class CAnimator3D;
class CScript;
class CSensor;
class CGameObject :public CEntity
{
private:
	CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];
	vector<CScript*> m_vecScript;
	vector<CGameObject*> m_vecChild;
	CGameObject* m_pParentObj;
	int m_iLayerIdx;
	bool m_bDead;
	bool m_bActive;
	bool m_bFrustumCheck;
	bool m_bFallDown = false;
	bool m_id;

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Enable();
	void Disable();

	void SetActive(bool _bTrue);
	bool IsActive() { return m_bActive; }
	void FrustumCheck(bool _bCheck) { m_bFrustumCheck = _bCheck; }
	bool GetFrustumCheck() { return m_bFrustumCheck; }
	void SetId(int id) { m_id = id; }
	int  GetId() { return m_id; }
	void AddComponent(CComponent* _pCom);
	CComponent* GetComponent(COMPONENT_TYPE _eType) { assert(_eType != COMPONENT_TYPE::SCRIPT); return m_arrCom[(UINT)_eType]; }
	CTransform* Transform() { return (CTransform*)m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]; }
	CCollider2D* Collider2D() { return(CCollider2D*)m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER2D]; }
	CCollider3D* Collider3D() { return(CCollider3D*)m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER3D]; }
	CAnimator2D* Animator2D() { return(CAnimator2D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D]; }
	CAnimator3D* Animator3D() { return(CAnimator3D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR3D]; }
	CSensor* Sensor() { return (CSensor*)m_arrCom[(UINT)COMPONENT_TYPE::SENSOR]; }
	const vector<CScript*>& GetScripts()const { return m_vecScript; }

	template<typename T>
	T* GetScript();

	void AddChild(CGameObject* _pChildObj);
	bool IsAncestor(CGameObject* _pObj);
	void ClearParent(CGameObject* _pNextParent = nullptr);
	CGameObject* GetParent() { return m_pParentObj; }
	int GetLayerIdx() { return m_iLayerIdx; }
	const vector<CGameObject*>& GetChild() { return m_vecChild; }

	bool IsDead() { return m_bDead; }
	void SetDead();
	bool IsFallDown() { return m_bFallDown; }
	void SetFallDown();

	void RegisterToLayer();

	CLONE(CGameObject);

private:
	CGameObject(const CGameObject& _origin);
	void SetLayerIdx(int _iLayerIdx) { m_iLayerIdx = _iLayerIdx; }

public:
	CGameObject();
	virtual ~CGameObject();
	friend class CLayer;
	friend class CEventMgr;

};


template<typename T>
inline T* CGameObject::GetScript() {
	T* pScript = nullptr;
	for (size_t i = 0; i < m_vecScript.size(); ++i) {
		pScript = dynamic_cast<T*>(m_vecScript[i]);
		if (nullptr != pScript)
			return pScript;
	}
	return pScript;
}