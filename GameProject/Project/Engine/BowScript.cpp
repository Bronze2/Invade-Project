#include "pch.h"
#include "BowScript.h"
#include "MeshRender.h"
#include "Animator3D.h"
#include "ArrowScript.h"
#include "CameraScript.h"
#include "EmptyCameraScript.h"
#include "SceneMgr.h"
#include "Network.h"
#include "CrossHairScript.h"
#include "Collider3D.h"
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


	if (isMain)	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CGameObject* pEmptyObject = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1]);
		CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];
		//CGameObject* pEmptyCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[1];

		Vec3 vBowFront = GetObj()->GetParent()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		Vec3 vArrowDir;

		Vec3 vArrowPos = m_pArrow[m_iCurArrow]->Transform()->GetLocalPos();

		if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
			m_fArrowSpeed = 500.f;
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

		if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
			CGameObject* pCrossHair = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"UI")->GetParentObj()[0]);
			CGameObject* pPlayer = GetObj()->GetParent();
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetParentId(GetObj()->GetParent()->GetScript<CPlayerScript>()->m_GetId());
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->m_SetId(m_iCurArrow);

			Vec3 vCrossHairDir = pCrossHair->GetScript<CCrossHairScript>()->GetDir();
			Vec3 vCrossHairPos = pCrossHair->GetScript<CCrossHairScript>()->GetPos();

			//Vec3 vTargetDir = pCrossHair->GetScript<CCrossHairScript>()->GetDir();
			//Vec3 vStartPos = pCrossHair->GetScript<CCrossHairScript>()->GetPos();


			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(false);
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);
			//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vTargetDir);

			Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();
			Vec3 vPlayerRot = pPlayer->Transform()->GetLocalRot();
			Vec3 vCamRot = pCamera->Transform()->GetLocalRot();
			float fCamRotDegree = XMConvertToDegrees(vCamRot.x);
			Vec3 vArrowPos = vPlayerPos + pPlayer->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -30.f + Vec3(0.f, pPlayer->Collider3D()->GetOffsetScale().y + fCamRotDegree, 0.f);		
			Vec3 vArrowRot = Vec3(vPlayerRot.x, XMConvertToRadians(XMConvertToDegrees(vPlayerRot.y) + 80.f), XMConvertToRadians(XMConvertToDegrees(vPlayerRot.z)));

			Vec3 vArrowDir = pPlayer->Transform()->GetWorldDir(DIR_TYPE::UP);
			vArrowDir.y = vCrossHairDir.y;
			vArrowDir.Normalize();
			m_pArrow[m_iCurArrow]->ClearParent();
			m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vArrowPos);
			m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(vArrowRot);
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetStartPos(vArrowPos);
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vCrossHairDir);
			Vec3 vQtrnDir = GetObj()->GetParent()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetQtrnRotAxis(vQtrnDir);




			//Vec3 vBowRot = GetObj()->GetParent()->Transform()->GetLocalRot();
			//Vec3 vArrowPos = m_pArrow[m_iCurArrow]->Transform()->GetWorldPos() + Vec3(0.f, 0.f, 0.f);
			//Vec3 vArrowRot = Vec3(vBowRot.x, XMConvertToRadians(XMConvertToDegrees(vBowRot.y) + 80.f), vBowRot.z);
			//m_pArrow[m_iCurArrow]->ClearParent();

			//m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vArrowPos);
			//m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(vArrowRot);
			//CSceneMgr::GetInst()->set_arrowPos(GetObj()->GetParent()->GetScript<CPlayerScript>()->m_GetId(), m_iCurArrow, vArrowPos);
			//CSceneMgr::GetInst()->set_arrowRot(GetObj()->GetParent()->GetScript<CPlayerScript>()->m_GetId(), m_iCurArrow, vArrowRot);


			/*Network::GetInst()->send_arrow_packet(m_pPlayer->GetScript<CPlayerScript>()->m_GetId(), vArrowPos, vArrowRot, vTargetDir, m_fArrowSpeed, m_camp,
				GetCurArrow()->GetScript<CArrowScript>()->GetPacketSkill());*/

			Network::GetInst()->send_arrow_packet(m_pPlayer->GetScript<CPlayerScript>()->m_GetId(), 
				GetCurArrow()->GetScript<CArrowScript>()->GetPacketSkill());

			m_iCurArrow++;
			m_iPower = 1;
			if (m_iCurArrow > 19) {
				m_iCurArrow = 0;

			}
			m_bMaxCharged = false;
		}

		//if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
		//	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetParentId(GetObj()->GetParent()->GetScript<CPlayerScript>()->m_GetId());
		//	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->m_SetId(m_iCurArrow);
		//	Vec3 vTargetDir = pEmptyCamera->GetScript<CEmptyCameraScript>()->GetShootDir();
		//
		//	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);
		//	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(false);
		//	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);
		//	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vTargetDir);
		//	Vec3 vArrowPos = m_pArrow[m_iCurArrow]->Transform()->GetWorldPos() + Vec3(0.f, 0.f, 0.f);
		//	Vec3 vArrowRot = GetObj()->GetParent()->Transform()->GetLocalRot() + Vec3(0.f, XMConvertToRadians(90.f), 0.f);
		//
		//	m_pArrow[m_iCurArrow]->ClearParent();
		//	m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vArrowPos);
		//	m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(vArrowRot);
		//	CSceneMgr::GetInst()->set_arrowPos(GetObj()->GetParent()->GetScript<CPlayerScript>()->m_GetId(),m_iCurArrow,vArrowPos);
		//	CSceneMgr::GetInst()->set_arrowRot(GetObj()->GetParent()->GetScript<CPlayerScript>()->m_GetId(),m_iCurArrow,vArrowRot);
		//
		//	Network::GetInst()->send_arrow_packet(m_iCurArrow,vArrowPos, vArrowRot, vTargetDir, m_fArrowSpeed, m_camp,
		//		GetCurArrow()->GetScript<CArrowScript>()->GetPacketSkill());
		//	//GetCurArrow()->s
		//	m_iCurArrow++;
		//	m_iPower = 1;
		//	if (m_iCurArrow > 19) {
		//		m_iCurArrow = 0;
		//
		//	}
		//	m_bMaxCharged = false;
		//}
	}
}
#include "SkillMgr.h"


