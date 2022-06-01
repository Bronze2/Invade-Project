#include "pch.h"
#include "PlayerScript.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "Camera.h"
#include"CameraScript.h"
#include "Animator3D.h"

#include "ProjectileScript.h"

#include "BowScript.h"
#include "ParticleSystem.h"
#include "SkillMgr.h"
#include "WaterSkill0Script.h"
#include "WindSkill0Script.h"
#include "WindSkill1Script.h"
#include "ThunderSkill0Script.h"
#include "ThunderSkill1Script.h"
#include"DarkSkill0Script.h"
#include "FireSkill0Script.h"
#include "FireSkill1Script.h"
#include "Collider3D.h"

void CPlayerScript::m_FAnimation()
{
	if (m_ePrevState != m_eState) {
		// Blend �ʿ�
		switch (m_eState)
		{
		case PLAYER_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::IDLE);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::IDLE, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::IDLE;
			}
		}
		break;
		case PLAYER_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::WALK);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::WALK, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::WALK;
			}
		}
		break;
		case PLAYER_STATE::RUN:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::RUN);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::RUN, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::RUN;
			}
		}
		break;
		case PLAYER_STATE::JUMP:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::JUMP);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::JUMP, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::JUMP;
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK_READY);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK_READY, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK_READY;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK_READY);
			}
		}
		break;
		case PLAYER_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK);
			}
		}
		break;
		case PLAYER_STATE::DEMAGED:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::DEMAGED);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::DEMAGED, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::DEMAGED;
			}
		}
		break;
		case PLAYER_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::DIE);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::DIE, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::DIE;
			}
		}
		break;

		default:
			break;
		}
	}
	else {
		switch (m_eState)
		{
		case PLAYER_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");

					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::IDLE);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
					}
				}
				
			}
		}
		break;
		case PLAYER_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");

					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::WALK);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
					}
				}
				
			}
		}
		break;
		case PLAYER_STATE::RUN:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"RUN");

					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::RUN);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
					}
				}

			}
		}
		break;
		case PLAYER_STATE::JUMP:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						m_eState = PLAYER_STATE::IDLE;
					}
				}
				
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
					}
				}
			}
		}
		break;
		case PLAYER_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
						m_eState = PLAYER_STATE::IDLE;
						GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::IDLE);
					}
				}
			}
		}
		break;
		case PLAYER_STATE::DEMAGED:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEMAGED");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						m_eState = PLAYER_STATE::IDLE;
					}
				}

			}
		}
		break;
		case PLAYER_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						m_eState = PLAYER_STATE::IDLE;
					}
				}
			}
		}
		break;

		default:
			break;
		}
	}
}


void CPlayerScript::Init()
{
	m_eState = PLAYER_STATE::IDLE;
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
		m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
		GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::IDLE);
		GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
		GetObj()->Animator3D()->SetCurTime(0.f);
		GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
		m_eState = PLAYER_STATE::IDLE;
		m_ePrevState = PLAYER_STATE::IDLE;
	}
}

void CPlayerScript::Awake()
{

	m_iMaxHp = 200;
	m_iCurHp = 50;

	m_fMoveSpeed = 300.f;

	m_pHealParticle = new CGameObject;
	m_pHealParticle->AddComponent(new CTransform);
	m_pHealParticle->AddComponent(new CParticleSystem);
	m_pHealParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"HardRain2"), L"ParticleUpdate4Mtrl");
	m_pHealParticle->ParticleSystem()->SetStartColor(Vec4(0.f, 0.5f, 0.5f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	m_pHealParticle->ParticleSystem()->SetEndColor(Vec4(0.f, 0.8f, 1.f, 1.0f));
	m_pHealParticle->ParticleSystem()->SetStartScale(10.f);
	m_pHealParticle->ParticleSystem()->SetEndScale(15.f);
	m_pHealParticle->FrustumCheck(false);
	m_pHealParticle->SetActive(false);
	
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pHealParticle);
	GetObj()->AddChild(m_pHealParticle);


	m_pFlameParticle = new CGameObject;
	m_pFlameParticle->AddComponent(new CTransform);
	m_pFlameParticle->AddComponent(new CParticleSystem);
	m_pFlameParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Flame"), L"ParticleUpdate4Mtrl");
	m_pFlameParticle->ParticleSystem()->SetStartColor(Vec4(0.5f, 0.5f, 0.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	m_pFlameParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 0.f, 0.f, 1.0f));
	m_pFlameParticle->ParticleSystem()->SetStartScale(10.f);
	m_pFlameParticle->ParticleSystem()->SetEndScale(0.1f);
	m_pFlameParticle->FrustumCheck(false);
	m_pFlameParticle->SetActive(false);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pFlameParticle);
	GetObj()->AddChild(m_pFlameParticle);
	

	m_pThunderParticle = new CGameObject;
	m_pThunderParticle->AddComponent(new CTransform);
	m_pThunderParticle->AddComponent(new CParticleSystem);
	m_pThunderParticle->Transform()->SetLocalPos(Vec3(0.f, 0.f,150.f));
	m_pThunderParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Thunder"), L"ParticleUpdate3Mtrl");
	m_pThunderParticle->ParticleSystem()->SetStartColor(Vec4(0.5f, 0.5f, 0.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	m_pThunderParticle->ParticleSystem()->SetEndColor(Vec4(1.f, 1.f, 0.f, 1.0f));
	m_pThunderParticle->ParticleSystem()->SetStartScale(10.f);
	m_pThunderParticle->ParticleSystem()->SetEndScale(10.f);
	m_pThunderParticle->FrustumCheck(false);
	m_pThunderParticle->SetActive(false);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pThunderParticle);
	GetObj()->AddChild(m_pThunderParticle);
	

	m_fMoveSpeed = 300.f;

}

