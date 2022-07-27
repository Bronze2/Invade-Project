#include "pch.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "PlayerScript.h"
#include "ParticleSystem.h"
#include "SkillMgr.h"
#include "TrailRenderer.h"
#include <math.h>

void CArrowScript::SetSkill(SKILL* _pSkill)
{
	m_pSkill = new SKILL;
	m_pSkill->DotDamage = _pSkill->DotDamage;
	m_pSkill->Code = _pSkill->Code;
	m_pSkill->Name = _pSkill->Name;
	m_pSkill->eSkillType = _pSkill->eSkillType;
	m_pSkill->eElementType = _pSkill->eElementType;

	m_pSkill->fCoolTime = _pSkill->fCoolTime;
	m_pSkill->fDuration = _pSkill->fDuration;
	m_pSkill->fDamage = _pSkill->fDamage;
	m_pSkill->bUse = false;
	m_pSkill->bFinal = false;
	m_pSkill->Count = 0;
	m_pSkill->Sum = _pSkill->Sum;
}
void CArrowScript::Awake()
{
	CScene* pCurScene=CSceneMgr::GetInst()->GetCurScene();
	m_pParticle = new CGameObject;
	m_pParticle->SetName(L"Particle");
	m_pParticle->AddComponent(new CTransform);
	switch (m_iType)
	{
	case ELEMENT_TYPE::WATER:
		m_pParticle->AddComponent(new CParticleSystem);
		m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Bubble99"), L"ParticleUpdateMtrl");
		m_pParticle->ParticleSystem()->SetStartColor(Vec4(0.4f, 0.4f, 0.8f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
		m_pParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 1.f, 1.f, 1.0f));
		m_pParticle->ParticleSystem()->SetStartScale(2.f);
		m_pParticle->ParticleSystem()->SetEndScale(5.f);
		break;
	case ELEMENT_TYPE::FIRE:
		m_pParticle->AddComponent(new CParticleSystem);
		m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"particle_00"));
		m_pParticle->ParticleSystem()->SetStartColor(Vec4(1.f, 1.f, 0.f, 0.5f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
		m_pParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 0.f, 0.f, 1.0f));
		m_pParticle->ParticleSystem()->SetStartScale(2.f);
		m_pParticle->ParticleSystem()->SetEndScale(5.f);
		break;
	case ELEMENT_TYPE::DARK:
		m_pParticle->AddComponent(new CParticleSystem);
		m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"smokeparticle"), L"ParticleUpdateMtrl");
		m_pParticle->ParticleSystem()->SetStartColor(Vec4(0.f, 0.f, 0.f, 0.5f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
		m_pParticle->ParticleSystem()->SetEndColor(Vec4(0.f, 0.f, 0.f, 1.0f));
		m_pParticle->ParticleSystem()->SetStartScale(2.f);
		m_pParticle->ParticleSystem()->SetEndScale(5.f);
		break;
	case ELEMENT_TYPE::THUNDER:

		break;
	case ELEMENT_TYPE::WIND:

		break;


	}
	m_pParticle->SetActive(false);
	m_pParticle->FrustumCheck(false);
	m_pParticle->Transform()->SetLocalPos(Vec3(0.5f, 0.f, 0.f));
	GetObj()->AddChild(m_pParticle);

	// 트레일
	m_pTrail = new CGameObject;
	m_pTrail->SetName(L"ArrowTrail");
	m_pTrail->AddComponent(new CTransform);
	m_pTrail->AddComponent(new CMeshRender);
	m_pTrail->AddComponent(new CTrailRenderer);
	m_pTrail->TrailRenderer()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Sparks"));
	m_pTrail->TrailRenderer()->SetColor(Vec4(0.8f, 0.f, 0.f, 0.5f));	// 레드 0.8f, 0.f, 0.f, 0.1f // 블루 0.f, 0.f, 0.8f, 0.1f
	m_pTrail->TrailRenderer()->SetMaxWidth(3.f);
	m_pTrail->TrailRenderer()->SetMinWidth(3.f);
	m_pTrail->TrailRenderer()->SetEmit(false);
	m_pTrail->SetActive(true);
	m_pTrail->FrustumCheck(false);
	m_pTrail->MeshRender()->SetDynamicShadow(false);
	//Vec3 vTrailPos = Vec3(0.f, 0.f, 0.f) - GetObj()->Transform()->GetWorldDir(DIR_TYPE::UP) * 100.f;
	//m_pTrail->Transform()->SetLocalPos(vTrailPos);
	m_pTrail->TrailRenderer()->SetTargetObj(GetObj());
	pCurScene->FindLayer(L"Default")->AddGameObject(m_pTrail);

	m_eState = ARROW_STATE::IDLE;

}


