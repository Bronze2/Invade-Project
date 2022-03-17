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
	case ELEMENT_TYPE::FROZEN:
		m_pParticle->AddComponent(new CParticleSystem);
		m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Snow"));
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
		m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"smokeparticle"));
		m_pParticle->ParticleSystem()->SetStartColor(Vec4(1.f, 1.f, 0.f, 0.5f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
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

	pCurScene->FindLayer(L"Default")->AddGameObject(m_pParticle);

	GetObj()->AddChild(m_pParticle);
}


void CArrowScript::Update()
{

	if (!m_bMove)
	{
		Vec3 vPos3 = Transform()->GetLocalPos();
		Vec3 vFront = vPos3;
		Vec3 vRight = Vec3(1, 0, 0);
		auto p = XMLoadFloat3(&vRight);
		auto m = XMLoadFloat4x4(&Transform()->GetWorldMat());
		auto r = XMVector3TransformNormal(p, m);
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMVector3Normalize(r));

		//	Vec3 vDir = Transform()->GetLocalDir(DIR_TYPE::RIGHT);

		//	Vec3 vWorldDir = Transform()SetVelocityY->GetWorldDir(DIR_TYPE::UP);
		vPos3.x += result.x;
		vPos3.z += result.z;
		//vDir.Normalize();
		if (KEY_HOLD(KEY_TYPE::KEY_UP)) {
			m_fTestRotate += 10.f*DT;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_DOWN)) {
			m_fTestRotate -= 10.f * DT;
		}
		


		vPos3.y += m_fTestRotate;
		Vec3 vTarget = vPos3 - vFront;
		vTarget.Normalize();
		//	vValue.Normalize();
		float vDotValue = Dot(vTarget, result);

		Vec3 vCrossValue = Cross(result, vTarget);
		float a = vCrossValue.y;
		float b = vTarget.y;
		float c = vCrossValue.z;
		if (c < 0) {
			m_fB = vCrossValue.z;
		}
		else {
			vCrossValue.z = m_fB;
		}

		if (vCrossValue != Vec3(0.f, 0.f, 0.f)) {

			XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), XMConvertToRadians(vDotValue*DT));
			Transform()->SetQuaternion(XMQuaternionMultiply(Transform()->GetQuaternion(), xmmatrix));
	
		}

		Vec3 vRot = Transform()->GetLocalRot();
		vPos3 = Transform()->GetLocalPos();


	
		Transform()->SetLocalRot(vRot);
		Transform()->SetLocalPos(vPos3);
	
		return;

	}
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vFront=vPos;
	Vec3 vRight=Vec3(1, 0, 0);
	auto p = XMLoadFloat3(&vRight);
	auto m = XMLoadFloat4x4(&Transform()->GetWorldMat());
	auto r = XMVector3TransformNormal(p, m);
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Normalize(r));

//	Vec3 vDir = Transform()->GetLocalDir(DIR_TYPE::RIGHT);

//	Vec3 vWorldDir = Transform()SetVelocityY->GetWorldDir(DIR_TYPE::UP);
	vPos.x += result.x;
	vPos.z += result.z;
	//vDir.Normalize();
	m_fVelocityY -= (GRAVITY * DT) / 100;
	m_fFallSpeed+= m_fVelocityY;
	vPos.y += m_fFallSpeed*DT;
	Vec3 vTarget = vPos-vFront;
	vTarget.Normalize();	
//	vValue.Normalize();
	float vDotValue = Dot(vTarget, result);
	if (false == m_bSetDotValue) {
		m_bSetDotValue = true;
		m_fDotValue = vDotValue;
	}
	else {
		
	}
	


	Vec3 vCrossValue=Cross(result, vTarget);
	float a = vCrossValue.y;
	float b = vTarget.y;
	float c = vCrossValue.z;
	int d = vCrossValue.y;
	if (vCrossValue != Vec3(0.f, 0.f, 0.f)) {

		XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), XMConvertToRadians(vDotValue));
		Transform()->SetQuaternion(XMQuaternionMultiply(Transform()->GetQuaternion(), xmmatrix));
		Vec4 v = Transform()->GetQuaternion();
		int a = 0;
	
	}

	

//	Transform()->SetLocalPos(vPos);

	
}

void CArrowScript::OnCollisionEnter(CCollider2D* _pOther)
{
	if (L"Monster Object" == _pOther->GetObj()->GetName())
		DeleteObject(GetObj());
}






void CArrowScript::Init()
{
}

CArrowScript::CArrowScript(ELEMENT_TYPE _iType):CScript((UINT)SCRIPT_TYPE::ARROWSCRIPT),m_iType(_iType),m_bMove(true)
{
}

CArrowScript::~CArrowScript()
{
}
