#pragma once
#include "Script.h"

enum class BOX_STATE {
    CLOSE,
    OPEN,
    GOAWAY,
    
};

class CBoxScript :
    public CScript
{
private:

    BOX_STATE m_bOpen;
    BUFF_TYPE m_eBuffType;
    CGameObject* m_pUpObj;
    UINT m_iIdx;
    float m_fUpObjPrevRot;
    CGameObject* m_pPlayer;
    CGameObject* m_pBuffParticle;
    Ptr<CSound> m_pSound;
    bool m_bParticle;
    CGameObject* m_pBoxCol;
public:
    void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
    void SetBoxCol(CGameObject* _pObj) { m_pBoxCol = _pObj; }
    void Init();
    UINT GetIdx() { return m_iIdx; }
    void SetIdx(const UINT& _iIdx) { m_iIdx = _iIdx; }
    BUFF_TYPE GiveBuff();
   
    void SetUp(CGameObject* _pObj) { m_pUpObj = _pObj; }
    BOX_STATE GetOpen() { return m_bOpen; }
    virtual void Update();
    void SetOpen(const UINT& _bOpen);
   CLONE(CBoxScript)


   CBoxScript();
    virtual ~CBoxScript();
    virtual void OnCollision3D(CCollider3D* _pOther);
};

