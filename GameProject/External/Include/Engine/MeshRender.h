#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
class CMeshRender :
    public CComponent
{
private:
    Ptr<CMesh> m_pMesh;
    
    vector<Ptr<CMaterial>>  m_vecMtrl;

    bool m_bDynamicShadow;
public:
    void SetMesh(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; }
    void SetMaterial(Ptr<CMaterial> _pMtrl,UINT _iSubset=0);

    void SetDynamicShadow(bool _bTrue) { m_bDynamicShadow = _bTrue; }
    bool IsDynamicShadow() { return m_bDynamicShadow; }

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetCloneMaterial(UINT _iSubset=0);
    Ptr<CMaterial> GetSharedMaterial(UINT _iSubset = 0) { return m_vecMtrl[_iSubset]; };

    UINT GetMtrlCount() { return(UINT)m_vecMtrl.size(); }
    ULONG64 GetInstID(UINT _iMtrlIdx);

    void Render();
    void Render(UINT _iMtrlIdx);
    void Render_ShadowMap();
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

   
public:
    CMeshRender();
    virtual ~CMeshRender();
    CLONE(CMeshRender)
};