void CArrowScript::Update()
{
	// 화살 충돌 테스트
	SkillCheck();
	if (!m_bMove) { 
		Vec3 vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		int a = 0;
		Vec4 vDir = Vec4(vWorldDir, 1.f);
		m_pParticle->ParticleSystem()->SetDir(vDir);
	

		return;
	
	}

	
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();

	switch (m_eState)
	{
	case ARROW_STATE::IDLE:
	{
	}
	break;
	case ARROW_STATE::ATTACK_READY:
	{
		if (!m_bMaxCharged) {
			vPos.x += 15.f * DT;			// 15.f
		}
		Transform()->SetLocalPos(vPos);
	}
	break;
	case ARROW_STATE::ATTACK:
	{
		if (Transform()->GetWorldPos().y < 5.f)
		{
			//GetObj()->SetActive(false);
			// 트레일
			m_pTrail->TrailRenderer()->SetEmit(false);
			
			if (nullptr != m_pSkill)
			{
				delete m_pSkill;
				m_pSkill = nullptr;
			}
			//m_eState = ARROW_STATE::IDLE;
			//vPos.y = 5.f;
			//Transform()->SetLocalPos(vPos);
			//Transform()->SetLocalRot(vRot);
		}
		else
		{
			m_vRestorePos = vPos;

			Vec4 vDir = Vec4(m_vDir, 1.f);
			float a = vDir.x;
			vDir.x = vDir.z;
			vDir.z = a;
			m_pParticle->ParticleSystem()->SetDir(vDir);

			// 트레일
			m_pTrail->TrailRenderer()->SetEmit(true);

			// 화살 기존 코드
			m_vXZDir = Vec3(m_vDir.x, 0.f, m_vDir.z);
			m_vXZDir.Normalize();
			m_fAngle = acos(Dot(m_vDir, m_vXZDir));
			m_fAngle -= PI / 9;

			m_fTime += DT;

			//m_fMaxTime = m_fSpeed * sin(fAngle) / GRAVITY * 30 * DT;
			//m_fMaxTime 
			//cout << fAngle * 180 / PI <<"최고점 시간" << m_fMaxTime <<endl;
			//vRot.z = fAngle * 180 / PI;

			//vPos.x = vPos.x +(  m_fSpeed * m_fTime * cos(fAngle));
			vPos.z = m_vStartPos.z + m_vXZDir.z * (m_fSpeed * m_fTime * cos(m_fAngle)) / 2;
			vPos.x = m_vStartPos.x + m_vXZDir.x * (m_fSpeed * m_fTime * cos(m_fAngle)) / 2;

			//vPos = m_vDir 
			vPos.y = m_vStartPos.y + ((m_fSpeed * m_fTime * sin(m_fAngle)) - (0.5 * (GRAVITY * 70) * m_fTime * m_fTime));

			//처음 화살 Update
			if (m_fVelocityY == 5000) {
				//  현재 포물선 운동을 하고 있는 Y값
				// 
				m_fVelocityY = ((m_fSpeed * m_fTime * sin(m_fAngle)) - (0.5 * (GRAVITY * 70) * m_fTime * m_fTime));

				//최고점 높이 == velocity가 0이되는 지점. 
				m_fHighest = (m_fSpeed * sin(m_fAngle)) * (m_fSpeed * sin(m_fAngle)) / ((GRAVITY * 70) * 2);
				m_fPerRotate = m_fHighest / m_fAngle;

			}
			else {
				m_fVelocityY = ((m_fSpeed * m_fTime * sin(m_fAngle)) - (0.5 * (GRAVITY * 70) * m_fTime * m_fTime));
				float fHigh = m_fHighest - m_fVelocityY;
				m_fRotateAngle = fHigh / m_fPerRotate;
				if (m_fRotateAngle <= 0.005f && m_fDir == 1) {
					m_fDir = -1;
				}

			Quaternion qRot = Quaternion::CreateFromAxisAngle(m_vQtrnRotAxis, m_fRotateAngle * m_fDir);
			Transform()->SetQuaternion(qRot);
			}

			Transform()->SetLocalPos(vPos);

		}
	}
	break;
	}
}

