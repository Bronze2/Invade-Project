#pragma once
#include "Resource.h"
#include "FBXLoader.h"
#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"
class CGameObject;
class CMeshData :
    public CResource
{
private:
    Ptr<CMesh> m_pMesh;
    vector<Ptr<CMaterial>> m_vecMtrl;

public:
    static CMeshData* LoadFromFBX(const wstring& _strFilePath);
    virtual void Load(const wstring& _strFilePath);
    virtual void Save(const wstring& _strFilePath);

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    vector<Ptr<CMaterial>> GetMaterial() { return m_vecMtrl; }

    CGameObject* Instantiate();

    CMeshData();
    virtual ~CMeshData();
};

