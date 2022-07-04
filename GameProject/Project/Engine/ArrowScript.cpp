
#include "pch.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "PlayerScript.h"
#include "ParticleSystem.h"
#include "SceneMgr.h"
#include "SkillMgr.h"
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

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
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
		m_pParticle->AddComponent(new CParticleSystem);
		m_pParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"smokeparticle"), L"ParticleUpdateMtrl");
		m_pParticle->ParticleSystem()->SetStartColor(Vec4(0.f, 0.f, 0.f, 0.5f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
		m_pParticle->ParticleSystem()->SetEndColor(Vec4(0.f, 0.f, 0.f, 1.0f));
		m_pParticle->ParticleSystem()->SetStartScale(2.f);
		m_pParticle->ParticleSystem()->SetEndScale(5.f);
		break;
	case ELEMENT_TYPE::WIND:
		m_pParticle->AddComponent(new CParticleSystem);
		break;


	}
	m_pParticle->SetActive(false);
	m_pParticle->FrustumCheck(false);
	m_pParticle->Transform()->SetLocalPos(Vec3(0.5f, 0.f, 0.f));
	GetObj()->AddChild(m_pParticle);


	m_eState = ARROW_STATE::IDLE;
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
void CArrowScript::Update()
{

	SkillCheck();

	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();

	// 추후에 카메라 위아래 움직임에 따라 발사 방향 바꿀 때 필요
	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];

	switch (m_eState)
	{
	case ARROW_STATE::IDLE:
	{
		Init();
		GetObj()->Transform()->SetLocalPos(Vec3(1000, 1000, 1000));
		GetObj()->SetActive(false);
	}
	break;
	case ARROW_STATE::ATTACK_READY:
	{
		if (!m_bMaxCharged) {
			Vec3 vRestorePos = vPos;
			vPos.x += 30.f * DT;			// 15.f
		}
		Transform()->SetLocalPos(vPos);
	}
	break;
	case ARROW_STATE::ATTACK:
	{
		//if (Transform()->GetLocalPos().y < 0.f)
		//{
		//	GetObj()->SetActive(false);
		//	Init();
		//	m_eState = ARROW_STATE::IDLE;
		//}
		vPos = Vec3::Lerp(vPos,CSceneMgr::GetInst()->get_arrowPos(m_ParentId, m_id),DT *10.f);
		vRot = Vec3::Lerp(vRot, CSceneMgr::GetInst()->get_arrowRot(m_ParentId, m_id), DT * 10.f);
		if (CSceneMgr::GetInst()->get_arrowSkill(m_ParentId, m_id)) {
			EnterSkill(vPos);
			CSceneMgr::GetInst()->set_arrowSkill(m_ParentId, m_id, false);
		}
		//cout << "클라[" << m_ParentId << "] 화살[" << m_id << "] Update " << vPos.x << "," << vPos.z << endl;
		Transform()->SetLocalPos(vPos);
		Transform()->SetLocalRot(vRot);


		//if (nullptr != m_pSkill) {
		//	if ((UINT)SKILL_CODE::THUNDER_1 == m_pSkill->Code) {
		//		if (vPos.y <= 1.f) {
		//			Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
		//			vPos3.y = 1.f;
		//			CreateThunderObject(vPos3, m_iLayerIdx);
		//			Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
		//		}
		//	}
		//	else if ((UINT)SKILL_CODE::_FIRE_1 == m_pSkill->Code) {
		//		if (vPos.y <= 1.f) {
		//			Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
		//			vPos3.y = 1.f;
		//			CreateBoomParticleObject(vPos3, L"smokeparticle");
		//			//Collision();
		//			Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
		//		}
		//	}
		//}

		//cout<<"화살 주인은" <<GetObj()->GetParent()->GetParent()->GetScript<CPlayerScript>()->m_GetId() <<endl;
		//cout << "화살 주인은" << GetParentId() << endl;
		
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

void CArrowScript::EnterSkill(Vec3 vPos)
{
	cout << "EnterSKill" << endl;
	if (nullptr != m_pSkill) {
		if ((UINT)SKILL_CODE::THUNDER_1 == m_pSkill->Code) {
				Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
				vPos.y = 1.f;
				CreateThunderObject(vPos, m_iLayerIdx);
				//Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
		}
		else if ((UINT)SKILL_CODE::_FIRE_1 == m_pSkill->Code) {
				Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
				vPos.y = 1.f;
				CreateBoomParticleObject(vPos, L"smokeparticle");
				//Collision();
				//Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
		}
	}

	SetState(ARROW_STATE::IDLE);

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

	m_pBow->AddChild(GetObj());
}
#include "Collider3D.h"
void CArrowScript::OnCollision3DEnter(CCollider3D* _pColldier)
{
	if (L"Temp" == _pColldier->GetObj()->GetName())
	{
		GetObj()->SetActive(false);
		Init();
	}
}

CArrowScript::CArrowScript(ELEMENT_TYPE _iType) :CScript((UINT)SCRIPT_TYPE::ARROWSCRIPT), m_iType(_iType), m_bMove(true)
{
}

CArrowScript::~CArrowScript()
{
	if (nullptr != m_pSkill)
	{
		delete m_pSkill;
		m_pSkill = nullptr;
	}

}

void CArrowScript::WaterSkill0(CCollider3D* _pCollider) {
	if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
		_pCollider->GetObj()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
		Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
	}

}

void CArrowScript::DarkSkill0(CCollider3D* _pCollider)
{
	if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
		_pCollider->GetObj()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
		Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
	}
}

void CArrowScript::ThunderSkill0(CCollider3D* _pCollider)
{
	if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
		_pCollider->GetObj()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
		Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
	}
}

