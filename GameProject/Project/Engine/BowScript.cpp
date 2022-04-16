#include "pch.h"
#include "BowScript.h"
#include "MeshRender.h"
#include "Animator3D.h"

void CBowScript::Update()
{
	m_pTargetBone = const_cast<tMTBone*>(m_pTargetObject->MeshRender()->GetMesh()->GetBone(m_iTargetBoneIdx));
	
	Transform()->SetLocalPos(m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].vTranslate);
	Transform()->SetLocalRot(m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].qRot);

	m_FAnimation();
}

void CBowScript::m_FAnimation()
{
	if (m_ePrevState != m_eState) {
		switch (m_eState)
		{
		case BOW_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE")) {
				m_pCurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE");
				m_FSetAnimation();
				m_ePrevState = BOW_STATE::IDLE;
			}
		}
		break;
		case BOW_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK")) {
				m_pCurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK");
				m_FSetAnimation();
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
					m_FSetAnimation();
				}
			}
		}
		break;
		case BOW_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK")) {
				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimation->EndFrame) {
					m_FSetAnimation();
				}
			}
		}
		break;

		default:
			break;
		}
	}
}

void CBowScript::m_FSetAnimation()
{
	GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimation->StartFrame);
	double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
	GetObj()->Animator3D()->SetCurTime(0.f);
	GetObj()->Animator3D()->SetStartFrameTime(time);
}


void CBowScript::Init()
{
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE")) {
		m_pCurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE");
	}
}

void CBowScript::Awake()
{
}


CBowScript::CBowScript() : CScript((UINT)SCRIPT_TYPE::BOWSCRIPT)
{
}

CBowScript::~CBowScript()
{
}
