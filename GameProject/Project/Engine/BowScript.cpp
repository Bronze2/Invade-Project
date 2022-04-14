#include "pch.h"
#include "BowScript.h"
#include "MeshRender.h"
#include "Animator3D.h"


void CBowScript::m_FAnimation()
{
	if (m_ePrevState != m_eState) {
		switch (m_eState)
		{
		case BOW_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE")) {
				m_pCurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE");
				GetObj()->Animator3D()->SetFrmaeIdx(m_pCurAnimation->StartFrame);
				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
				GetObj()->Animator3D()->SetCurTime(0.f);
				GetObj()->Animator3D()->SetStartFrameTime(time);
				m_ePrevState = BOW_STATE::IDLE;
			}
		}
		break;
		case BOW_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK")) {
				m_pCurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK");
				GetObj()->Animator3D()->SetFrmaeIdx(m_pCurAnimation->StartFrame);
				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
				GetObj()->Animator3D()->SetCurTime(0.f);
				GetObj()->Animator3D()->SetStartFrameTime(time);
				m_ePrevState = BOW_STATE::ATTACK;
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
		case BOW_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE")) {
				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimation->EndFrame) {
					GetObj()->Animator3D()->SetFrmaeIdx(m_pCurAnimation->StartFrame);
					double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
					GetObj()->Animator3D()->SetCurTime(0.f);
					GetObj()->Animator3D()->SetStartFrameTime(time);
				}
			}
		}
		break;
		case BOW_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK")) {
				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimation->EndFrame) {
					GetObj()->Animator3D()->SetFrmaeIdx(m_pCurAnimation->StartFrame);
					double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
					GetObj()->Animator3D()->SetCurTime(0.f);
					GetObj()->Animator3D()->SetStartFrameTime(time);
				}
			}
		}
		break;

		default:
			break;
		}
	}
}

void CBowScript::Init()
{
	m_eState = BOW_STATE::IDLE;
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE")) {
		m_pCurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE");
		GetObj()->Animator3D()->SetFrmaeIdx(m_pCurAnimation->StartFrame);
		double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
		GetObj()->Animator3D()->SetCurTime(0.f);
		GetObj()->Animator3D()->SetStartFrameTime(time);
		m_eState = BOW_STATE::IDLE;
		m_ePrevState = BOW_STATE::IDLE;
	}
}

void CBowScript::Awake()
{
}

void CBowScript::Update()
{
	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		m_eState = BOW_STATE::ATTACK;
	}
	if (KEY_AWAY(KEY_TYPE::KEY_LBTN))
	{
		m_eState = BOW_STATE::IDLE;
	}

	m_FAnimation();

}

CBowScript::CBowScript() : CScript((UINT)SCRIPT_TYPE::BOWSCRIPT)
{
}

CBowScript::~CBowScript()
{
}