void CPlayerScript::Update()
{
	
	SkillCoolTimeCheck();
	Vec3 vDirUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vDirFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Transform()->SetWorldDir(DIR_TYPE::UP, vDirFront);
	Transform()->SetWorldDir(DIR_TYPE::FRONT, vDirUp);

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject* pEmptyObject = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1]);
	CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];
	CCameraScript* pCameraScript = pCamera->GetScript<CCameraScript>();
	CGameObject* pBow = GetObj()->GetChild()[0];

	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vPos2 = Transform()->GetLocalPos();
	Vec3 vPos3 = Transform()->GetWorldPos();
	Vec3 vRot = Transform()->GetLocalRot();
	Vec3 vRestoreRot;
	int rotydegree = XMConvertToDegrees(vRot.y);
	int reminder = rotydegree % 360 + 90;

	if (0 <= reminder && reminder <= 180) {
		m_bCheckDegree = true;
	}
	else {
		m_bCheckDegree = false;
	}



	if ((KEY_TAB(KEY_TYPE::KEY_W) || KEY_TAB(KEY_TYPE::KEY_S) || KEY_TAB(KEY_TYPE::KEY_A) || KEY_TAB(KEY_TYPE::KEY_D)) && KEY_NONE(KEY_TYPE::KEY_LBTN)) {
		m_fFactor = 0.f;
		m_fTurnDegree = 0.f;
		m_bTurn = true;

		m_vRestoreRot = vRot;

	}


	if ((KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_S) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_D)) && KEY_NONE(KEY_TYPE::KEY_LBTN)) {
		m_eState = PLAYER_STATE::IDLE;
		m_fTurnDegree = 0.f;
		m_fFactor = 0.f;
		m_bTurn = true;
		m_vRestoreRot = vRot;
	}

	if ((KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_D)) && KEY_NONE(KEY_TYPE::KEY_LBTN)) {
		m_eState = PLAYER_STATE::WALK;
		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			m_eState = PLAYER_STATE::RUN;
		}


		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

		// m_fRotateDegree = 현재 정상적으로 플레이어가 있어야 할 회전값 (W기준 여기로 가야 함)
		m_fRotateDegree = XMConvertToDegrees(pEmptyObject->Transform()->GetLocalRot().y) - 90.f;

		m_fCurDegree = 0.f;
		m_iKeyHoldCnt = 0;

		if (KEY_HOLD(KEY_TYPE::KEY_W)) {
			m_fCurDegree += 0.f;
			m_iKeyHoldCnt++;
		}
		if (KEY_HOLD(KEY_TYPE::KEY_S)) {
			m_fCurDegree += 180.f;
			m_iKeyHoldCnt++;
		}
		if (KEY_HOLD(KEY_TYPE::KEY_A)) {
			if (KEY_HOLD(KEY_TYPE::KEY_S)) {
				m_fCurDegree += 270.f;
			}
			else {
				m_fCurDegree -= 90.f;
			}
			m_iKeyHoldCnt++;
		}
		if (KEY_HOLD(KEY_TYPE::KEY_D)) {
			m_fCurDegree += 90.f;
			m_iKeyHoldCnt++;
		}

		m_fCurDegree /= (float)m_iKeyHoldCnt;

		if ((KEY_HOLD(KEY_TYPE::KEY_W) && KEY_HOLD(KEY_TYPE::KEY_S)) || (KEY_HOLD(KEY_TYPE::KEY_A) && KEY_HOLD(KEY_TYPE::KEY_D))) {
			m_fCurDegree = 0.f;
		}

		m_fTurnDegree = m_fRotateDegree + m_fCurDegree;
		
		float fRotDegree = XMConvertToDegrees(m_vRestoreRot.y);
		if (fRotDegree > 360.f) {
			fRotDegree -= 360.f;
		}
		
		// 180도 이상 회전하는 경우 없도록
		if (abs(fRotDegree - m_fTurnDegree) > 180.f) {
			if (abs(fRotDegree - ((m_fTurnDegree + 360.f) - fRotDegree)) > abs(fRotDegree - ((m_fTurnDegree - 360.f) - fRotDegree))) {
				m_fTurnDegree -= 360.f;
			}
			else {
				m_fTurnDegree += 360.f;
			}
		}

		vRot.y = XMConvertToRadians(fRotDegree * (1.f - m_fFactor) + m_fTurnDegree * m_fFactor);

		if (m_fFactor < 1.f) {
			// 회전 속도 빠르게 할 때 m_fFactor 빠르게 해주면 됨
			m_fFactor += 5.f * DT;
		}
		else {
			if ((KEY_HOLD(KEY_TYPE::KEY_W) && KEY_HOLD(KEY_TYPE::KEY_S)) || (KEY_HOLD(KEY_TYPE::KEY_A) && KEY_HOLD(KEY_TYPE::KEY_D))) {
				m_bTurn = true;
			}
			else {
				m_bTurn = false;
			}
		}
		
		if (!m_bTurn) {
			vPos += vFront * m_fMoveSpeed * DT;
		}

		m_FColCheck(vPos3, vPos);
	}

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
		m_fRotateDegree = XMConvertToDegrees(pEmptyObject->Transform()->GetLocalRot().y) - 90.f;
		// 공격 시 무조건 카메라가 바라보는 방향으로 플레이어 회전시키기 (화살 개발 이후 주석 풀기)
		vRot.y = XMConvertToRadians(m_fRotateDegree + 10.f);	// 5.f 더 회전시킬건지?

		m_eState = PLAYER_STATE::ATTACK_READY;
	}

	if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
		m_eState = PLAYER_STATE::ATTACK;
	}
	if (m_bMoveCheck) {
		vPos = vPos2;
		m_bMoveCheck = false;
	}

	if (KEY_TAB(KEY_TYPE::KEY_3))
	{
		m_eState = PLAYER_STATE::DEMAGED;
	}

	if (KEY_TAB(KEY_TYPE::KEY_4))
	{
		m_eState = PLAYER_STATE::JUMP;
	}

	if (KEY_TAB(KEY_TYPE::KEY_5))
	{
		m_eState = PLAYER_STATE::DIE;
	}

	UseSkill();
	StatusCheck();
	GetDamage();
	Transform()->SetLocalRot(vRot);
	Transform()->SetLocalPos(vPos);

	m_FAnimation();

}
void CPlayerScript::SetType(ELEMENT_TYPE _iType)
{
	m_iType = _iType;
	switch (m_iType)
	{
	case ELEMENT_TYPE::WATER:
	{
		m_tESkill =CSkillMgr::GetInst()->FindSkill(0);
		m_tZSkill = CSkillMgr::GetInst()->FindSkill(1);
	}
		break;
	case ELEMENT_TYPE::DARK:
	{
		m_tESkill = CSkillMgr::GetInst()->FindSkill(4);
		m_tZSkill = CSkillMgr::GetInst()->FindSkill(5);
	}
	break;
	case ELEMENT_TYPE::WIND:
	{
		m_tESkill = CSkillMgr::GetInst()->FindSkill(2);
		m_tZSkill = CSkillMgr::GetInst()->FindSkill(3);
	}
	break;
	case ELEMENT_TYPE::THUNDER:
	{
		m_tESkill = CSkillMgr::GetInst()->FindSkill(6);
		m_tZSkill = CSkillMgr::GetInst()->FindSkill(7);
	}
	break;
	case ELEMENT_TYPE::FIRE:
	{
		m_tESkill = CSkillMgr::GetInst()->FindSkill(8);
		m_tZSkill = CSkillMgr::GetInst()->FindSkill(9);
	}
	break;

	default:
		break;
	}
	
		
		
}
void CPlayerScript::SkillCoolTimeCheck()
{
	if (m_tESkill->bUse) {
		if (CoolTimeCheck(m_tESkill->StartTime, m_tESkill->fCoolTime)) {
			m_tESkill->bUse = false;
		}
	}
	if (m_tZSkill->bUse) {
		if (CoolTimeCheck(m_tZSkill->StartTime, m_tZSkill->fCoolTime)) {
			m_tZSkill->bUse = false;
		}
	}


}
void CPlayerScript::StatusCheck()
{
	m_bHealCheck = false;
	m_bFlameCheck = false;
	m_bThunderCheck = false;

	if (m_pHealParticle->IsActive()) {
		for (int i = 0; i < m_arrSkill.size(); ++i) {
			if (m_arrSkill[i]->eElementType == ELEMENT_TYPE::WATER) {
				m_bHealCheck = true;
				break;
			}
		}
		if (!m_bHealCheck) {
			m_pHealParticle->SetActive(false);
		}


	}
	if (m_pFlameParticle->IsActive()) {
		for (int i = 0; i < m_arrSkill.size(); ++i) {
			if (m_arrSkill[i]->eElementType == ELEMENT_TYPE::FIRE) {
				m_bFlameCheck = true;
				break;
			}
		}
		if (!m_bFlameCheck ) {
			m_pFlameParticle->SetActive(false);
		}


	}
	if (m_pThunderParticle->IsActive()) {
		for (int i = 0; i < m_arrSkill.size(); ++i) {
			if (m_arrSkill[i]->eElementType == ELEMENT_TYPE::THUNDER) {
				m_bThunderCheck = true;
				break;
			}
		}
		if (!m_bThunderCheck) {
			m_pThunderParticle->SetActive(false);
		}


	}
}
void CPlayerScript::UseSkill()
{
	if (KEY_AWAY(KEY_TYPE::KEY_E)) {
		if (m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->bSetSkill()) {
			if (!m_tESkill->bUse)
			{

				m_tESkill->bUse = true;
				m_tESkill->StartTime = chrono::system_clock::now();
				m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->SetSkill(m_tESkill);
			}
		}
	}

	if (KEY_AWAY(KEY_TYPE::KEY_Z)) {
		if (!m_tZSkill->bUse)
		{
			m_tZSkill->bUse = true;
			m_tZSkill->StartTime = chrono::system_clock::now();
			switch (m_tZSkill->eElementType)
			{
			case ELEMENT_TYPE::WATER:
				for (int i = 0; i < m_arrAlliance.size(); ++i) {
					if (nullptr != m_arrAlliance[i]->GetScript<CPlayerScript>()) {
						m_arrAlliance[i]->GetScript<CPlayerScript>()->DamageBySkill(m_tZSkill);
					}
				}

				DamageBySkill(m_tZSkill);
				break;
			case ELEMENT_TYPE::DARK:
			{

			}
				break;
			default:
			{	
				if (m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->bSetSkill()) {
					m_pBowObject->GetScript<CBowScript>()->GetCurArrow()->GetScript<CArrowScript>()->SetSkill(m_tZSkill);
				}
			}
				
				break;
			}
			

		}

	}
}
void CPlayerScript::DamageBySkill(SKILL* _pSkill)
{
	SKILL* pSkill = new SKILL;
	pSkill->DotDamage = _pSkill->DotDamage;
	pSkill->Code = _pSkill->Code;
	pSkill->Name = _pSkill->Name;
	pSkill->eSkillType = _pSkill->eSkillType;
	pSkill->eElementType = _pSkill->eElementType;

	pSkill->fCoolTime = _pSkill->fCoolTime;
	pSkill->fDuration = _pSkill->fDuration;
	pSkill->fDamage = _pSkill->fDamage;
	pSkill->bUse = false;
	pSkill->bFinal = false;
	pSkill->Count = 0;
	pSkill->Sum = _pSkill->Sum;
	m_arrSkill.push_back(pSkill);


}
void CPlayerScript::GetDamage()
{
	for (int i = 0; i < m_arrSkill.size(); ++i) {

		if (m_arrSkill[i]->bFinal)continue;
		if (m_arrSkill[i]->bUse == false) {
			m_arrSkill[i]->StartTime = std::chrono::system_clock::now();
			m_arrSkill[i]->bUse = true;
			switch (m_arrSkill[i]->eElementType)
			{
			case ELEMENT_TYPE::WATER:
				m_pHealParticle->SetActive(true);
				break;
			case ELEMENT_TYPE::FIRE:
				m_pThunderParticle->SetActive(true);
				break;
			default:
				break;
			}
		}
		else {
			if (TickCheck(m_arrSkill[i]->StartTime, m_arrSkill[i]->Count) != m_arrSkill[i]->Count) {
				m_iCurHp -= m_arrSkill[i]->DotDamage;
				m_arrSkill[i]->Sum += m_arrSkill[i]->DotDamage;
				
				m_arrSkill[i]->Count += 1;
			}
			
			if (m_arrSkill[i]->Count >= (int)m_arrSkill[i]->fDuration) {
				m_arrSkill[i]->bFinal = true;
				m_iCurHp += m_arrSkill[i]->Sum;
				
				//m_iCurHp -= 0.01f;
			}
		}
	}
	m_arrSkill.erase(std::remove_if(m_arrSkill.begin(), m_arrSkill.end(), SkillFinalCheck), m_arrSkill.end());
}

