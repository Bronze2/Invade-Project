#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Mesh.h"
#include "Material.h"
class CCollider2D :
    public CComponent
{
private:
    static UINT g_iColID;

    Vec3 m_vOffsetPos;
    Vec3 m_vOffsetScale;
    COLLIDER2D_TYPE m_eType;
    int m_iCollisionCount;
    Ptr<CMesh> m_pColMesh;
    Ptr<CMaterial> m_pColMtrl;

    Matrix m_matColWorld;
    UINT m_iColID;

public:
    virtual void Update();
    virtual void FinalUpdate();
    void Render();

    void OnCollisionEnter(CCollider2D* _pOther);
    void OnCollision(CCollider2D* _pOther);
    void OnCollisionExit(CCollider2D* _pOther);

    void SetOffsetPos(const Vec3& _vPos) { m_vOffsetPos = _vPos; }
    void SetOffsetScale(const Vec3& _vScale) { m_vOffsetScale = _vScale; }

    const Vec3& GetOffsetPos() { return m_vOffsetPos; }
    const Vec3& GetOffsetScale() { return m_vOffsetScale; }
    COLLIDER2D_TYPE GetColliderType() { return m_eType; }
    void SetCollider2DType(COLLIDER2D_TYPE _eType);
    UINT GetColID() { return m_iColID; }
    const Matrix& GetColliderWorldMat() { return m_matColWorld; }

    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
    void operator=(const CCollider2D* _pOther);
    CCollider2D();
    CCollider2D(const CCollider2D& _Other);
    virtual ~CCollider2D();

    CLONE(CCollider2D);
};

