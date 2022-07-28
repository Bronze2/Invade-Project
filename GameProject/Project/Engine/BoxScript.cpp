#include "pch.h"
#include "BoxScript.h"
#include <random>
#include "PlayerScript.h"
#include <ctime>
#include <iostream>
#include <functional>
#include "SpawnMgr.h"
#include "Transform.h"
#include "ParticleSystem.h"
void CBoxScript::Init()
{
	mt19937 engine((unsigned int)time(NULL));
	uniform_int_distribution<int> distribution(0, (UINT)BUFF_TYPE::DAMAGE);
	auto generator = bind(distribution, engine);
	m_eBuffType = (BUFF_TYPE)generator();
	m_bOpen = BOX_STATE::CLOSE;


	m_pBuffParticle = new CGameObject;
	m_pBuffParticle->AddComponent(new CTransform);
	m_pBuffParticle->AddComponent(new CParticleSystem);
	m_pBuffParticle->SetActive(false);
	GetObj()->Transform()->GetLocalPos();
	m_pBuffParticle->Transform()->SetLocalPos(GetObj()->Transform()->GetLocalPos());
	switch (m_eBuffType)
	{
	case BUFF_TYPE::HEAL:
		m_pBuffParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"CartoonSmoke"), L"ParticleUpdate2Mtrl");
		m_pBuffParticle->ParticleSystem()->SetStartColor(Vec4(0.f, 0.5f, 0.5f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
		m_pBuffParticle->ParticleSystem()->SetEndColor(Vec4(0.f, 0.8f, 1.f, 1.0f));
		break;
	case BUFF_TYPE::STRENGTH:
		m_pBuffParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Sword"), L"ParticleUpdate2Mtrl");
		m_pBuffParticle->ParticleSystem()->SetStartColor(Vec4(1.f, 0.f, 0.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
		m_pBuffParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 0.f, 0.f, 1.f));
		break;
	case BUFF_TYPE::DAMAGE:

		m_pBuffParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Skeleton"), L"ParticleUpdate2Mtrl");
		m_pBuffParticle->ParticleSystem()->SetStartColor(Vec4(0.f, 0.f, 0.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
		m_pBuffParticle->ParticleSystem()->SetEndColor(Vec4(0.3f, 0.3f, 0.4f, 1.f));
		break;
	default:
		break;
	}

	m_pBuffParticle->ParticleSystem()->SetStartScale(20.f);
	m_pBuffParticle->ParticleSystem()->SetEndScale(30.f);

	m_pBuffParticle->ParticleSystem()->SetMinLifeTime(3.f);
	m_pBuffParticle->ParticleSystem()->SetMaxLifeTime(3.f);


	m_pSound= CResMgr::GetInst()->FindRes<CSound>(L"explosion");// 사운드 바꿔줭
	
	m_bParticle = false;

	m_pBuffParticle->FrustumCheck(false);

	//불스킬 터질시

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pBuffParticle);
}

BUFF_TYPE CBoxScript::GiveBuff()
{

	CSpawnMgr::GetInst()->GetSpawnArray()[m_iIdx].bSpawn = false;
	CSpawnMgr::GetInst()->SubSum();
	switch (m_eBuffType)
	{
	case BUFF_TYPE::HEAL:
		m_pPlayer->GetScript<CPlayerScript>()->SetDamage(-50.f);
		break;
	case BUFF_TYPE::STRENGTH:
		m_pPlayer->GetScript<CPlayerScript>()->GetBuff();
		break;
	case BUFF_TYPE::DAMAGE:
		m_pPlayer->GetScript<CPlayerScript>()->SetDamage(50.f);
		break;
	default:
		break;
	}
	return m_eBuffType;
}

void CBoxScript::Update()
{
	if (BOX_STATE::OPEN==m_bOpen) {
		if (!m_bParticle) {
			m_bParticle = true;
			m_pBuffParticle->SetActive(true);
		}
		Vec3 vRot = m_pUpObj->Transform()->GetLocalRot();
		vRot.x += DT;
		m_fUpObjPrevRot += DT;

		m_pUpObj->Transform()->SetLocalRot(vRot);
		
		if (m_fUpObjPrevRot >= 1.f) {
			m_bOpen=BOX_STATE::GOAWAY;
			DeleteObject(GetObj());
			DeleteObject(m_pUpObj);
			DeleteObject(m_pBuffParticle);
			DeleteObject(m_pBoxCol);
			GiveBuff();
		}
	}
	
}

void CBoxScript::SetOpen(const UINT& _bOpen)
{
	m_bOpen = (BOX_STATE)_bOpen;
	m_fUpObjPrevRot =0.f;
}

CBoxScript::CBoxScript():CScript((UINT)SCRIPT_TYPE::BOX)
{
}

CBoxScript::~CBoxScript()
{
}

void CBoxScript::OnCollision3D(CCollider3D* _pColldier)
{


}
