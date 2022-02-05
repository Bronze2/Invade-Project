#include "pch.h"
#include "MeshRender.h"

#include "Transform.h"
#include "ResMgr.h"
#include "Animator3D.h"


CMeshRender::CMeshRender()
	: CComponent(COMPONENT_TYPE::MESHRENDER),m_bDynamicShadow(false)
{
	m_vecMtrl.resize(1);
}

CMeshRender::~CMeshRender()
{
}


ULONG64 CMeshRender::GetInstID(UINT _iMtrlIdx)
{
	if (m_pMesh == NULL || m_vecMtrl[_iMtrlIdx] == NULL)
		return 0;
	uInstID id{ m_pMesh->GetID(),(WORD)m_vecMtrl[_iMtrlIdx]->GetID(),(WORD)_iMtrlIdx };
	return id.IIID;
}

void CMeshRender::Render()
{
	if (IsActive() == false || nullptr == m_pMesh)
		return;
	int a = 1;
	for (size_t i = 0; i < m_vecMtrl.size(); ++i)
	{
		if (nullptr == m_vecMtrl[i] || nullptr == m_vecMtrl[i]->GetShader())
			continue;

		
		Transform()->UpdateData();
		if (Animator3D())
		{
			Animator3D()->UpdateData();
	
			a = 1;
			m_vecMtrl[i]->SetData(SHADER_PARAM::INT_0, &a); // Animation Mesh ¾Ë¸®±â
		}
		m_vecMtrl[i]->UpdateData();
		m_pMesh->Render((UINT)i);
		a = 0;
		m_vecMtrl[i]->SetData(SHADER_PARAM::INT_0, &a);
	}
}



void CMeshRender::Render_ShadowMap()
{
	int a = 1;
	Ptr<CMaterial>pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");
	for (UINT i = 0; i < m_pMesh->GetSubsetCount(); ++i) {
		if (Animator3D()) {
			Animator3D()->UpdateData();
			pMtrl->SetData(SHADER_PARAM::INT_0, &a);
		}
		Transform()->UpdateData();
		pMtrl->UpdateData();
		m_pMesh->Render(i);
	}
	if (Animator3D()) {
		a = 0;
		pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	}
}


Ptr<CMaterial> CMeshRender::GetCloneMaterial(UINT _iSubSet)
{
	if (nullptr == m_vecMtrl[_iSubSet])
		return nullptr;

	m_vecMtrl[_iSubSet] = m_vecMtrl[_iSubSet]->Clone();
	return m_vecMtrl[_iSubSet];
}

void CMeshRender::SetMaterial(Ptr<CMaterial> _pMtrl, UINT _iSubset)
{
	if (m_vecMtrl.size() <= (size_t)_iSubset)
		m_vecMtrl.resize(_iSubset + 1);

	m_vecMtrl[_iSubset] = _pMtrl;
}

void CMeshRender::SaveToScene(FILE* _pFile)
{
}

void CMeshRender::LoadFromScene(FILE* _pFile)
{
}