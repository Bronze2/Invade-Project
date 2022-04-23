#include "pch.h"
#include "BowScript.h"
#include "MeshRender.h"
#include "Animator3D.h"

void CBowScript::Update()
{
	m_FAnimation();

	m_pTargetBone = const_cast<tMTBone*>(m_pTargetObject->MeshRender()->GetMesh()->GetBone(m_iTargetBoneIdx));

	Vec3 vTrans1 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].vTranslate;
	Vec3 vTrans2 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetNextFrameIdx()].vTranslate;
	Vec4 qRot1 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].qRot;
	Vec4 qRot2 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetNextFrameIdx()].qRot;
	
	float factor = m_pTargetObject->Animator3D()->GetRatio();

	Vec3 vTrans = vTrans1 * (1.f - factor) + vTrans2 * factor;
	Vec4 qRot = qRot1 * (1.f - factor) + qRot2 * factor;
	
	Vec3 vRot = Vec3(XMConvertToRadians(-20.f), XMConvertToRadians(83.f), XMConvertToRadians(0.f));
	
	Transform()->SetLocalPos(vTrans);
	Transform()->SetQuaternion(qRot);
	Transform()->SetLocalRot(vRot);

}

void CBowScript::m_FAnimation()
{
	if (m_ePrevState != m_eState) {
		switch (m_eState)
		{
		case BOW_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
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
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY");
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
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
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
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
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
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY")) {
				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY");
					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
				}
			}
		}
		break;
		case BOW_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
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
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
		m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
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