void CBowScript::CreateArrow(PACKET_SKILL skill)
{
	cout << "생성 - " << m_iCurArrow  <<endl;
	CGameObject* pPlayer = GetObj()->GetParent();
	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetParentId(GetObj()->GetParent()->GetScript<CPlayerScript>()->m_GetId());
	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->m_SetId(m_iCurArrow);
	//Vec3 vTargetDir = pCrossHair->GetScript<CCrossHairScript>()->GetDir();
	//Vec3 vStartPos = pCrossHair->GetScript<CCrossHairScript>()->GetPos();
	if (skill == PACKET_SKILL::Z_FIRE)
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSkill(CSkillMgr::GetInst()->FindSkill(9));
	if (skill == PACKET_SKILL::Z_TUNDER)
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSkill(CSkillMgr::GetInst()->FindSkill(7));
	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);
	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(false);
	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);
	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetisMain(false);
	//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vTargetDir);

	Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();
	Vec3 vPlayerRot = pPlayer->Transform()->GetLocalRot();
	Vec3 vArrowRot = Vec3(vPlayerRot.x, XMConvertToRadians(XMConvertToDegrees(vPlayerRot.y) + 80.f), XMConvertToRadians(XMConvertToDegrees(vPlayerRot.z)));

	Vec3 vArrowDir = pPlayer->Transform()->GetWorldDir(DIR_TYPE::UP);
	vArrowDir.Normalize();
	m_pArrow[m_iCurArrow]->ClearParent();
	m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(vArrowRot);
	m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vPlayerPos);
	m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetLerp(vPlayerPos,Vec4(0,0,0,0));

	m_pArrow[m_iCurArrow]->SetActive(true);

	Vec3 vQtrnDir = GetObj()->GetParent()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	m_iCurArrow++;
	m_iPower = 1;
	if (m_iCurArrow > 19) {
		m_iCurArrow = 0;

	}
	m_bMaxCharged = false;

}
void CBowScript::InitArrow(int ArrowId, Vec3 Pos, Vec3 Rot, PACKET_SKILL skill)
{


	m_pArrow[ArrowId]->GetScript<CArrowScript>()->Init();
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetParentId(GetObj()->GetParent()->GetScript<CPlayerScript>()->m_GetId());
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->m_SetId(ArrowId);

	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);
	if(skill == PACKET_SKILL::Z_FIRE)
		m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetSkill(CSkillMgr::GetInst()->FindSkill(9));
	if (skill == PACKET_SKILL::Z_TUNDER)
		m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetSkill(CSkillMgr::GetInst()->FindSkill(7));
	m_pArrow[ArrowId]->ClearParent();
	m_pArrow[ArrowId]->Transform()->SetLocalPos(Pos);
	m_pArrow[ArrowId]->Transform()->SetLocalRot(Rot);

	CSceneMgr::GetInst()->set_arrowPos(GetObj()->GetParent()->GetScript<CPlayerScript>()->m_GetId(), ArrowId, Pos);
	CSceneMgr::GetInst()->set_arrowRot(GetObj()->GetParent()->GetScript<CPlayerScript>()->m_GetId(), ArrowId, Rot);

	//m_pArrow[ArrowId]->SetActive(true);

	//cout << "화살 생성 -other"<< m_pArrow[ArrowId]->GetScript<CArrowScript>()->GetParentId()<<"에서 받아옴" << endl;

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

