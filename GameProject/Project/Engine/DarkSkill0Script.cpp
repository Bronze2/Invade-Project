#include "pch.h"
#include "DarkSkill0Script.h"

#include "PlayerScript.h"
#include "MeshRender.h"
#include "StaticUI.h"
#include "RenderMgr.h"

CDarkSkill0Script::CDarkSkill0Script() :CScript((UINT)SCRIPT_TYPE::DARKSKILL0), m_bStart(false), m_bTickCheck(false)
{
}

CDarkSkill0Script::~CDarkSkill0Script()
{
}

void CDarkSkill0Script::Awake() 
{
	m_pDarkUI = new CGameObject;
	m_pDarkUI->SetName(L"UIDark");
	m_pDarkUI->FrustumCheck(false);
	m_pDarkUI->AddComponent(new CTransform);
	m_pDarkUI->AddComponent(new CMeshRender);
	m_pDarkUI->AddComponent(new CStaticUI);

	tResolution res = CRenderMgr::GetInst()->GetResolution();

	CGameObject* pUICam = dynamic_cast<CGameObject*>(CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->GetParentObj()[5]);

	m_pDarkUI->Transform()->SetLocalPos(Vec3(0.f, 0.f, 1.f));
	m_pDarkUI->Transform()->SetLocalScale(Vec3(res.fWidth, res.fHeight, 1.f));
	m_pDarkUI->StaticUI()->SetCamera(pUICam->Camera());

	m_pDarkUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pDarkUIMtrl = new CMaterial;
	pDarkUIMtrl->DisableFileSave();
	pDarkUIMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"DarkTexShader"));
	m_pDarkUI->MeshRender()->SetMaterial(pDarkUIMtrl);
	Ptr<CTexture> pDarkUITex = CResMgr::GetInst()->FindRes<CTexture>(L"DarkUITex");
	m_pDarkUI->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pDarkUITex.GetPointer());

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pDarkUI);

	m_pDarkUI->SetActive(false);
}

void CDarkSkill0Script::Update()
{
	if (nullptr == m_pSkill)return;
	if (nullptr != m_pTarget->GetScript<CPlayerScript>()) {
		if (!m_bStart) {
			m_bStart = true;
			m_tStart = clock();
		}
		else {
			m_tEnd = clock();
			m_tInterval = m_tEnd - m_tStart / CLOCKS_PER_SEC;
			if (m_tInterval >= m_pSkill->fCoolTime) {
				m_bStart = false;
				m_bTickCheck = false;
				DeleteObject(GetObj());
			}
		}
	}

	m_pDarkUI->SetActive(true);

}
