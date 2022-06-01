#include "pch.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "PlayerScript.h"
#include "ParticleSystem.h"
#include <math.h>
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
		m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"HardRain"), L"ParticleUpdateMtrl");
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
	

	m_pParticle->FrustumCheck(false);
	m_pParticle->Transform()->SetLocalPos(Vec3(0.5f, 0.f, 0.f));
	GetObj()->AddChild(m_pParticle);


	m_eState = ARROW_STATE::IDLE;

}


void CArrowScript::Update()
{
	// 화살 충돌 테스트
	
	if (!m_bMove) { 
		Vec3 vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		int a = 0;
		Vec4 vDir = Vec4(vWorldDir, 1.f);
		m_pParticle->ParticleSystem()->SetDir(vDir);
	

		return;
	
	}


	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();

	// ���Ŀ� ī�޶� ���Ʒ� �����ӿ� ���� �߻� ���� �ٲ� �� �ʿ�
	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];

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
		if (Transform()->GetWorldPos().y < 0.f)
		{
			GetObj()->SetActive(false);
			//Init();
			m_eState = ARROW_STATE::IDLE;
		}

		m_vRestorePos = vPos;

		vPos.x += m_vDir.x * m_fSpeed * DT;
		vPos.y += m_vDir.y * m_fSpeed / 2 * DT;
		vPos.z += m_vDir.z * m_fSpeed * DT;

		m_fVelocityY -= (GRAVITY * DT) * 4;
		m_fFallSpeed += m_fVelocityY;
		vPos.y += m_fFallSpeed * DT;

		//m_vDeltaPos = vPos - m_vRestorePos;
		//float fAngle = XMConvertToRadians(acos(Dot(m_vDir, m_vDeltaPos)));
		//vRot.y += fAngle;
		
		Transform()->SetLocalPos(vPos);
		Transform()->SetLocalRot(vRot);

		//m_vTargetDir = vPos - m_vRestorePos;
		//m_vTargetDir.Normalize();

		//float value = XMConvertToRadians(90.f * DT * 10);

		//float vDotValue = Dot(vDir, m_vTargetDir);
		//Vec3 vCrossValue = Cross(m_vTargetDir, m_vDir);

		//Vec3 vCrossValue = Cross(m_vDir, vDir);

		//XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), value);
		//Transform()->SetQuaternion(XMQuaternionMultiply(Transform()->GetQuaternion(), xmmatrix));
		//Transform()->SetQuaternion(Vec4(fAngle, 0.f, 0.f, 1.f));
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


	m_bSetDotValue = false;
	m_fVelocityY = 0.f;
	m_fFallSpeed = 0.f;
	m_bMaxCharged = false;
	Transform()->SetQuaternion(Vec4(0.f,0.f,0.f,1.f));
	Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(80.f), XMConvertToRadians(0.f)));



}

#include "Collider3D.h"
void CArrowScript::OnCollision3DEnter(CCollider3D* _pColldier)
{
	
}

CArrowScript::CArrowScript(ELEMENT_TYPE _iType):CScript((UINT)SCRIPT_TYPE::ARROWSCRIPT),m_iType(_iType),m_bMove(true)
{
}

CArrowScript::~CArrowScript()
{
}
 