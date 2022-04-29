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
public:
    bool M_FLengthCheck(const Vec3& _Pos);
    void SetStartPos(const Vec3& _Pos) { m_vStartPos = _Pos; }
    void SetProjectileType(PROJECTILE_TYPE _Type) { m_eProjectileType = _Type; }
    void SetMatrixObject(const Matrix& _Matrix) { m_matObjectWorldMatrix = _Matrix; }
\
    void SetTargetPos(const Vec3& _Pos) { m_vTargetPos = _Pos; }

    void SetObject( CGameObject* _pGameObject) { m_pObject = _pGameObject; }
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

