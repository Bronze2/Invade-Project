#include "pch.h"
#include "MinionScript.h"
#include "Animator3D.h"
#include "SceneMgr.h"
#include "Sensor.h"
#include "TimeMgr.h"


void CMinionScript::Init()
{
	m_eState = MINION_STATE::WALK;
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
		m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
		GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::WALK);
		GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
		GetObj()->Animator3D()->SetCurTime(0.f);
		GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
		m_eState = MINION_STATE::WALK;
		m_ePrevState = MINION_STATE::WALK;
	}

	switch (m_eAttackType)
	{
	case MINION_ATTACK_TYPE::MELEE:{
		SetAttackRange(50);
		m_uiMaxHp = 450; m_uiAttackDamage = 30;
		}
		break;

	case MINION_ATTACK_TYPE::RANGE: {
		SetAttackRange(150);
		m_uiMaxHp = 300; m_uiAttackDamage = 20;
	}
	 break;

	case MINION_ATTACK_TYPE::CANON: {
		SetAttackRange(50);
		m_uiMaxHp = 550; m_uiAttackDamage = 60;
	}
	 break;

	}
	m_iCurHp = m_uiMaxHp;
	m_pTarget = m_pNexus;

}


void CMinionScript::Update()
{
	//CheckHp();
	if(m_eState != CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).state)
		m_eState = CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).state;

	m_FAnimation();
	if (m_eState == MINION_STATE::DIE) {
		return;
	}
	Vec3 vPos;
	Vec3 vRot;


	//cout << "GPos	";
	//cout << CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).pos.x << ",";
	//cout << CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).pos.y << ",";
	//cout << CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).pos.z << endl;

	if (Transform()->GetLocalPos().x == 0 &&
		Transform()->GetLocalPos().y == 0 &&
		Transform()->GetLocalPos().z == 0 )
	{
		Transform()->SetLocalPos(CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).pos);
	}
	switch (m_eState)
	{
	case MINION_STATE::WALK: {
		vPos = Vec3::Lerp(Transform()->GetLocalPos(), CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).pos, DT * 10.f);
		vRot = Vec3::Lerp(Transform()->GetLocalRot(), CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).rot, DT * 10.f);

		Transform()->SetLocalPos(vPos);
		Transform()->SetLocalRot(CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).rot);

	}
	break;
	case MINION_STATE::ATTACK:
	{
		vPos = Vec3::Lerp(Transform()->GetLocalPos(), CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).pos, DT * 10.f);
		vRot = Vec3::Lerp(Transform()->GetLocalRot(), CSceneMgr::GetInst()->get_minioninfo(GetObj()->GetId()).rot, DT * 10.f);

		Transform()->SetLocalPos(vPos);
		Transform()->SetLocalRot(vRot);

		if (m_bFinishAnimation) {
			CSceneMgr::GetInst()->set_minioninfoState(GetObj()->GetId(), MINION_STATE::WALK);
			m_eState = MINION_STATE::WALK;
		}
	}
	break;
	case MINION_STATE::DIE:
		cout << " DIE" << endl;
		break;
	default:
		break;
	}
	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);

}



void CMinionScript::OnDetectionEnter(CGameObject* _pOther)
{
	m_arrEnemy.push_back(_pOther);
	m_bFindNear = true;
}

void CMinionScript::OnDetection(CGameObject* _pOther)
{
	
}

void CMinionScript::OnDetectionExit(CGameObject* _pOther)
{
	vector<CGameObject*>::iterator iter=m_arrEnemy.begin();
	for (int i=0; iter != m_arrEnemy.end(); ++iter,++i) {
		if (m_arrEnemy[i] == _pOther) {
			m_arrEnemy.erase(iter);
			if (Sensor()->GetDetectionCount() == 0) {
				m_bFindNear = false;
				m_pTarget = m_pNexus;
			}
			else {
				FindNearObject(m_arrEnemy);
			}
			return;
		}
		
	}

}

void CMinionScript::AddObject(CGameObject* _pObject)
{

	m_arrEnemy.push_back(_pObject);

}

void CMinionScript::CheckHp()
{
	if (m_iCurHp <= 0.f&&!GetObj()->IsFallDown()) {
		m_eState = MINION_STATE::DIE;
		GetObj()->SetFallDown();
	}
}

void CMinionScript::CheckRange()
{
	
	if (m_pTarget == nullptr)return;
	Vec3 vTargetPos=m_pTarget->Transform()->GetWorldPos();
	Vec3 vPos = Transform()->GetWorldPos();
	float length = sqrt(pow(vTargetPos.x - vPos.x, 2) + pow(vTargetPos.z - vPos.z, 2));
	if (m_fAttackRange >= length) {
		m_eState = MINION_STATE::ATTACK;
	}
	else {
		if (m_bFinishAnimation) {
			
			
			m_eState = MINION_STATE::WALK;
			
			
		}
		
	}
}

