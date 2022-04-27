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

	m_eState = ARROW_STATE::IDLE;
}


void CArrowScript::Update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();

	// 추후에 카메라 위아래 움직임에 따라 발사 방향 바꿀 때 필요
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
#ifndef ARROW_TEST
		if (!m_bMaxCharged) {
			Vec3 vRestorePos = vPos;
			vPos.x += 50.f * DT;
			m_vDir = vRestorePos - vPos;
		}
		m_vRestorePos = vPos;
#else
		if (!m_bMaxCharged) {
			Vec3 vRestorePos = vPos;
			vPos.x += 130.f * DT;			// 15.f
		}
		Transform()->SetLocalPos(vPos);
#endif
	}
	break;
	case ARROW_STATE::ATTACK:
	{
#ifndef ARROW_TEST
		//if (Transform()->GetWorldPos().y <= 0.f) {
		//	GetObj()->SetActive(false);
		//	Init();
		//	m_eState = ARROW_STATE::IDLE;
		//}

		m_fElapsedTime += DT;
		if (m_fElapsedTime >= DT * 70.f) {
			vPos = m_vDestPos;
		}

		//Vec3 vDir = m_vDestPos - m_vRestorePos;

		//m_fRatio += 0.1f;
		//if (m_fRatio <= 1.f)
		//	vPos = Vec3::Lerp(m_vRestorePos, m_vDestPos, 0.05f);
		//else
		//	vPos = m_vDestPos;



		//vPos = Vec3::Lerp(m_vRestorePos, m_vDestPos, 0.05f);



		//m_vDir.Normalize();

		//vPos += m_vDir * m_fSpeed * DT * 0.1f;

		//m_fVelocityY -= (GRAVITY * DT) / 10;
		//m_fFallSpeed += m_fVelocityY;
		//vPos.y = m_vRestorePos.y;
		//vPos.y -= m_fFallSpeed * DT;

		//float value = XMConvertToRadians(90.f * DT * 10);

		//float vDotValue = Dot(m_vDir, Transform()->GetWorldDir(DIR_TYPE::FRONT));
		//Vec3 vCrossValue = Cross(Transform()->GetWorldDir(DIR_TYPE::FRONT), m_vDir);

		//XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&m_vDir), value);
		//Transform()->SetQuaternion(XMQuaternionMultiply(Transform()->GetQuaternion(), xmmatrix));






		//Vec3 vTarget = vPos - vFront;
		//vTarget.Normalize();
		////	vValue.Normalize();
		//float vDotValue = Dot(vTarget, result);
		//Vec3 vCrossValue = Cross(result, vTarget);
		//float a = vCrossValue.x;
		//float  c = vCrossValue.z;

		//if (!m_bSetDotValue) {
		//	m_bSetDotValue = true;
		//	m_fyPos = vCrossValue.z;
		//	m_fxPos = vCrossValue.x;
		//}
		//else {
		//	if (m_fyPos < vCrossValue.z) {
		//		vCrossValue.z = m_fyPos;
		//		vCrossValue.x = m_fxPos;
		//	}
		//	else {
		//		m_fyPos = vCrossValue.z;
		//		m_fxPos = vCrossValue.x;
		//	}
		//}

		//if (vCrossValue != Vec3(0.f, 0.f, 0.f)) {
		//	float value = XMConvertToRadians(vDotValue * DT * 10);

		//	XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), value);
		//	Transform()->SetQuaternion(XMQuaternionMultiply(Transform()->GetQuaternion(), xmmatrix));
		//}

		//Transform()->SetLocalPos(vPos);

#endif
#ifdef ARROW_TEST
		/*Vec3 vPos = Transform()->GetLocalPos();
		if (vPos.y < 0.f) {
			GetObj()->SetActive(false);
			Init();
			m_eState = ARROW_STATE::IDLE;
		}
		Vec3 vRot = Transform()->GetLocalRot();

		Vec3 vFront = vPos;
		Vec3 vRight = m_vDir;
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
		Transform()->SetLocalPos(vPos); */

		if (Transform()->GetWorldPos().y < 0.f)
		{
			GetObj()->SetActive(false);
			Init();
			m_eState = ARROW_STATE::IDLE;
		}

		m_vRestorePos = vPos;

		Vec3 vDir = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		vPos += m_vDir * m_fSpeed * DT;
		m_fVelocityY -= (GRAVITY * DT) / 10;
		m_fFallSpeed += m_fVelocityY;
		vPos.y += m_fFallSpeed * DT;

		m_vDeltaPos = vPos - m_vRestorePos;
		float fAngle = XMConvertToRadians(acos(Dot(m_vDir, m_vDeltaPos)));
		//vRot.y += fAngle;
		
		Transform()->SetLocalPos(vPos);
		Transform()->SetLocalRot(vRot);

		m_vTargetDir = vPos - m_vRestorePos;
		m_vTargetDir.Normalize();

		float value = XMConvertToRadians(90.f * DT * 10);

		float vDotValue = Dot(vDir, m_vTargetDir);
		//Vec3 vCrossValue = Cross(m_vTargetDir, m_vDir);

		Vec3 vCrossValue = Cross(m_vDir, vDir);

		//XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), value);
		//Transform()->SetQuaternion(XMQuaternionMultiply(Transform()->GetQuaternion(), xmmatrix));
		//Transform()->SetQuaternion(Vec4(fAngle, 0.f, 0.f, 1.f));

#endif
	}
	break;
	}
}

void CArrowScript::Init()
{
	m_bSetDotValue = false;
	m_fVelocityY = 0.f;
	m_fFallSpeed = 0.f;
	m_bMaxCharged = false;
	Transform()->SetQuaternion(Vec4(0.f,0.f,0.f,1.f));
	Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(0.f), XMConvertToRadians(0.f)));

	//m_pBow->AddChild(GetObj());
}
#include "Collider3D.h"
void CArrowScript::OnCollision3DEnter(CCollider3D* _pColldier)
{
	if (L"Monster" == _pColldier->GetObj()->GetName())
	{
		GetObj()->SetActive(false);
		Init();
	}
}

CArrowScript::CArrowScript(ELEMENT_TYPE _iType):CScript((UINT)SCRIPT_TYPE::ARROWSCRIPT),m_iType(_iType),m_bMove(true)
{
}

CArrowScript::~CArrowScript()
{
}
 