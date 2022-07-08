#include "pch.h"
#include "BowScript.h"
#include "MeshRender.h"
#include "Animator3D.h"
#include "ArrowScript.h"
#include "CameraScript.h"
#include "CrossHairScript.h"
#include "Collider3D.h"
#include "Trail.h"

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
		m_fArrowSpeed = 200.f;
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->Init();
		m_pArrow[m_iCurArrow]->SetActive(true);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK_READY);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN)) {
		m_fArrowSpeed += 2000.f * DT;
		if (m_fArrowSpeed > 1000.f) {
			m_fArrowSpeed = 1000.f;
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(true);
		}
	}

	// 크로스헤어
	if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
		CGameObject* pCrossHair = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"UI")->GetParentObj()[0]);
		Vec3 vTargetDir = pCrossHair->GetScript<CCrossHairScript>()->GetDir();
		Vec3 vStartPos = pCrossHair->GetScript<CCrossHairScript>()->GetPos();
		
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(false);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vTargetDir);

		cout << vTargetDir.x << ", " << vTargetDir.y << ", " << vTargetDir.z << endl;

		Vec3 vPlayerRot = GetObj()->GetParent()->Transform()->GetLocalRot();
		Vec3 vCamRot = pCamera->Transform()->GetLocalRot();
		float fCamRotDegree = XMConvertToDegrees(vCamRot.x);
		Vec3 vArrowPos = m_pArrow[m_iCurArrow]->Transform()->GetWorldPos();
		Vec3 vArrowRot = Vec3(vPlayerRot.x, XMConvertToRadians(XMConvertToDegrees(vPlayerRot.y) + 80.f), XMConvertToRadians(XMConvertToDegrees(vPlayerRot.z)));
		//Vec4 qArrowQtrn = GetObj()->GetParent

		m_pArrow[m_iCurArrow]->ClearParent();

		m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vArrowPos);
		m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(vArrowRot);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetStartPos(vArrowPos);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetStartAngle(fCamRotDegree);

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
	pBlackTex = CResMgr::GetInst()->FindRes<CTexture>(L"Black");
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	Ptr<CMaterial> pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
	CResMgr::GetInst()->AddRes(L"ArrowMtrl", pMtrl);
	for (int i = 0; i < 20; ++i) {
		m_pArrow[i] = new CGameObject;
		m_pArrow[i]->SetName(L"Arrow");

		m_pArrow[i]->AddComponent(new CTransform());
		m_pArrow[i]->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		m_pArrow[i]->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(80.f), 0.f));
		m_pArrow[i]->Transform()->SetLocalScale(Vec3(80.f, 1.f, 1.f));

		m_pArrow[i]->AddComponent(new CMeshRender);
		m_pArrow[i]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
		
		m_pArrow[i]->MeshRender()->SetMaterial(pMtrl);
		m_pArrow[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlackTex.GetPointer());

		m_pArrow[i]->AddComponent(new CCollider3D);
		m_pArrow[i]->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		m_pArrow[i]->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f)); 
		m_pArrow[i]->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));

		m_pArrow[i]->AddComponent(new CArrowScript(m_iType));
		pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);
		m_pArrow[i]->SetActive(false);
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
