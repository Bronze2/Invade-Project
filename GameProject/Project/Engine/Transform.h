#pragma once
#include "Component.h"



class CTransform :
    public CComponent
{
private:
    Vec3 m_vLocalPos;
    Vec3 m_vLocalScale;
    Vec3 m_vLocalRot;

    Vec3 m_vLocalDir[(UINT)DIR_TYPE::END];
    Vec3 m_vWorldDir[(UINT)DIR_TYPE::END];
    Matrix m_matWorld;
    Matrix m_matWorldInv;
    Vec4 m_vQuaternion;
    CGameObject* m_pCamera;
    bool m_bBillBoard;

public:
    const Vec4 GetQuaternion()const { return m_vQuaternion; }
    void SetQuaternion(const Vec4& _vQuaternion) { m_vQuaternion = _vQuaternion; }
  
    const Vec3 GetLocalPos() { return m_vLocalPos; }
    void SetBillBoard(bool _bBillBoard) { m_bBillBoard = _bBillBoard; }
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }
    const Vec3& GetLocalScale() { return m_vLocalScale; }
    Vec3 GetWorldScale();
    const Vec3& GetLocalRot() { return m_vLocalRot; }
    const Matrix& GetWorldMat() { return m_matWorld; }
    void SetWorldMat(const Matrix& _mMatrix) { m_matWorld = _mMatrix; }
    const Vec3& GetLocalDir(DIR_TYPE _eType) { return m_vLocalDir[(UINT)_eType]; }
    const Vec3& GetWorldDir(DIR_TYPE _eType) { return m_vWorldDir[(UINT)_eType]; }
    void SetLocalPos(const Vec3& _vPos) { if (m_vLocalPos != _vPos) { m_vLocalPos = _vPos; Changed(); } }
    void SetLocalScale(const Vec3& _vScale) { if (m_vLocalScale != _vScale) { m_vLocalScale = _vScale; Changed(); } }
    void SetLocalRot(const Vec3& _vRot) { if (m_vLocalRot != _vRot) { m_vLocalRot = _vRot; Changed(); } }
    void LookAt(const Vec3& _vLook);
    void SetWorldDir(DIR_TYPE _eType, Vec3 _vDir) { m_vWorldDir[(UINT)_eType] = _vDir; }
    void LookAt(const Vec3& _vLook, const Vec3& _vRot);
   // const Vec3 GetLocalDir() { return m_vLocalDir; }
   
    Matrix LookAt(const Vec3& Eye, const Vec3& target, const Vec3& _Up);

    void SetCamera(CGameObject* _pCamera) { m_pCamera = _pCamera; }

    bool IsCasting(const Vec3& _vPos);
    float GetMaxScale();
    void UpdateData();
    virtual void FinalUpdate();
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
    virtual CTransform* Clone() { return new CTransform(*this); }
    CTransform();
    virtual ~CTransform();
};


