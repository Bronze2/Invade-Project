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
    int m_id;

    PROJECTILE_TYPE m_eProjectileType;
public:
    void SetProjectileType(PROJECTILE_TYPE _Type) { m_eProjectileType = _Type; }
    void SetMatrixObject(const Matrix& _Matrix) { m_matObjectWorldMatrix = _Matrix; }
    \
        void SetTargetPos(const Vec3& _Pos) { m_vTargetPos = _Pos; }

    void SetObject(CGameObject* _pGameObject) { m_pObject = _pGameObject; }
    virtual void Update();
    void SetSpeed(const float& _Speed) { m_fSpeed = _Speed; }

    void SetDamage(const UINT& _Damage) { m_uiDamage = _Damage; }

    void SetBone(const UINT& _iBone) { m_iBone = _iBone; }
    void SetStartFrame(const UINT& _iStartFrame) { m_iStartFrame = _iStartFrame; }
    void SetDir(const Vec3& _Dir) { m_vDir = _Dir; }

    virtual void OnCollision3DEnter(CCollider3D* _pOther);

    void Init();

    void m_SetId(int id) { m_id = id; };
    int m_GetId() { return m_id; };

    CProjectileScript();
    virtual ~CProjectileScript();
    CLONE(CProjectileScript);
};