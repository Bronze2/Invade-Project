#pragma once
#include "Component.h"
#include "Transform.h"
#include "EventMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
enum class SCRIPT_TYPE {
    ARROWSCRIPT,
    MONSTERSCRIPT,
    PLAYERSCRIPT,
    MINIONSCRIPT,
    CAMERASRIPT,
    SPAWNSCRIPT,
    TOWERSCRIPT,
    BOWSCRIPT,
    EMPTYPLAYERSCRIPT,
    PROJECTILESCRIPT,
    OBJSTACLESCRIPT,
    END,
};


class CScript :
    public CComponent
{
public:
    UINT m_iScriptType;
    int index;
public:
    virtual void Update() = 0;
    static void CreateObject(CGameObject* _pNewObject, int _iLayerIdx);
    static void CreateObject(int index, CGameObject* _pNewObject, const wstring& _strLayerName);

    static void DeleteObject(CGameObject* _pDeleteObject);
    void AddChild(CGameObject* _pChildObject);
    static void AddChild(CGameObject* _pParent, CGameObject* _pChild);
    void ClearParent();
    static void ClearParent(CGameObject* _pTarget);
    void SetIndex(int m_index) { index = m_index; };
    void TransferLayer(int index ,const wstring& _strLayerName, bool _bMoveAll);
    void TransferLayer(int _iLayerIdx, bool _bMoveAll);
    static void TransferLayer(int index ,CGameObject* _pTarget, const wstring& _strLayerName, bool _bMoveAll);
    static void TransferLayer(CGameObject* _pTarget, int _iLayerIdx, bool _bMoveAll);

    static void EnableObject(CGameObject* _pTarget);
    static void Disable(CGameObject* _pTarget);

protected:
    virtual void OnCollisionEnter(CCollider2D* _pOther) {}
    virtual void OnCollision(CCollider2D* _pOther) {}
    virtual void OnCollisionExit(CCollider2D* _pOther) {}

    virtual void OnCollision3DEnter(CCollider3D* _pOther) {}
    virtual void OnCollision3D(CCollider3D* _pOther) {}
    virtual void OnCollision3DExit(CCollider3D* _pOther) {}


    virtual void OnDetectionEnter(CGameObject* _pOther) {}
    virtual void OnDetection(CGameObject* _pOther) {}
    virtual void OnDetectionExit(CGameObject* _pOther) {}

public:
    virtual void SaveToScene(FILE* _pFile) {}
    virtual void LoadFromScene(FILE* _pFile) {}

    CScript(UINT _iScriptType);
    virtual ~CScript();
    friend class CCollider2D;
    friend class CCollider3D;
    friend class CSensor;
};