void CMinionScript::FindNearObject(const vector<CGameObject*>& _pObject)
{
	if (m_arrEnemy.size() == 0) {
		m_pTarget = m_pNexus;
	}

	if (0 == _pObject.size()||!m_bFindNear)return;
	for (int i = 0; i < _pObject.size(); ++i) {
		if (i == 0) {
			m_pTarget = _pObject[i];
		}
		else {
			Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
			Vec3 vPos = Transform()->GetWorldPos();
			float length1= sqrt(pow(vTargetPos.x - vPos.x, 2) + pow(vTargetPos.z - vPos.z, 2));

			Vec3 vTargetPos2 = _pObject[i]->Transform()->GetWorldPos();
			float length2 = sqrt(pow(vTargetPos2.x - vPos.x, 2) + pow(vTargetPos2.z - vPos.z, 2));
			if (length1 > length2) {
				m_pTarget = _pObject[i];
			}
		}
	}



}


void CMinionScript::m_FAnimation()
{

	m_bFinishAnimation = false;
	if (m_eState != m_ePrevState)
	{
		switch (m_eState)
		{
		case MINION_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)MINION_STATE::IDLE);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::IDLE, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = m_eState;
			}
		}

		break;
		case MINION_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)MINION_STATE::WALK);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::WALK, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = m_eState;
			}
		}

		break;
		case MINION_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)MINION_STATE::ATTACK);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::ATTACK, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = m_eState;
			}
		}

		break;
		case MINION_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)MINION_STATE::DIE);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::DIE, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = m_eState;
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
		case MINION_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
					if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame()) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::IDLE);
						GetObj()->Animator3D()->SetNextFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
						m_bFinishAnimation = true;
					}
				}
			}
		}

		break;
		case MINION_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
					if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame()) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::WALK);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
						m_bFinishAnimation = true;
					}
				}
			}
		}

		break;

		break;
		case MINION_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
					if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame()) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::ATTACK);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);

						m_bFinishAnimation = true;
					}
				}
			}
		}

		break;
		case MINION_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - 1) ||
					m_pCurAnimClip->iStartFrame > GetObj()->Animator3D()->GetFrameIdx()) {
					GetObj()->SetFallDown();
					DeleteObject(GetObj());
				}
			}
		}

		break;
		default:
			break;
		}

	}


//#ifdef _ANIMATION_TEST
//	m_bFinishAnimation = false;
//	if (m_eState != m_ePrevState)
//	{
//		switch (m_eState)
//		{
//		case MINION_STATE::WALK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
//				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
//				GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::WALK);
//				GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::WALK, 0.f);
//				GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
//				m_ePrevState = m_eState;
//			}
//		}
//
//		break;
//		case MINION_STATE::ATTACK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
//				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
//				GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::ATTACK);
//				GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::ATTACK, 0.f);
//				GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
//				m_ePrevState = m_eState;
//			}
//		}
//
//		break;
//		case MINION_STATE::DIE:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
//				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
//				GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::DIE);
//				GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
//				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::DIE, 0.f);
//				GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
//				m_ePrevState = m_eState;
//			}
//		}
//
//		break;
//		default:
//			break;
//		}
//	}
//	else {
//		switch (m_eState)
//		{
//		case MINION_STATE::WALK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
//				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame) {
//					GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::WALK);
//					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
//					GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::WALK, 0.f);
//					GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
//					m_bFinishAnimation = true;
//				}
//			}
//		}
//
//		break;
//		case MINION_STATE::ATTACK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
//				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame) {
//					GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::ATTACK);
//					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
//					GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::ATTACK, 0.f);
//					GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
//
//					m_bFindNear = true;
//					m_bFinishAnimation = true;
//
//					if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
//						m_pTarget->GetScript<CMinionScript>()->GetDamage(m_uiAttackDamage);
//					}
//				}
//			}
//		}
//
//		break;
//		case MINION_STATE::DIE:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
//				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame || m_pCurAnimClip->iStartFrame > GetObj()->Animator3D()->GetFrameIdx()) {
//					DeleteObject(GetObj());
//				}
//			}
//		}
//
//		break;
//		default:
//			break;
//		}
//
//	}
//#else
//	m_bFinishAnimation = false;
//	if (m_eState != m_ePrevState)
//	{
//		switch (m_eState)
//		{
//		case MINION_STATE::WALK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK")) {
//				m_CurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK");
//				GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
//				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
//				GetObj()->Animator3D()->SetCurTime(0.f);
//				GetObj()->Animator3D()->SetStartFrameTime(time);
//				m_ePrevState = m_eState;
//			}
//		}
//
//		break;
//		case MINION_STATE::ATTACK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK")) {
//				m_CurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK");
//				GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
//				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
//				GetObj()->Animator3D()->SetCurTime(0.f);
//				GetObj()->Animator3D()->SetStartFrameTime(time);
//				m_ePrevState = m_eState;
//			}
//		}
//
//		break;
//		case MINION_STATE::DIE:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"DIE")) {
//				m_CurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"DIE");
//				GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
//				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
//				GetObj()->Animator3D()->SetCurTime(0.f);
//				GetObj()->Animator3D()->SetStartFrameTime(time);
//				m_ePrevState = m_eState;
//			}
//		}
//
//		break;
//		default:
//			break;
//		}
//	}
//	else {
//		switch (m_eState)
//		{
//		case MINION_STATE::WALK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK")) {
//				if (GetObj()->Animator3D()->GetFrameIdx() >= m_CurAnimation->EndFrame) {
//					GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
//					double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
//					GetObj()->Animator3D()->SetCurTime(0.f);
//					GetObj()->Animator3D()->SetStartFrameTime(time);
//					m_bFinishAnimation = true;
//				}
//			}
//		}
//
//		break;
//		case MINION_STATE::ATTACK:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK")) {
//				if (GetObj()->Animator3D()->GetFrameIdx() >= m_CurAnimation->EndFrame) {
//					GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
//					double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
//					GetObj()->Animator3D()->SetCurTime(0.f);
//					GetObj()->Animator3D()->SetStartFrameTime(time);
//					m_bFindNear = true;
//					m_bFinishAnimation = true;
//					if (m_pTarget == nullptr) {
//
//					}
//					else
//						if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
//							m_pTarget->GetScript<CMinionScript>()->GetDamage(m_uiAttackDamage);
//						}
//				}
//			}
//		}
//
//		break;
//		case MINION_STATE::DIE:
//		{
//			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"DIE")) {
//				if (GetObj()->Animator3D()->GetFrameIdx() >= m_CurAnimation->EndFrame || m_CurAnimation->StartFrame > GetObj()->Animator3D()->GetFrameIdx()) {
//					DeleteObject(GetObj());
//
//				}
//			}
//		}
//
//		break;
//		default:
//			break;
//		}
//
//	}
//#endif
}