void CPlayerScript::m_FColCheck(Vec3 _vBeforePos, Vec3 _vAfterPos)
{
	if (m_bColCheck) {
		for (int i = 0; i < m_arrColObject.size(); ++i) {
			Vec3 vTargetPos = m_arrColObject[i]->Transform()->GetWorldPos();
			m_bMoveCheck=lengthCompare(_vBeforePos, _vAfterPos,GetObj(), vTargetPos);
			if (m_bMoveCheck) {
				return;
			}
		}
	}
}
void CPlayerScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	if (_pOther->GetObj()->GetScript<CArrowScript>() == nullptr) {
		if (_pOther->GetObj()->GetScript<CProjectileScript>() != nullptr) {

		}
		else {
			m_arrColObject.push_back(_pOther->GetObj());
			m_bColCheck = true;
		}
	}
	else {
		if (nullptr != _pOther->GetObj()->GetScript<CWaterSkill0Script>())
		{

		}
		else {
			if (nullptr != _pOther->GetObj()->GetScript<CArrowScript>())
			{
				if (_pOther->GetObj()->GetScript<CArrowScript>()->GetLayerIdx() != GetObj()->GetLayerIdx()) {
					CreateHitParticleObject(_pOther->GetObj()->GetChild()[0]->Transform()->GetWorldPos(), L"particle_00");
				}
			}

			
		}
	}
}

