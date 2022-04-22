#include "pch.h"
#include "PlayerScript.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "Camera.h"
#include"CameraScript.h"
#include "Animator3D.h"
#include "Network.h"
#include "BowScript.h"
#include "TimeMgr.h"

void CPlayerScript::m_FAnimation()
{
	if (m_ePrevState != m_eState) {
		// Blend �ʿ�
		switch (m_eState)
		{
		case PLAYER_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::IDLE);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::IDLE, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::IDLE;
			}
		}
		break;
		case PLAYER_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::WALK);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::WALK, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::WALK;
			}
		}
		break;
		case PLAYER_STATE::JUMP:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::JUMP);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::JUMP, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::JUMP;
			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK_READY);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK_READY, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK_READY;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK_READY);
			}
		}
		break;
		case PLAYER_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::ATTACK);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::ATTACK, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::ATTACK;
				GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::ATTACK);
			}
		}
		break;
		case PLAYER_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::DIE);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::DIE, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = PLAYER_STATE::DIE;
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
		case PLAYER_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");

					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::IDLE);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
					}
				}

			}
		}
		break;
		case PLAYER_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");

					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::WALK);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
					}
				}

			}

			//#ifdef _ANIMATION_TEST
			//         if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
			//            GetObj()->Animator3D()->SetBlendState(true);
			//            GetObj()->Animator3D()->SetNextClipIndex((UINT)PLAYER_STATE::WALK);
			//            GetObj()->Animator3D()->SetNextFrameIdx(m_pCurAnimClip->iStartFrame);
			//            GetObj()->Animator3D()->SetCurTime((UINT)PLAYER_STATE::WALK, 0.f);
			//            GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
			//         }
			//#else

			//#endif
		}
		break;
		case PLAYER_STATE::JUMP:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"JUMP");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						m_eState = PLAYER_STATE::IDLE;
					}
				}

			}
		}
		break;
		case PLAYER_STATE::ATTACK_READY:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK_READY");
				}
			}
		}
		break;
		case PLAYER_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						m_eState = PLAYER_STATE::IDLE;
						GetObj()->GetChild()[0]->GetScript<CBowScript>()->SetState(BOW_STATE::IDLE);
					}
				}
			}
		}
		break;
		case PLAYER_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");

					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame())) {
						m_eState = PLAYER_STATE::IDLE;
					}
				}
			}
		}
		break;

		default:
			break;
		}
	}
}


void CPlayerScript::Init()
{
	m_eState = PLAYER_STATE::IDLE;
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
		m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
		GetObj()->Animator3D()->SetCurClipIndex((UINT)PLAYER_STATE::IDLE);
		GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
		GetObj()->Animator3D()->SetCurTime(0.f);
		GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
		m_eState = PLAYER_STATE::IDLE;
		m_ePrevState = PLAYER_STATE::IDLE;
	}
}

void CPlayerScript::Awake()
{
	pBlackTex = CResMgr::GetInst()->FindRes<CTexture>(L"Black");
	CScene* pCurScene=CSceneMgr::GetInst()->GetCurScene();
	for (int i = 0; i < 20; ++i) {
		m_pArrow[i] = new CGameObject;
		m_pArrow[i]->SetName(L"Arrow");
	
	
		m_pArrow[i]->AddComponent(new CTransform());
		Vec3 vPos = m_pArrow[i]->Transform()->GetLocalPos();
		m_pArrow[i]->Transform()->SetLocalPos(vPos);
		m_pArrow[i]->Transform()->SetLocalScale(Vec3(100.f, 1.f, 1.f));

		m_pArrow[i]->AddComponent(new CMeshRender);
		m_pArrow[i]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
		m_pArrow[i]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//	m_pArrow[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlackTex.GetPointer());

		m_pArrow[i]->AddComponent(new CCollider2D);
		m_pArrow[i]->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);

		m_pArrow[i]->AddComponent(new CArrowScript(m_iType));
		pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);
		m_pArrow[i]->SetActive(false);
	}

	m_iCurArrow = 0;
	m_iPower = 1;
}

