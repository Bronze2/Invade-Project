#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"
class CLight3D :
    public CComponent
{
private:
    tLight3D m_tLightInfo;
    Ptr<CMesh> m_pVolumeMesh;
    Ptr<CMaterial> m_pLightMtrl;
    int m_iArrIdx;
    CGameObject* m_pCamObj;
    
public:
    void SetLightType(LIGHT_TYPE _eType);
    void SetLightPos(const Vec3& _vPos);
    void SetDiffuseColor(const Vec3& _vDiffuse) { m_tLightInfo.tColor.vDiff = _vDiffuse; }
    void SetSpecular(const Vec3& _VSpec) { m_tLightInfo.tColor.vSpec = _VSpec; }
    void SetAmbient(const Vec3& _vAmb) { m_tLightInfo.tColor.vAmb = _vAmb; }
    void SetLightDir(const Vec3& _vDir);
    void SetLightRange(float _fRange) { m_tLightInfo.fRange = _fRange; }
    const tLight3D& GetLight3DInfo() { return m_tLightInfo; }

    virtual void FinalUpdate();

    void Render();
    void Render_ShadowMap();
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

    CLight3D();
    CLight3D(const CLight3D& _Light);
    virtual ~CLight3D();

    CLONE(CLight3D)


};

