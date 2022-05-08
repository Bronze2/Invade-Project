#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Mesh.h"
#include "Material.h"
class CCollider3D :
    public CComponent
{
private:
    static UINT g_iColID;

    Vec3 m_vOffsetPos;
    Vec3 m_vOffsetScale;
    COLLIDER3D_TYPE m_eType;
    int m_iCollisionCount;
    Ptr<CMesh> m_pColMesh;
    Ptr<CMaterial> m_pColMtrl;

    Matrix m_matColWorld;
    UINT m_iColID;
    BoundingBox m_bBound;

public:
    virtual void Update();
    virtual void FinalUpdate();

    BoundingBox GetBox() { return m_bBound; }
    void Render();

    void OnCollisionEnter(CCollider3D* _pOther);
    void OnCollision(CCollider3D* _pOther);
    void OnCollisionExit(CCollider3D* _pOther);
    void SetOffsetPos(const Vec3& _vPos) { m_vOffsetPos = _vPos; }
    void SetOffsetScale(const Vec3& _vScale) { m_vOffsetScale = _vScale; }

    const Vec3& GetOffsetPos() { return m_vOffsetPos; }
    const Vec3& GetOffsetScale() { return m_vOffsetScale; }
    COLLIDER3D_TYPE GetColliderType() { return m_eType; }
    void SetCollider3DType(COLLIDER3D_TYPE _eType);
    UINT GetColID() { return m_iColID; }
    const Matrix& GetColliderWorldMat() { return m_matColWorld; }

    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
    void operator=(const CCollider3D* _pOther);
    CCollider3D();
    CCollider3D(const CCollider3D& _Other);
    virtual ~CCollider3D();

    CLONE(CCollider3D);
};