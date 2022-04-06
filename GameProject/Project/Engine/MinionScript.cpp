#include "pch.h"
#include "MinionScript.h"
#include "Animator3D.h"

#include "Sensor.h"
void CMinionScript::Init()
{
	m_eState = MINION_STATE::WALK;
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK")) {
		m_CurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK");
		GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
		double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
		GetObj()->Animator3D()->SetCurTime(0.f);
		GetObj()->Animator3D()->SetStartFrameTime(time);
		m_eState = MINION_STATE::WALK;
		m_ePrevState = MINION_STATE::WALK;
	}	
	m_pTarget = m_pNexus;
}


void CMinionScript::Update()
{

	FindNearObject(m_arrEnemy);
	Vec3 vPos = Transform()->GetWorldPos(); 
	Vec3 vTargetPos;
	Vec3 vRot = Transform()->GetLocalRot();
	if (nullptr != m_pTarget) {
		Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
		float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
		float rotate = angle * 0.0174532925f;
		vRot.y = rotate;

	}
	Vec3 vLocalPos = Transform()->GetLocalPos();
	
	m_FAnimation();
	Vec3 vWorldDir=GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	vLocalPos.x -= vWorldDir.x *100.f*DT;
	vLocalPos.z -= vWorldDir.z * 100.f * DT;
//
//	GetObj()->Transform()->SetLocalPos(vPos);
//
	Transform()->SetLocalPos(vLocalPos);
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
			return;
		}
		
	}
	if (Sensor()->GetDetectionCount() == 0) {
		m_bFindNear = false;
	}
}

void CMinionScript::AddObject(CGameObject* _pObject)
{

	m_arrEnemy.push_back(_pObject);

}

void  CMinionScript::FindNearObject(const vector<CGameObject*>& _pObject)
{
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
	switch (m_eState)
	{
	case MINION_STATE::WALK:
	{
		if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK")) {
			if (GetObj()->Animator3D()->GetFrameIdx() == m_CurAnimation->EndFrame) {
				GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
				GetObj()->Animator3D()->SetCurTime(0.f);
				GetObj()->Animator3D()->SetStartFrameTime(time);
			}
		}
	}

	break;
	case MINION_STATE::ATTACK:
	{
		if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK")) {
			if (GetObj()->Animator3D()->GetFrameIdx() == m_CurAnimation->EndFrame) {
				GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
				GetObj()->Animator3D()->SetCurTime(0.f);
				GetObj()->Animator3D()->SetStartFrameTime(time);
				m_bFindNear = true;
			}
		}
	}

	break;
	case MINION_STATE::DIE:
	{
		if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"DIE")) {
			if (GetObj()->Animator3D()->GetFrameIdx() == m_CurAnimation->EndFrame) {
				GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
				GetObj()->Animator3D()->SetCurTime(0.f);
				GetObj()->Animator3D()->SetStartFrameTime(time);
				m_bFindNear = true;
			}
		}
	}

	break;
	default:
		break;
	}

}

void CMinionScript::m_FFind()
{
}

CMinionScript::CMinionScript():CScript((UINT)SCRIPT_TYPE::MINIONSCRIPT),m_eState(MINION_STATE::WALK)
{
}

CMinionScript::CMinionScript(float _fSpeed, float _fRange, MINION_STATE _eState, MINION_CAMP _eCamp)
	: CScript((UINT)SCRIPT_TYPE::MINIONSCRIPT),m_fSpeed(_fSpeed),m_fRange(_fRange),m_eState(_eState),m_eCamp(_eCamp)
{
}

CMinionScript::~CMinionScript()
{
}