void CMinionScript::m_FFind()
{
}

CMinionScript::CMinionScript():CScript((UINT)SCRIPT_TYPE::MINIONSCRIPT),m_eState(MINION_STATE::WALK)
{
}

CMinionScript::CMinionScript(float _fSpeed, float _fRange, MINION_STATE _eState, CAMP_STATE _eCamp)
	: CScript((UINT)SCRIPT_TYPE::MINIONSCRIPT),m_fSpeed(_fSpeed),m_fRange(_fRange),m_eState(_eState),m_eCamp(_eCamp), m_bAllienceCol(false),m_bFinishAnimation(false)
	,m_bRotate(false),m_bSeparate(false)
{
}

CMinionScript::~CMinionScript()
{
}

#include "Collider3D.h"

static bool iSeparate=true;

void CMinionScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	if (_pOther->GetObj()->GetScript<CMinionScript>() == nullptr) {
	
	}
	else {
		if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() == m_eCamp&&m_eState==MINION_STATE::WALK) {
			m_bAllienceCol = true;
			m_bRotate = true;
			if (_pOther->GetObj()->GetScript<CMinionScript>()->GetState() == MINION_STATE::WALK) {
				float Value=_pOther->Transform()->GetLocalPos().x - Transform()->GetLocalPos().x;
				if (Value >= 0) {
					_pOther->GetObj()->GetScript<CMinionScript>()->SetSeparate(false);
					GetObj()->GetScript<CMinionScript>()->SetSeparate(true);
				}
				else {
					GetObj()->GetScript<CMinionScript>()->SetSeparate(false);
					_pOther->GetObj()->GetScript<CMinionScript>()->SetSeparate(true);
				}
			
				}
			}
			m_iAllienceCol += 1;
		}
}




void CMinionScript::OnCollision3D(CCollider3D* _pOther)
{
}

void CMinionScript::OnCollision3DExit(CCollider3D* _pOther)
{
	if (_pOther->GetObj()->GetScript<CMinionScript>() == nullptr) {

	}
	else {
		if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() == m_eCamp && m_eState == MINION_STATE::WALK) {
			
		
		}
		m_iAllienceCol -= 1;
		if (m_iAllienceCol <= 0) {
			m_iAllienceCol = 0;
		}
		if (m_iAllienceCol == 0) {
			m_bAllienceCol = false;
		}
	}

}

void CMinionScript::SetDamage(const int& _Damage)
{
	m_iCurHp -= _Damage;
	if (m_iCurHp < 0) {
		m_iCurHp = 0;
	}
	if (m_iCurHp > m_uiMaxHp)
		m_iCurHp = m_uiMaxHp;
}