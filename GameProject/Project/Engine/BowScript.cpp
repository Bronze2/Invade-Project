#include "pch.h"
#include "BowScript.h"
#include "MeshRender.h"
#include "Animator3D.h"

void CBowScript::Update()
{
	m_pTargetBone = const_cast<tMTBone*>(m_pTargetObject->MeshRender()->GetMesh()->GetBone(m_iTargetBoneIdx));

#ifdef _ANIMATION_TEST
	Transform()->SetLocalPos(m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetNextFrameIdx()].vTranslate);
	Transform()->SetLocalRot(m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetNextFrameIdx()].qRot);

#else
	Transform()->SetLocalPos(m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].vTranslate);
	Transform()->SetLocalRot(m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].qRot);
#endif

	m_FAnimation();
}

void CBowScript::m_FAnimation()
{
	if (m_ePrevState != m_eState) {
		switch (m_eState)
		{
		case BOW_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(STATE_IDLE)) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(STATE_IDLE);
				GetObj()->Animator3D()->SetCurClipIndex((UINT)BOW_STATE::IDLE);
				GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)BOW_STATE::IDLE, 0.f);
				GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
				m_ePrevState = BOW_STATE::IDLE;
			}
		}
		break;
		case BOW_STATE::ATTACK_READY:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(STATE_ATTACK_READY)) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(STATE_ATTACK_READY);
				GetObj()->Animator3D()->SetCurClipIndex((UINT)BOW_STATE::ATTACK_READY);
				GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)BOW_STATE::ATTACK_READY, 0.f);
				GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
				m_ePrevState = BOW_STATE::ATTACK_READY;
			}
		}
		break;
		case BOW_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(STATE_ATTACK)) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(STATE_ATTACK);
				GetObj()->Animator3D()->SetCurClipIndex((UINT)BOW_STATE::ATTACK);
				GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)BOW_STATE::ATTACK, 0.f);
				GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
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
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(STATE_IDLE)) {
				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame) {
					GetObj()->Animator3D()->SetCurClipIndex((UINT)BOW_STATE::IDLE);
					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
					GetObj()->Animator3D()->SetCurTime((UINT)BOW_STATE::IDLE, 0.f);
					GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
				}
			}
		}
		break;
		case BOW_STATE::ATTACK_READY:
		{
		}
		break;
		case BOW_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(STATE_ATTACK)) {
				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame)
				{
					m_eState = BOW_STATE::IDLE;
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
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(STATE_IDLE)) {
		m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(STATE_IDLE);
		GetObj()->Animator3D()->SetCurClipIndex((UINT)BOW_STATE::IDLE);
		GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
		GetObj()->Animator3D()->SetCurTime(0.f);
		GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
		m_eState = BOW_STATE::IDLE;
		m_ePrevState = BOW_STATE::IDLE;
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
