#pragma once
#include "Component.h"
#include "Transform.h"
#include "Collider2D.h"
#include "Animator2D.h"
#include "Animation2D.h"
#include "EventMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include"ResMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
enum class SCRIPT_TYPE {
    ARROWSCRIPT,
    MONSTERSCRIPT,
    PLAYERSCRIPT,
    MINIONSCRIPT,
    CAMERASRIPT,
    END,
};


class CScript :
    public CComponent
{
public:
    UINT m_iScriptType;

public:
    virtual void Update() = 0;
    static void CreateObject(CGameObject* _pNewObject, int _iLayerIdx);
    static void CreateObject(CGameObject* _pNewObject, const wstring& _strLayerName);

    static void DeleteObject(CGameObject* _pDeleteObject);
    void AddChild(CGameObject* _pChildObject);
    static void AddChild(CGameObject* _pParent, CGameObject* _pChild);
    void ClearParent();
    static void ClearParent(CGameObject* _pTarget);

    void TransferLayer(const wstring& _strLayerName, bool _bMoveAll);
    void TransferLayer(int _iLayerIdx, bool _bMoveAll);
    static void TransferLayer(CGameObject* _pTarget, const wstring& _strLayerName, bool _bMoveAll);
    static void TransferLayer(CGameObject* _pTarget, int _iLayerIdx, bool _bMoveAll);

    static void EnableObject(CGameObject* _pTarget);
    static void Disable(CGameObject* _pTarget);

protected:
    virtual void OnCollisionEnter(CCollider2D* _pOther){}
    virtual void OnCollision(CCollider2D* _pOther){}
    virtual void OnCollisionExit(CCollider2D* _pOther){}
    
    virtual void OnCollision3DEnter(CCollider3D* _pOther) {}
    virtual void OnCollision3D(CCollider3D* _pOther) {}
    virtual void OnCollision3DExit(CCollider3D* _pOther) {}
public:
    virtual void SaveToScene(FILE* _pFile){}
    virtual void LoadFromScene(FILE* _pFile){}

    CScript(UINT _iScriptType);
    virtual ~CScript();
    friend class CCollider2D;
    friend class CCollider3D;
};

