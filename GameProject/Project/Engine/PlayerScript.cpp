#include "pch.h"
#include "PlayerScript.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "Camera.h"
#include"CameraScript.h"
#include "Animator3D.h"

#include "ProjectileScript.h"

#include "BowScript.h"

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
		case PLAYER_STATE::DEFEAT:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEFEAT")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEFEAT");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::DEFEAT);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::DEFEAT, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::DEFEAT;
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
		case PLAYER_STATE::DEFEAT:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEFEAT")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DEFEAT");
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
	m_fMoveSpeed = 300.f;
}

void CPlayerScript::Update()
{
	// Z-up To Y-up
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
	if (KEY_HOLD(KEY_TYPE::KEY_W) && KEY_HOLD(KEY_TYPE::KEY_A)) {
		int a = 0;
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

		// 180도 이상 회전하는 경우 없도록
		if ((fRotDegree - m_fTurnDegree > 180.f) || (fRotDegree - m_fTurnDegree < -180.f)) {
			if (m_fTurnDegree > 180.f) {		// 0, 270 || -90, 180 -> -90, -180
				m_fTurnDegree -= 360.f;
			}
			else {								// -135, 90
				m_fTurnDegree += 360.f;
			}
		}

		// 0 -> 270보다는 0 -> -90되는게 이쁘니까
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
		m_eState = PLAYER_STATE::WALK;
	}

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
		m_fRotateDegree = XMConvertToDegrees(pEmptyObject->Transform()->GetLocalRot().y) - 90.f;
		// 공격 시 무조건 카메라가 바라보는 방향으로 플레이어 회전시키기 (화살 개발 이후 주석 풀기)
		vRot.y = XMConvertToRadians(m_fRotateDegree);	// 5.f 더 회전시킬건지?

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
		m_eState = PLAYER_STATE::DEFEAT;
	}

	if (KEY_TAB(KEY_TYPE::KEY_4))
	{
		m_eState = PLAYER_STATE::JUMP;
	}

	if (KEY_TAB(KEY_TYPE::KEY_5))
	{
		m_eState = PLAYER_STATE::DIE;
	}

	Transform()->SetLocalRot(vRot);
	Transform()->SetLocalPos(vPos);

	m_FAnimation();

}
#include "Collider3D.h"
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
	int a = 0;
	if (_pOther->GetObj()->GetScript<CArrowScript>() != nullptr) {

	}
	else {
		if (_pOther->GetObj()->GetScript<CProjectileScript>() != nullptr) {

		}
		else {
			m_arrColObject.push_back(_pOther->GetObj());
			m_bColCheck = true;
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
		vector<CGameObject*>::iterator iter = m_arrColObject.begin();
		for (int i = 0; iter != m_arrColObject.end(); ++iter, ++i) {
			if (m_arrColObject[i] == _pOther->GetObj()) {
				m_arrColObject.erase(iter);
				return;
			}

		}
	}
	

}

CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT), m_bCheckStartMousePoint(false)
, m_bColCheck(false), m_bMoveCheck(false), m_bCheckDegree(false), m_fLerpTime(0.f), m_fMaxLerpTime(10.f)
{
}

CPlayerScript::~CPlayerScript()
{
}