void CArrowScript::ThunderSkill1(CCollider3D* _pCollider)
{
	Vec3 vPos = GetObj()->Transform()->GetWorldPos();
	vPos.y = 1.f;
	CreateThunderObject(vPos, m_iLayerIdx);
	if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
		_pCollider->GetObj()->GetScript<CPlayerScript>()->SetDamage(m_iDamage);
		Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
	}
	//else if (nullptr != _pCollider->GetObj()->GetScript<CMinionScript>()) {
	//	_pCollider->GetObj()->GetScript<CMinionScript>()->SetDamage(m_iDamage);
	//	Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
	//}
	//else if (nullptr != _pCollider->GetObj()->GetScript<CTowerScript>()) {
	//	_pCollider->GetObj()->GetScript<CTowerScript>()->GetDamage(m_iDamage);
	//	Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
	//}
}

void CArrowScript::FireSkill0(CCollider3D* _pCollider)
{
	if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
		_pCollider->GetObj()->GetScript<CPlayerScript>()->DamageBySkill(m_pSkill);
		Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
	}
}

void CArrowScript::FireSkill1(CCollider3D* _pCollider)
{
	if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
		Vec3 vPos3 = GetObj()->Transform()->GetWorldPos();
		CreateBoomParticleObject(vPos3, L"smokeparticle");
		//Collision();
		Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
	}

}

void CArrowScript::WindSkill0(CCollider3D* _pCollider)
{
	if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>())
		_pCollider->GetObj()->GetScript<CPlayerScript>()->SetDamage(m_pSkill->fDamage);
	else {
		Transform()->SetLocalPos(Vec3(-1000.f, -1000.f, -1000.f));
	}

}

void CArrowScript::WindSkill1(CCollider3D* _pCollider)
{
	//if (nullptr != _pCollider->GetObj()->GetScript<CMinionScript>()) {
	//	_pCollider->GetObj()->GetScript<CMinionScript>()->SetDamage(m_pSkill->fDamage);
	//}
	//else if (nullptr != _pCollider->GetObj()->GetScript<CTowerScript>()) {
	//	_pCollider->GetObj()->GetScript<CTowerScript>()->GetDamage(m_iDamage);

	//}
	//else if (nullptr != _pCollider->GetObj()->GetScript<CPlayerScript>()) {
	//	_pCollider->GetObj()->GetScript<CPlayerScript>()->SetDamage(m_pSkill->fDamage);

	//}

}
