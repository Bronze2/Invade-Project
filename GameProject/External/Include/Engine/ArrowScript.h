#pragma once
#include "Script.h"
#include "PlayerScript.h"
class CArrowScript :
    public CScript
{
private: 
    float m_fSpeed;
    Vec3 m_vDir;
    Vec3 m_vStartPos;
    float m_fTime;
    float m_fVelocityY;
    float m_fVelocityX;
    float m_fVelocityZ;
    float a;
    ELEMENT_TYPE m_iType;
    CGameObject* m_pParticle;
    bool m_bMove;
public:
    virtual void Awake();
    virtual void Update();
    virtual void OnCollisionEnter(CCollider2D* _pOther);
    void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
    void SetDir(Vec3 _vDir) { m_vDir = _vDir; }
    void SetTime(float _fTime) { m_fTime = _fTime; }
    void SetStartPos(Vec3 _vPos) { m_vStartPos = _vPos; }
    void SetVelocityY(float _fVelocityY) { m_fVelocityY = _fVelocityY; }
    void SetVelocityX() { m_fVelocityX = m_vDir.x*m_fSpeed  * DT; }
    void SetVelocityZ() { m_fVelocityZ = m_vDir.z * m_fSpeed * DT; }
    void SetType(UINT _iType) { m_iType = (ELEMENT_TYPE)_iType; }
    void SetMove(bool _bMove) { m_bMove = _bMove; }
    void Init();
private:
  
public:
    CLONE(CArrowScript);

    CArrowScript(ELEMENT_TYPE _iType);
    virtual ~CArrowScript();
};

