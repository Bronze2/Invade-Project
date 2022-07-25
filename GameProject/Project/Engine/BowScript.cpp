#include "pch.h"
#include "BowScript.h"
#include "MeshRender.h"
#include "Animator3D.h"
#include "ArrowScript.h"
#include "CameraScript.h"
#include "CrossHairScript.h"
#include "Collider3D.h"
#include "Camera.h"

#include "FontMgr.h"

void CBowScript::Update()
{
	m_FAnimation();

	m_pTargetBone = const_cast<tMTBone*>(m_pTargetObject->MeshRender()->GetMesh()->GetBone(m_iTargetBoneIdx));

	Vec3 vTrans1 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].vTranslate;
	Vec3 vTrans2 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetNextFrameIdx()].vTranslate;
	Vec4 qRot1 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].qRot;
	Vec4 qRot2 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetNextFrameIdx()].qRot;
	
	float fFactor = m_pTargetObject->Animator3D()->GetRatio();

	Vec3 vTrans = Vec3::Lerp(vTrans1, vTrans2, fFactor);
	Vec4 qRot = Vec4::Lerp(qRot1, qRot2, fFactor);
	
	Vec3 vRot = Vec3(XMConvertToRadians(-20.f), XMConvertToRadians(83.f), XMConvertToRadians(0.f));

	Transform()->SetLocalPos(vTrans);
	Transform()->SetQuaternion(qRot);
	Transform()->SetLocalRot(vRot);

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject* pEmptyObject = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1]);
	CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];

	Vec3 vBowFront = GetObj()->GetParent()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vArrowDir;

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
		m_fArrowSpeed = 500.f;
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->Init();
		m_pArrow[m_iCurArrow]->SetActive(true);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK_READY);
		CFontMgr::GetInst()->AddText(L"BowTest", L"BBBow~", Vec2(0.9f, 0.f), Vec2(1.5f, 1.5f), Vec2(0.5f, 0.f), Vec4(1.f, 0.f, 1.f, 1.f));
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN)) {
		m_fArrowSpeed += 3000.f * DT;
		if (m_fArrowSpeed < 1500.f) {
			m_fArrowSpeed = 1500.f;
		}
		else if (m_fArrowSpeed > 3000.f) {
			m_fArrowSpeed = 3000.f;
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(true);
		}
	}

	// 크로스헤어
	if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
		CFontMgr::GetInst()->DeleteText(L"BowTest");
		CGameObject* pCrossHair = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"UI")->GetParentObj()[0]);
		CGameObject* pPlayer = GetObj()->GetParent();
		Vec3 vCrossHairDir = pCrossHair->GetScript<CCrossHairScript>()->GetDir();
		Vec3 vCrossHairPos = pCrossHair->GetScript<CCrossHairScript>()->GetPos();
		
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(false);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);

		Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();
		Vec3 vPlayerRot = pPlayer->Transform()->GetLocalRot();
		Vec3 vCamRot = pCamera->Transform()->GetLocalRot();
		float fCamRotDegree = XMConvertToDegrees(vCamRot.x);
		Vec3 vArrowPos = vPlayerPos + pPlayer->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -30.f + Vec3(0.f, pPlayer->Collider3D()->GetOffsetScale().y + fCamRotDegree, 0.f);		// -30.f는 pMainCam의 GetLocalPos.z
		// 화살fbx
		Vec3 vArrowRot = Vec3(XMConvertToRadians(XMConvertToDegrees(vPlayerRot.x) + 90.f), XMConvertToRadians(XMConvertToDegrees(vPlayerRot.y) + 80.f), XMConvertToRadians(XMConvertToDegrees(vPlayerRot.z)));
		// 깃대 안보임 XMConvertToRadians(XMConvertToDegrees(vPlayerRot.x) + 90.f) / 깃대보임 XMConvertToRadians(XMConvertToDegrees(vPlayerRot.y) + 90.f) 

		m_pArrow[m_iCurArrow]->ClearParent();
		m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vArrowPos);
		m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(vArrowRot);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetStartPos(vArrowPos);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vCrossHairDir);
		Vec3 vQtrnDir = GetObj()->GetParent()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetQtrnRotAxis(vQtrnDir);

		m_iCurArrow++;
		m_iPower = 1;
		if (m_iCurArrow > 19) {
			m_iCurArrow = 0;

		}
		m_bMaxCharged = false;
	}
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
	// 화살fbx
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	Ptr<CMeshData> pMeshData;
	for (int i = 0; i < 20; ++i) {
		if (GetObj()->GetParent()->GetLayerIdx() == 3) {
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\BlueArrow.mdat", L"MeshData\\BlueArrow.mdat");
		}
		else {
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\RedArrow.mdat", L"MeshData\\RedArrow.mdat");
		}

		m_pArrow[i] = pMeshData->Instantiate();
		m_pArrow[i]->SetName(L"Arrow");

		m_pArrow[i]->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		m_pArrow[i]->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(0.f), 0.f));
		m_pArrow[i]->Transform()->SetLocalScale(Vec3(1.f, 0.2f, 0.2f));

		m_pArrow[i]->AddComponent(new CCollider3D);
		m_pArrow[i]->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		m_pArrow[i]->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f)); 
		m_pArrow[i]->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));

		m_pArrow[i]->FrustumCheck(false);

		m_pArrow[i]->AddComponent(new CArrowScript(m_iType));
		pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);
		m_pArrow[i]->GetScript<CArrowScript>()->SetBow(GetObj());
		m_pArrow[i]->GetScript<CArrowScript>()->SetLayerIdx(GetObj()->GetLayerIdx());
		m_pArrow[i]->GetScript<CArrowScript>()->SetPlayer(m_pPlayer);
		m_pArrow[i]->GetScript<CArrowScript>()->SetType((UINT)(m_pPlayer->GetScript<CPlayerScript>()->GetType()));
		
		GetObj()->AddChild(m_pArrow[i]);
		m_pArrow[i]->Awake();

	}

	m_iCurArrow = 0;
	m_iPower = 1;
	m_bMaxCharged = false;
}


CBowScript::CBowScript() : CScript((UINT)SCRIPT_TYPE::BOWSCRIPT), m_fArcherLocation(20.f)
{
}

CBowScript::~CBowScript()
{
}
