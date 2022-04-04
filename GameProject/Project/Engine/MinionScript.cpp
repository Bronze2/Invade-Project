#include "pch.h"
#include "MinionScript.h"
#include "Animator3D.h"

void CMinionScript::Init()
{
	m_eState = MINION_STATE::WALK;
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK")) {
		m_CurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK");
		GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
		double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
		GetObj()->Animator3D()->SetCurTime(0.f);
		GetObj()->Animator3D()->SetStartFrameTime(time);
	}	
}


void CMinionScript::Update()
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
		break;
	case MINION_STATE::FIND:
		break;
	case MINION_STATE::DIE:
		break;
	default:
		break;
	}
	Vec3 vPos = GetObj()->Transform()->GetLocalPos();
	vPos.z -= 50.f * DT;

	GetObj()->Transform()->SetLocalPos(vPos);



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