void CBowScript::DeleteArrow(int ArrowId)
{
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetState(ARROW_STATE::IDLE);
	m_pArrow[ArrowId]->Transform()->SetLocalPos(Vec3(-1000, -1000, -1000));

	//m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetState(ARROW_STATE::IDLE);
	//m_pArrow[ArrowId]->Transform()->SetLocalPos(Vec3(1000, 1000, 1000));
	//m_pArrow[ArrowId]->SetActive(false);
}

void CBowScript::UpdateArrow(int ArrowId , Vec3 LocalPos, Vec4 Quaternion)
{
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetLerp(LocalPos, Quaternion);
}


void CBowScript::Awake()
{
	pBlackTex = CResMgr::GetInst()->FindRes<CTexture>(L"Black");
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (int i = 0; i < 20; ++i) {
		m_pArrow[i] = new CGameObject;
		m_pArrow[i]->SetName(L"Arrow");

		m_pArrow[i]->AddComponent(new CTransform());
		m_pArrow[i]->Transform()->SetLocalScale(Vec3(80.f, 1.f, 1.f));
		m_pArrow[i]->AddComponent(new CMeshRender);
		m_pArrow[i]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
		m_pArrow[i]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
		m_pArrow[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlackTex.GetPointer());
		m_pArrow[i]->AddComponent(new CArrowScript(m_iType));
		
		m_pArrow[i]->AddComponent(new CCollider3D);
		m_pArrow[i]->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		m_pArrow[i]->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		m_pArrow[i]->Collider3D()->SetOffsetPos(Vec3(0.f,0.f,0.f));
		
		m_pArrow[i]->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		//pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);
		m_pArrow[i]->SetActive(false);
		m_pArrow[i]->GetScript<CArrowScript>()->SetBow(GetObj());
		m_pArrow[i]->GetScript<CArrowScript>()->SetLayerIdx(GetObj()->GetLayerIdx());
		m_pArrow[i]->GetScript<CArrowScript>()->SetPlayer(m_pPlayer);
		m_pArrow[i]->GetScript<CArrowScript>()->SetType((UINT)(m_pPlayer->GetScript<CPlayerScript>()->GetType()));
		m_pArrow[i]->GetScript<CArrowScript>()->SetPacketSkill(PACKET_SKILL::NONE);
		m_pArrow[i]->GetScript<CArrowScript>()->SetisMain(m_pPlayer->GetScript<CPlayerScript>()->GetIsMain());
		m_pArrow[i]->GetScript<CArrowScript>()->SetCamp(m_pPlayer->GetScript<CPlayerScript>()->GetCamp());

		m_pArrow[i]->GetScript<CArrowScript>()->Awake();

		pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);



		GetObj()->AddChild(m_pArrow[i]);
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