void CPlayerScript::Update()
{

	// Z-up To Y-up
	Vec3 vDirUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vDirFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	Transform()->SetWorldDir(DIR_TYPE::UP, vDirFront);
	Transform()->SetWorldDir(DIR_TYPE::FRONT, vDirUp);

	if (isMain ) {

		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vRot = Transform()->GetLocalRot();
		if (CTimeMgr::GetInst()->GetPlayerMoveFPS()) {
			if (KEY_HOLD(KEY_TYPE::KEY_W)) {
				Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
				Network::GetInst()->send_move_packet(0, vFront.x, vFront.y, vFront.z, 0);
				m_eState = PLAYER_STATE::WALK;
			}
			if (KEY_HOLD(KEY_TYPE::KEY_S)) {
				Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
				Network::GetInst()->send_move_packet(0, vBack.x, vBack.y, vBack.z, 0);
				m_eState = PLAYER_STATE::WALK;
			}
			if (KEY_HOLD(KEY_TYPE::KEY_A)) {
				Vec3 vLeft = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
				Network::GetInst()->send_move_packet(0, vLeft.x, vLeft.y, vLeft.z, 0);
				m_eState = PLAYER_STATE::WALK;
			}
			if (KEY_HOLD(KEY_TYPE::KEY_D)) {
				Vec3 vRight = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
				Network::GetInst()->send_move_packet(0, vRight.x, vRight.y, vRight.z, 0);
				m_eState = PLAYER_STATE::WALK;
			}

			if (KEY_AWAY(KEY_TYPE::KEY_W)) {
				Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
				Network::GetInst()->send_move_packet(0, vFront.x, vFront.y, vFront.z, 1);
				m_eState = PLAYER_STATE::IDLE;
			}
			if (KEY_AWAY(KEY_TYPE::KEY_S)) {
				Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
				Network::GetInst()->send_move_packet(0, vBack.x, vBack.y, vBack.z, 1);
				m_eState = PLAYER_STATE::IDLE;
			}
			if (KEY_AWAY(KEY_TYPE::KEY_A)) {
				Vec3 vLeft = Transform()->GetWorldDir(DIR_TYPE::FRONT);
				Network::GetInst()->send_move_packet(0, vLeft.x, vLeft.y, vLeft.z, 1);
				m_eState = PLAYER_STATE::IDLE;
			}
			if (KEY_AWAY(KEY_TYPE::KEY_D)) {
				Vec3 vRight = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
				Network::GetInst()->send_move_packet(0, vRight.x, vRight.y, vRight.z, 1);
				m_eState = PLAYER_STATE::IDLE;
			}
		}

		//Arrow 
	/*	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
			CGameObject* pObj = GetObj()->GetChild()[0];
			Vec3 vPos = pObj->Transform()->GetLocalPos();
			Vec3 vRight = pObj->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
			Vec3 vFront = pObj->Transform()->GetLocalDir(DIR_TYPE::FRONT);
			m_vRestorePos = vPos;
			vPos += vRight * 10.f;
			vPos += vFront * 10.f;
			pObj->Transform()->SetLocalPos(vPos);
			m_fArrowSpeed = 200.f;

		}
		if (KEY_HOLD(KEY_TYPE::KEY_LBTN)) {
			m_fArrowSpeed += 1000.f * DT;
			if (m_fArrowSpeed > 2000.f) {
				m_fArrowSpeed = 2000.f;
			}
		}

		if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
			CGameObject* pObj = GetObj()->GetChild()[0];
			Vec3 vPos = pObj->Transform()->GetLocalPos();
			Vec3 vRight = -pObj->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
			Vec3 vFront = -pObj->Transform()->GetLocalDir(DIR_TYPE::FRONT);
			m_vRestorePos = vPos;
			vPos += vRight * 10.f;
			vPos += vFront * 10.f;
			pObj->Transform()->SetLocalPos(vPos);
			m_pArrow[m_iCurArrow]->SetActive(true);
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->Init();
			vRight = pObj->Transform()->GetWorldDir(DIR_TYPE::FRONT);
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vRight);
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetVelocityX();
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetVelocityZ();


			Vec2 xzValue = GetDiagnal(m_fArcherLocation, vRight.x, vRight.z);


			CCameraScript* p = dynamic_cast<CCameraScript*>(GetObj()->GetChild()[0]->GetScripts()[0]);
			float fDegree = p->GetDegree();
			float fDegree2 = fDegree;
			fDegree *= -1.f;


			float yValue = sin(XMConvertToRadians(fDegree)) * m_fArcherLocation;

			Vec3 vArrowPos = Vec3(GetObj()->Transform()->GetLocalPos().x + xzValue.x, GetObj()->Transform()->GetLocalPos().y + 70 + yValue, GetObj()->Transform()->GetLocalPos().z + xzValue.y);


			m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vArrowPos);
			fDegree *= -1.f;



			m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(Vec3(GetObj()->Transform()->GetLocalRot().x, GetObj()->Transform()->GetLocalRot().y, GetObj()->Transform()->GetLocalRot().z));
			if (m_iCurArrow == 0) {
				int a = 0;
			}

			Vec3 vFront2 = vArrowPos;
			Vec3 vRight2 = Vec3(1, 0, 0);
			auto k = XMLoadFloat3(&vRight2);
			auto m = XMLoadFloat4x4(&m_pArrow[m_iCurArrow]->Transform()->GetWorldMat());
			auto r = XMVector3TransformNormal(k, m);
			XMFLOAT3 result;
			XMStoreFloat3(&result, XMVector3Normalize(r));

			float flength = sqrt(pow(result.x, 2) + pow(result.z, 2));


			vArrowPos.x += result.x;
			vArrowPos.z += result.z;
			float xValue = sqrt(pow(m_fArcherLocation, 2) - pow(yValue, 2));
			float xValue2 = xValue + flength;
			float fyValue2 = yValue * xValue2 / xValue;

			float SubeyValue2Value = fyValue2 - yValue;
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetFallSpeen(SubeyValue2Value);

			vArrowPos.y += SubeyValue2Value;
			Vec3 vTarget = vArrowPos - vFront2;

			vTarget.Normalize();
			float vDotValue = Dot(vTarget, result);
			Vec3 vCrossValue;
			if (vTarget.y > 0.f) {
				vCrossValue = Cross(vTarget, result);
			}
			else {
				vCrossValue = Cross(result, vTarget);
			}


			if (vCrossValue != Vec3(0.f, 0.f, 0.f)) {

				XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), XMConvertToRadians(fDegree2));
				m_pArrow[m_iCurArrow]->Transform()->SetQuaternion(XMQuaternionMultiply(m_pArrow[m_iCurArrow]->Transform()->GetQuaternion(), xmmatrix));

			}

			m_iCurArrow++;
			m_iPower = 1;
			if (m_iCurArrow > 19) {
				m_iCurArrow = 0;

			}

		}
		*/
		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		if (!m_bCheckStartMousePoint) {
			m_bCheckStartMousePoint = true;
		}
		else {
			vRot.y += vDrag.x * DT * 1.f;
			float fDegree = XMConvertToDegrees(vRot.y);
			if (fDegree < -360) {
				fDegree += 360.f;
				vRot.y = XMConvertToRadians(fDegree);
			}
			else if (fDegree > 360) {
				fDegree -= 360.f;
				vRot.y = XMConvertToRadians(fDegree);
			}
			Network::GetInst()->send_rotation_packet(vRot);
		}
		
		
		if (KEY_TAB(KEY_TYPE::KEY_NUM4))
		{
			m_eState = PLAYER_STATE::JUMP;
		}

		if (KEY_TAB(KEY_TYPE::KEY_NUM5))
		{
			m_eState = PLAYER_STATE::DIE;
		}


		Transform()->SetLocalRot(vRot);
		Transform()->SetLocalPos(vPos);
	}
	Update_LerpPos();
	m_FAnimation();

}

void CPlayerScript::SetState(int state)
{
	if (state == 0)
	{
		m_eState = PLAYER_STATE::WALK;

	}
	if (state == 1)
	{
		m_eState = PLAYER_STATE::IDLE;
	}
}

void CPlayerScript::Update_LerpPos()
{
	Vec3 vPos = Vec3::Lerp(Transform()->GetLocalPos(), m_LerpPos, DT*5.f); 

	Transform()->SetLocalPos(vPos);
}



CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT), m_bCheckStartMousePoint(false), m_fArcherLocation(20.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