void CArrowScript::Init()
{
	//m_pBow->AddChild(GetObj());
	if (nullptr == GetObj()->GetParent()) {
		m_pBow->AddChild(GetObj());
	}

	m_pTrail->TrailRenderer()->SetEmit(false);
	m_bSetDotValue = false;
	m_fVelocityY = 5000;
	m_fFallSpeed = 0.f;
	m_bMaxCharged = false;
	m_fTime = 0.f;
	m_fDir = 1;
	m_fHighest = 0;
	m_fPerRotate = 1;
	Transform()->SetQuaternion(Vec4(0.f,0.f,0.f,1.f));
	Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(0.f), XMConvertToRadians(0.f)));

}
void CArrowScript::SkillCheck()
{
	if (nullptr == m_pParticle)return;
	if (nullptr != m_pSkill) {
		m_pParticle->SetActive(true);
	}
	else {
		return;
		m_pParticle->SetActive(false);
	}
}
#include "TowerScript.h"
#include "MinionScript.h"
#include "Collider3D.h"
void CArrowScript::OnCollision3DEnter(CCollider3D* _pColldier)
{
	if (nullptr == m_pSkill) {
		if (_pColldier->GetObj()->GetLayerIdx() != m_iLayerIdx) {
			if (nullptr != _pColldier->GetObj()->GetScript<CPlayerScript>()) {
				_pColldier->GetObj()->GetScript<CPlayerScript>()->SetDamage(m_iDamage);
			}
			else if (nullptr != _pColldier->GetObj()->GetScript<CTowerScript>()) {
				_pColldier->GetObj()->GetScript<CTowerScript>()->GetDamage(m_iDamage);
			}
			else if (nullptr != _pColldier->GetObj()->GetScript<CMinionScript>()) {
				_pColldier->GetObj()->GetScript<CMinionScript>()->SetDamage(m_iDamage);
			}
		}
	}
	else {
		switch (m_pSkill->eElementType)
		{
		case ELEMENT_TYPE::WATER:
		{
			if (_pColldier->GetObj()->GetLayerIdx() == m_iLayerIdx) {
				if(_pColldier->GetObj()!=m_pPlayer)
					WaterSkill0(_pColldier);
			}
		}
			break;
		case ELEMENT_TYPE::DARK:
		{
			if (_pColldier->GetObj()->GetLayerIdx() != m_iLayerIdx) {
					DarkSkill0(_pColldier);
			}
		}
		break;
		case ELEMENT_TYPE::THUNDER:
		{
			if (m_pSkill->Code == (UINT)SKILL_CODE::THUNDER_0) {
				if (_pColldier->GetObj()->GetLayerIdx() != m_iLayerIdx) {
					ThunderSkill0(_pColldier);
				}
			}
			else {
				if (_pColldier->GetObj()->GetLayerIdx() != m_iLayerIdx) {
					ThunderSkill1(_pColldier);
				}
			}
		}
		break;
		case ELEMENT_TYPE::FIRE:
		{
			if (m_pSkill->Code == (UINT)SKILL_CODE::FIRE_0) {
				if (_pColldier->GetObj()->GetLayerIdx() != m_iLayerIdx) {
					FireSkill0(_pColldier);
				}
			}
			else {
				if (_pColldier->GetObj()->GetLayerIdx() != m_iLayerIdx) {
					FireSkill1(_pColldier);
				}
			}
		}
			break;
		case ELEMENT_TYPE::WIND:
		{
			if (m_pSkill->Code == (UINT)SKILL_CODE::WIND_0) 
			{
				if (_pColldier->GetObj()->GetLayerIdx() != m_iLayerIdx) {
					WindSkill0(_pColldier);
				}
			}
			else {
				if (_pColldier->GetObj()->GetLayerIdx() != m_iLayerIdx) {
					WindSkill1(_pColldier);
				}
			}
		}
		break;
		default:
			break;
		}
	}


}

CArrowScript::CArrowScript(ELEMENT_TYPE _iType):CScript((UINT)SCRIPT_TYPE::ARROWSCRIPT),m_iType(_iType),m_bMove(true)
,m_iDamage(20)
{
}

CArrowScript::~CArrowScript()
{
}
 
void CArrowScript::WaterSkill0(CCollider3D* _pColldier) {

	_pColldier->GetObj()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
	Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));

}

void CArrowScript::DarkSkill0(CCollider3D* _pCollider)
{
}

void CArrowScript::ThunderSkill0(CCollider3D* _pCollider)
{
}

void CArrowScript::ThunderSkill1(CCollider3D* _pCollider)
{
}

void CArrowScript::FireSkill0(CCollider3D* _pCollider)
{
}

void CArrowScript::FireSkill1(CCollider3D* _pCollider)
{
}

void CArrowScript::WindSkill0(CCollider3D* _pCollider)
{
}

void CArrowScript::WindSkill1(CCollider3D* _pCollider)
{
}