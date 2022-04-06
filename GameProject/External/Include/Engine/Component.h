#pragma once
#include "Entity.h"
#include "GameObject.h"
class CComponent :
    public CEntity
{
private: 
    CGameObject* m_pObject;
    COMPONENT_TYPE m_eComType;
    bool m_bChange;
    bool m_bActive;
public:
    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update(){}
    virtual void LateUpdate(){}
    virtual void FinalUpdate(){}
    virtual void Enable() {}
    virtual void disable(){}
    virtual void SaveToScene(FILE* _pFile) = 0;
    virtual void LoadFromScene(FILE* _pFile) = 0;

    void SetActive(bool _bTrue);
    bool IsActive() { return m_bActive; }

    COMPONENT_TYPE GetComponentType() { return m_eComType; }
    CGameObject* GetObj() { return m_pObject; }
    CTransform* Transform() { return m_pObject->Transform(); }
    CMeshRender* MeshRender() { return m_pObject->MeshRender(); }
    CAnimator2D* Animator2D() { return m_pObject->Animator2D(); }
    CAnimator3D* Animator3D() { return m_pObject->Animator3D(); }
    CCollider2D* Collider2D() { return m_pObject->Collider2D(); }
    CCollider3D* Collider3D() { return m_pObject->Collider3D(); }
    CLight2D* Light2D() { return m_pObject->Light2D(); }
    CLight3D* Light3D() { return m_pObject->Light3D(); }
    CCamera* Camera() { return m_pObject->Camera(); }
    CSensor* Sensor() { return m_pObject->Sensor(); }

private:
    void SetGameObject(CGameObject* _pObject) { m_pObject = _pObject; }

protected:
    void Changed() { m_bChange = true; }
public:
    bool IsChanged() { return m_bChange; }

    virtual CComponent* Clone() = 0;

    CComponent(COMPONENT_TYPE _eType);
    virtual ~CComponent();
    friend class CGameObject;
    friend class CEventMgr;
    friend void ComponentSetChange(CComponent*, bool);


};

