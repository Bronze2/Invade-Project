
#include "pch.h"
#include "ArrowScript.h"
#include "PlayerScript.h"
#include "Server.h"
#include <math.h>
void CArrowScript::Awake()
{

	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//m_pParticle = new CGameObject;
	//m_pParticle->SetName(L"Particle");
	//m_pParticle->AddComponent(new CTransform);
	//switch (m_iType)
	//{
	//case ELEMENT_TYPE::FROZEN:
	//	m_pParticle->AddComponent(new CParticleSystem);
	//	m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Snow"));
	//	m_pParticle->ParticleSystem()->SetStartColor(Vec4(0.4f, 0.4f, 0.8f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	//	m_pParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 1.f, 1.f, 1.0f));
	//	m_pParticle->ParticleSystem()->SetStartScale(2.f);
	//	m_pParticle->ParticleSystem()->SetEndScale(5.f);

	//	break;
	//case ELEMENT_TYPE::FIRE:
	//	m_pParticle->AddComponent(new CParticleSystem);
	//	m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"particle_00"));
	//	m_pParticle->ParticleSystem()->SetStartColor(Vec4(1.f, 1.f, 0.f, 0.5f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	//	m_pParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 0.f, 0.f, 1.0f));
	//	m_pParticle->ParticleSystem()->SetStartScale(2.f);
	//	m_pParticle->ParticleSystem()->SetEndScale(5.f);
	//	break;
	//case ELEMENT_TYPE::DARK:
	//	m_pParticle->AddComponent(new CParticleSystem);
	//	m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"smokeparticle"));
	//	m_pParticle->ParticleSystem()->SetStartColor(Vec4(1.f, 1.f, 0.f, 0.5f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	//	m_pParticle->ParticleSystem()->SetEndColor(Vec4(0.f, 0.f, 0.f, 1.0f));
	//	m_pParticle->ParticleSystem()->SetStartScale(2.f);
	//	m_pParticle->ParticleSystem()->SetEndScale(5.f);
	//	break;
	//case ELEMENT_TYPE::THUNDER:

	//	break;
	//case ELEMENT_TYPE::WIND:

	//	break;


	//}


	//m_pParticle->FrustumCheck(false);
	//m_pParticle->Transform()->SetLocalPos(Vec3(0.5f, 0.f, 0.f));

	//pCurScene->FindLayer(L"Default")->AddGameObject(m_pParticle);

	//GetObj()->AddChild(m_pParticle);

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
		//if (!m_bMaxCharged) {
		//	Vec3 vRestorePos = vPos;
		//	vPos.x += 30.f;
		//}
		//Transform()->SetLocalPos(vPos);
	}
	break;
	case ARROW_STATE::ATTACK:
	{
		if (Transform()->GetLocalPos().y < 0.f)
		{
			GetObj()->SetActive(false);
			Init();
			m_eState = ARROW_STATE::IDLE;
			CServer::GetInst()->send_delete_arrow_packet(m_ParentId, m_id,0,0,0);
		}

		m_vRestorePos = vPos;

		//Vec3 vDir = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
	
		vPos += m_vDir * m_fSpeed * DT;
		m_fVelocityY -= (GRAVITY * DT);
		m_fFallSpeed += m_fVelocityY ;
		vPos.y += m_fFallSpeed * DT;

		m_vDeltaPos = vPos - m_vRestorePos;
		float fAngle = XMConvertToRadians(acos(Dot(m_vDir, m_vDeltaPos)));
		//vRot.y += fAngle;

		Transform()->SetLocalPos(vPos);
		Transform()->SetLocalRot(vRot);
		//cout << m_vDir.x << "," << m_vDir.y << "," << m_vDir.z << endl;
		Transform()->SetLocalPos(vPos);
		m_vTargetDir = vPos - m_vRestorePos;
		m_vTargetDir.Normalize();
		CServer::GetInst()->send_move_arrow_packet(m_ParentId,m_id,vPos,vRot);

		//float value = XMConvertToRadians(90.f * DT * 10);

		//float vDotValue = Dot(vDir, m_vTargetDir);
		//Vec3 vCrossValue = Cross(m_vTargetDir, m_vDir);

		//Vec3 vCrossValue = Cross(m_vDir, vDir);

		//XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), value);
		//Transform()->SetQuaternion(XMQuaternionMultiply(Transform()->GetQuaternion(), xmmatrix));
		//Transform()->SetQuaternion(Vec4(fAngle, 0.f, 0.f, 1.f));
	
		// 화살 생성 -> 클라이언트 BOW스크립트 수정 ( INIT 함수 생성)
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
	Transform()->SetQuaternion(Vec4(0.f, 0.f, 0.f, 1.f));
	Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(0.f), XMConvertToRadians(0.f)));

	//m_pBow->AddChild(GetObj());
}
#include "Collider3D.h"
#include "MinionScript.h"
void CArrowScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	if (_pOther->GetObj()->GetScript<CMinionScript>() != nullptr) {
		if (nullptr != _pOther->GetObj()) {
			if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() != GetCamp())
			{
				_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(500);
				GetObj()->SetActive(false);
				Init();
				CServer::GetInst()->send_delete_arrow_packet(m_ParentId, m_id,2, _pOther->GetObj()->GetScript<CMinionScript>()->m_GetId(),500);
			}
		}

	}

	if (_pOther->GetObj()->GetScript<CPlayerScript>() != nullptr) {
		if (nullptr != _pOther->GetObj()) {
			if (_pOther->GetObj()->GetScript<CPlayerScript>()->GetCamp() != GetCamp())
			{
				cout << _pOther->GetObj()->GetScript<CPlayerScript>()->m_GetId() << endl;
				_pOther->GetObj()->GetScript<CPlayerScript>()->GetDamage(500);
				GetObj()->SetActive(false);
				Init();
				CServer::GetInst()->send_delete_arrow_packet(m_ParentId, m_id,1, _pOther->GetObj()->GetScript<CPlayerScript>()->m_GetId(),500);
			}
		}

	}
}

CArrowScript::CArrowScript(ELEMENT_TYPE _iType) :CScript((UINT)SCRIPT_TYPE::ARROWSCRIPT), m_iType(_iType), m_bMove(true)
{
}

CArrowScript::~CArrowScript()
{
}