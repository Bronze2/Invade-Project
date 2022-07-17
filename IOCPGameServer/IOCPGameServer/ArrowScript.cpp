
#include "pch.h"
#include "ArrowScript.h"
#include "PlayerScript.h"
#include "TowerScript.h"
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

	m_eState = ARROW_STATE::ATTACK;
}


void CArrowScript::Update()
{
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

		//if (!m_bMaxCharged) {
		//	Vec3 vRestorePos = vPos;
		//	vPos.x += 30.f;
		//}
		//Transform()->SetLocalPos(vPos);
	}
	break;
	case ARROW_STATE::ATTACK:
	{
		cout << " UPDATE Arrow - " << m_id << endl;

		if (Transform()->GetLocalPos().y < 0.f)
		{
			cout << " Arrow Delete Server ID - " << m_id << endl;
			Init();
			//m_eState = ARROW_STATE::ATTACK;
			CServer::GetInst()->send_delete_arrow_packet(m_ParentId, m_id,0,0,0, m_PacketSkill);
			//GetObj()->SetActive(false);
			m_eState = ARROW_STATE::IDLE;
		}

		m_vRestorePos = vPos;

		//Vec3 vDir = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
	

		// m_fSpeed -> X�����̰�
		// m_fXSpeed + m_fYspeed = 500;

		vPos += m_vDir * m_fSpeed * DT;
	
		// y  m_fYspeed 
		m_fVelocityY -= (GRAVITY * DT);
		m_fFallSpeed += m_fVelocityY ;
		vPos.y += m_fFallSpeed * 7*  DT;
		
		//			(V * cos(rad_zy * Pi / 180) * cos(rad_zx * Pi / 180))
		//			V* sin(rad_zy * Pi / 180)
		
		m_vDeltaPos = vPos - m_vRestorePos;
		float fAngle = XMConvertToRadians(acos(Dot(m_vDir, m_vDeltaPos)));
		//vRot.y += fAngle;

		Transform()->SetLocalPos(vPos);
		Transform()->SetLocalRot(vRot);
		//cout << m_vDir.x << "," << m_vDir.y << "," << m_vDir.z << endl;
		Transform()->SetLocalPos(vPos);
		m_vTargetDir = vPos - m_vRestorePos;
		m_vTargetDir.Normalize();
		CServer::GetInst()->send_move_arrow_packet(m_ParentId,m_id,vPos,vRot, m_PacketSkill);

		//float value = XMConvertToRadians(90.f * DT * 10);

		//float vDotValue = Dot(vDir, m_vTargetDir);
		//Vec3 vCrossValue = Cross(m_vTargetDir, m_vDir);

		//Vec3 vCrossValue = Cross(m_vDir, vDir);

		//XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), value);
		//Transform()->SetQuaternion(XMQuaternionMultiply(Transform()->GetQuaternion(), xmmatrix));
		//Transform()->SetQuaternion(Vec4(fAngle, 0.f, 0.f, 1.f));
	
		// ȭ�� ���� -> Ŭ���̾�Ʈ BOW��ũ��Ʈ ���� ( INIT �Լ� ����)
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
	m_pParent->AddChild(index,GetObj());
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
				Init();
				CServer::GetInst()->send_delete_arrow_packet(m_ParentId, m_id,2, _pOther->GetObj()->GetScript<CMinionScript>()->m_GetId(),500, m_PacketSkill);
				 GetObj()->SetActive(false);
				
			}
		}

	}

	if (_pOther->GetObj()->GetScript<CPlayerScript>() != nullptr) {
		if (nullptr != _pOther->GetObj()) {
			if (_pOther->GetObj()->GetScript<CPlayerScript>()->GetCamp() != GetCamp())
			{
				cout << _pOther->GetObj()->Transform()->GetLocalPos().y << endl;
				//cout << _pOther->GetObj()->GetScript<CPlayerScript>()->m_GetId() << endl;
				_pOther->GetObj()->GetScript<CPlayerScript>()->GetDamage(500);
				Init();
				CServer::GetInst()->send_delete_arrow_packet(m_ParentId, m_id,1, _pOther->GetObj()->GetScript<CPlayerScript>()->m_GetId(),500, m_PacketSkill);
				GetObj()->SetActive(false);
			}
		}

	}

	if (_pOther->GetObj()->GetScript<CTowerScript>() != nullptr) {
		if (nullptr != _pOther->GetObj()) {
			Init();
			CServer::GetInst()->send_delete_arrow_packet(m_ParentId, m_id, 3, 0, 500, m_PacketSkill);
			GetObj()->SetActive(false);

		}

	}
}

CArrowScript::CArrowScript(ELEMENT_TYPE _iType) :CScript((UINT)SCRIPT_TYPE::ARROWSCRIPT), m_iType(_iType), m_bMove(true)
{
}

CArrowScript::~CArrowScript()
{
}