void CPlayerScript::OnCollision3D(CCollider3D* _pOther)
{
}

void CPlayerScript::OnCollision3DExit(CCollider3D* _pOther)
{

	if (_pOther->GetObj()->GetScript<CArrowScript>() != nullptr) {

	}
	else {
		
		m_arrColObject.erase(std::remove(m_arrColObject.begin(), m_arrColObject.end(), _pOther->GetObj()), m_arrColObject.end());
		
	}
	

}

void CPlayerScript::OnDetectionEnter(CGameObject* _pOther)
{
	if (_pOther->GetLayerIdx() != GetObj()->GetLayerIdx()) {
	}
	else {
		if(nullptr!=_pOther->GetScript<CPlayerScript>())
			m_arrAlliance.push_back(_pOther);
	}

}

void CPlayerScript::OnDetection(CGameObject* _pOther)
{

}

void CPlayerScript::OnDetectionExit(CGameObject* _pOther)
{
	vector<CGameObject*>::iterator iter = m_arrAlliance.begin();
	m_arrAlliance.erase(std::remove(m_arrAlliance.begin(), m_arrAlliance.end(),_pOther), m_arrAlliance.end());
	
}

CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT), m_bCheckStartMousePoint(false)
, m_bColCheck(false), m_bMoveCheck(false), m_bCheckDegree(false), m_fLerpTime(0.f), m_fMaxLerpTime(10.f), m_bHealCheck(false)
, m_bFlameCheck(false), m_bThunderCheck(false)
{}


CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::SetDamage(const int& _Damage)
{
	m_iCurHp -= _Damage;
	if (m_iCurHp < 0) {
		m_iCurHp = 0;
	}
	if (m_iCurHp > m_iMaxHp)
		m_iCurHp = m_iMaxHp;
}
