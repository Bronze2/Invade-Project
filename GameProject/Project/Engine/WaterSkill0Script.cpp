#include "pch.h"
#include "WaterSkill0Script.h"

#include "PlayerScript.h"
#include"ParticleSystem.h"
CWaterSkill0Script::CWaterSkill0Script():CScript((UINT)SCRIPT_TYPE::WATERSKILL0),m_bStart(false),m_bTickCheck(false)
,m_iCount(0)
{
}

CWaterSkill0Script::~CWaterSkill0Script()
{
}

void CWaterSkill0Script::Update()
{
	if (nullptr == m_pSkill)return;
	if (!m_bMove) {
		Vec3 vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		int a = 0;
		Vec4 vDir = Vec4(vWorldDir, 1.f);
		m_pParticle->ParticleSystem()->SetDir(vDir);


		return;

	}


	/*	if (nullptr != m_pTarget->GetScript<CPlayerScript>()) {
		if (!m_bStart) {
			m_bStart = true;
			m_tStart = clock();

		}
		else {
			m_tEnd = clock();
			m_tInterval = m_tEnd - m_tStart / CLOCKS_PER_SEC;
			if (!m_bTickCheck) {
				m_tTickInterval = m_tEnd - m_tStart / CLOCKS_PER_SEC;
				
			}
			else {
				m_tTickInterval = m_tEnd - m_tTickStart / CLOCKS_PER_SEC;
			}
			if (m_tTickInterval >= 1.f) {
				m_pTarget->GetScript<CPlayerScript>()->SetDamage(m_pSkill->DotDamage);
				m_tTickStart = clock();
				m_bTickCheck = true;
			}
			if (m_tInterval >= m_pSkill->fCoolTime) {
				m_bStart = false;
				m_bTickCheck = false;
				DeleteObject(GetObj());
			}
		}
	}*/

	Vec3 vPos = Transform()->GetLocalPos();
	if (vPos.y < 0.f) {
		GetObj()->SetActive(false);
		Init();

	}
	Vec3 vRot = Transform()->GetLocalRot();


	Vec3 vFront = vPos;
	Vec3 vRight = Vec3(1, 0, 0);
	auto p = XMLoadFloat3(&vRight);
	auto m = XMLoadFloat4x4(&Transform()->GetWorldMat());
	auto r = XMVector3TransformNormal(p, m);
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Normalize(r));
	vPos.x += result.x * m_fSpeed * DT;
	vPos.z += result.z * m_fSpeed * DT;
	m_fVelocityY -= (GRAVITY * DT) / 10;
	m_fFallSpeed += m_fVelocityY;
	vPos.y += m_fFallSpeed * DT;



	Vec3 vTarget = vPos - vFront;
	vTarget.Normalize();
	//	vValue.Normalize();
	float vDotValue = Dot(vTarget, result);
	Vec3 vCrossValue = Cross(result, vTarget);
	float a = vCrossValue.x;
	float  c = vCrossValue.z;

	if (!m_bSetDotValue) {
		m_bSetDotValue = true;
		m_fyPos = vCrossValue.z;
		m_fxPos = vCrossValue.x;
	}
	else {
		if (m_fyPos < vCrossValue.z) {
			vCrossValue.z = m_fyPos;
			vCrossValue.x = m_fxPos;
		}
		else {
			m_fyPos = vCrossValue.z;
			m_fxPos = vCrossValue.x;
		}
	}


	if (vCrossValue != Vec3(0.f, 0.f, 0.f)) {
		float value = XMConvertToRadians(vDotValue * DT * 10);

		XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), value);
		Transform()->SetQuaternion(XMQuaternionMultiply(Transform()->GetQuaternion(), xmmatrix));

	}



	Transform()->SetLocalPos(vPos);
}
void CWaterSkill0Script::Init()
{

	m_bSetDotValue = false;
	m_fVelocityY = 0.f;
	m_fFallSpeed = 0.f;
	Transform()->SetQuaternion(Vec4(0.f, 0.f, 0.f, 1.f));

}

void CWaterSkill0Script::SetSkill(SKILL* _pSkill)
{
	m_pSkill = _pSkill;
}

void CWaterSkill0Script::Awake()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	m_pParticle = new CGameObject;
	m_pParticle->SetName(L"Particle");
	m_pParticle->AddComponent(new CTransform);
	m_pParticle->AddComponent(new CParticleSystem);
	m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Bubble99"), L"ParticleUpdateMtrl");
	m_pParticle->ParticleSystem()->SetStartColor(Vec4(0.4f, 0.4f, 0.8f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	m_pParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 1.f, 1.f, 1.0f));
	m_pParticle->ParticleSystem()->SetStartScale(2.f);
	m_pParticle->ParticleSystem()->SetEndScale(5.f);
	m_pParticle->FrustumCheck(false);
	m_pParticle->Transform()->SetLocalPos(Vec3(0.5f, 0.f, 0.f));
	GetObj()->AddChild(m_pParticle);



}
#include "Collider3D.h"
#include "SkillMgr.h"

void CWaterSkill0Script::OnCollision3DEnter(CCollider3D* _pColldier)
{
	if (nullptr != _pColldier->GetObj()->GetScript<CPlayerScript>()) {
		if (_pColldier->GetObj()->GetLayerIdx() == 3) {
		
			_pColldier->GetObj()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
			Transform()->SetLocalPos(Vec3(-10000.f, -10000.f, -10000.f));

		}
	}
}
