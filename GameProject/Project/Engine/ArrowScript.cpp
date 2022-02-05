#include "pch.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "PlayerScript.h"
#include "ParticleSystem.h"

void CArrowScript::Awake()
{
	
	CScene* pCurScene=CSceneMgr::GetInst()->GetCurScene();
	m_pParticle = new CGameObject;
	m_pParticle->SetName(L"Particle");
	m_pParticle->AddComponent(new CTransform);
	switch (m_iType)
	{
	case ELEMENT_TYPE::FROZEN:
		m_pParticle->AddComponent(new CParticleSystem);
		m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Snow"));
		m_pParticle->ParticleSystem()->SetStartColor(Vec4(0.4f, 0.4f, 0.8f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
		m_pParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 1.f, 1.f, 1.0f));
		m_pParticle->ParticleSystem()->SetStartScale(5.f);
		m_pParticle->ParticleSystem()->SetEndScale(10.f);

		break;
	case ELEMENT_TYPE::FIRE:
		m_pParticle->AddComponent(new CParticleSystem);
		m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"smokeparticle"));
		m_pParticle->ParticleSystem()->SetStartColor(Vec4(1.f, 1.f, 0.f, 0.5f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
		m_pParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 0.f, 0.f, 1.0f));
		m_pParticle->ParticleSystem()->SetStartScale(5.f);
		m_pParticle->ParticleSystem()->SetEndScale(10.f);
		break;
	case ELEMENT_TYPE::DARK:

		break;
	case ELEMENT_TYPE::THUNDER:

		break;
	case ELEMENT_TYPE::WIND:

		break;


	}
	

	m_pParticle->FrustumCheck(false);
	m_pParticle->Transform()->SetLocalPos(Vec3(0.5f, 0.f, 0.f));

	pCurScene->FindLayer(L"Default")->AddGameObject(m_pParticle);

	GetObj()->AddChild(m_pParticle);
}


void CArrowScript::Update()
{

	if (!m_bMove)
		return;
	Vec3 vPos = Transform()->GetLocalPos();
	
	vPos.x +=m_fVelocityX;
	vPos.z += m_fVelocityZ;
	m_fVelocityY -= (GRAVITY * DT) / 100;
	vPos.y += m_fVelocityY;
	Transform()->SetLocalPos(vPos);

}

void CArrowScript::OnCollisionEnter(CCollider2D* _pOther)
{
	if (L"Monster Object" == _pOther->GetObj()->GetName())
		DeleteObject(GetObj());
}




CArrowScript::CArrowScript(ELEMENT_TYPE _iType):CScript((UINT)SCRIPT_TYPE::ARROWSCRIPT),m_iType(_iType),m_bMove(true)
{
}

CArrowScript::~CArrowScript()
{
}
