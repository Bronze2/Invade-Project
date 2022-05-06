#pragma once
#include "Script.h"

enum class PROJECTILE_TYPE {
    MINION,
    TOWER,
};
class CProjectileScript :
    public CScript
{
private:
    float m_fSpeed;
    UINT m_uiDamage;
    CGameObject* m_pObject;
    float m_fAlpha;
    UINT m_iBone;
    UINT m_iStartFrame;
    Vec3 m_vTargetPos;
    bool m_bRotate;
    Vec3 m_vDir;
    Matrix m_matObjectWorldMatrix;
    Vec3 m_vStartPos;

    PROJECTILE_TYPE m_eProjectileType;

    float m_fLength;
    CGameObject* m_pTarget;

    bool m_bUpdate;
    UINT m_bLaunch;//0 not ¹ß»ç //1 ¹ß»ç È¸Àü// 2 ¾Ë¾Æ¼­

    float m_fRoty;
    int m_id;

public:
    void m_SetId(int id) { m_id = id; }

    void SetRotY(float _fRotY) { m_fRoty = _fRotY; }
    void SetLaunch(UINT _bLaunch) { m_bLaunch = _bLaunch; }
    void SetTarget(CGameObject* _pTarget) { m_pTarget = _pTarget; }
    bool M_FLengthCheck(const Vec3& _Pos);
    void SetStartPos(const Vec3& _Pos) { m_vStartPos = _Pos; }
    void SetProjectileType(PROJECTILE_TYPE _Type) { m_eProjectileType = _Type; }
    void SetMatrixObject(const Matrix& _Matrix) { m_matObjectWorldMatrix = _Matrix; }
        void SetTargetPos(const Vec3& _Pos) { m_vTargetPos = _Pos; }

    void SetObject(CGameObject* _pGameObject) { m_pObject = _pGameObject; }
    virtual void Update();
    void SetSpeed(const float& _Speed) { m_fSpeed = _Speed; }

    void SetDamage(const UINT& _Damage) { m_uiDamage = _Damage; }

    void SetBone(const UINT& _iBone) { m_iBone = _iBone; }
    void SetStartFrame(const UINT& _iStartFrame) { m_iStartFrame = _iStartFrame; }

    virtual void OnCollision3DEnter(CCollider3D* _pOther);

    void Init();


    CProjectileScript();
    virtual ~CProjectileScript();
    CLONE(CProjectileScript);
};
