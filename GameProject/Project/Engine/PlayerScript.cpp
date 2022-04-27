#include "pch.h"
#include "PlayerScript.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "Camera.h"
#include"CameraScript.h"
#include "Animator3D.h"

#include "ProjectileScript.h"

#include "BowScript.h"

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
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iEndFrame);
					}
				}
			}
		}
		break;
		case PLAYER_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame)) {
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
#ifndef ARROW_TEST
	pBlackTex = CResMgr::GetInst()->FindRes<CTexture>(L"Black");
	CScene* pCurScene=CSceneMgr::GetInst()->GetCurScene();
	CGameObject* pBow = GetObj()->GetChild()[0];
	for (int i = 0; i < 20; ++i) {
		m_pArrow[i] = new CGameObject;
		m_pArrow[i]->SetName(L"Arrow");
	
	
		m_pArrow[i]->AddComponent(new CTransform());
		//Vec3 vPos = m_pArrow[i]->Transform()->GetLocalPos();
		m_pArrow[i]->Transform()->SetLocalPos(Vec3(0.f, 40.f, 0.f));
		m_pArrow[i]->Transform()->SetLocalScale(Vec3(80.f, 1.f, 1.f));			// 자식 아닐 땐 (80.f, 0.f, 1.f)
		m_pArrow[i]->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(0.f), 0.f));

		//m_pArrow[i]->Transform()->SetLocalPos(Vec3(-10.f, 75.f, -140.f));
		//m_pArrow[i]->Transform()->SetLocalScale(Vec3(80.f, 1.f, 1.f));
		//m_pArrow[i]->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));

		m_pArrow[i]->AddComponent(new CMeshRender);
		m_pArrow[i]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
		m_pArrow[i]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//	m_pArrow[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlackTex.GetPointer());

		m_pArrow[i]->AddComponent(new CCollider2D);
		m_pArrow[i]->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);

		m_pArrow[i]->AddComponent(new CArrowScript(m_iType));
		m_pArrow[i]->GetScript<CArrowScript>()->SetBow(GetObj()->GetChild()[0]);
		pCurScene->FindLayer(L"Blue")->AddGameObject(m_pArrow[i]);
		m_pArrow[i]->SetActive(false);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(false);
	}

	m_iCurArrow = 0;
	m_iPower = 1;
	m_bMaxCharged = false;
#endif

	m_fMoveSpeed = 300.f;
}

void CPlayerScript::Update()
{
	// Z-up To Y-up
	Vec3 vDirUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vDirFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Transform()->SetWorldDir(DIR_TYPE::UP, vDirFront);
	Transform()->SetWorldDir(DIR_TYPE::FRONT, vDirUp);

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject* pEmptyObject = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1]);
	CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];
	CCameraScript* pCameraScript = pCamera->GetScript<CCameraScript>();
	CGameObject* pBow = GetObj()->GetChild()[0];

	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vPos2 = Transform()->GetLocalPos();
	Vec3 vPos3 = Transform()->GetWorldPos();
	Vec3 vRot = Transform()->GetLocalRot();
	Vec3 vRestoreRot;
	int rotydegree = XMConvertToDegrees(vRot.y);
	int reminder = rotydegree % 360 + 90;

	if (0 <= reminder && reminder <= 180) {
		m_bCheckDegree = true;
	}
	else {
		m_bCheckDegree = false;
	}
	if (KEY_HOLD(KEY_TYPE::KEY_W) && KEY_HOLD(KEY_TYPE::KEY_A)) {
		int a = 0;
	}


	if ((KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_D)) && KEY_NONE(KEY_TYPE::KEY_LBTN)) {
		
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT); 
		m_fRotateDegree = XMConvertToDegrees(pEmptyObject->Transform()->GetLocalRot().y) - 90.f;

		if (KEY_HOLD(KEY_TYPE::KEY_W)) {
			vRot.y = 0.f;
		}
		if (KEY_HOLD(KEY_TYPE::KEY_S)) {
			vRot.y = XMConvertToRadians(180.f);
		}
		if (KEY_HOLD(KEY_TYPE::KEY_A)) {
			vRot.y = XMConvertToRadians(-90.f);
		}
		if (KEY_HOLD(KEY_TYPE::KEY_D)) {
			vRot.y = XMConvertToRadians(90.f);
		}

		vPos += vFront * m_fMoveSpeed * DT;
		vPos.y = 0.f;
		vRot.y += XMConvertToRadians(m_fRotateDegree);
		m_FColCheck(vPos3, vPos);
		m_eState = PLAYER_STATE::WALK;
	}

	if ((KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_S) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_D)) && KEY_NONE(KEY_TYPE::KEY_LBTN)) {
		m_eState = PLAYER_STATE::IDLE;
	}
	
#ifndef ARROW_TEST
	Vec3 vCurArrowPos = vPos + m_pArrow[m_iCurArrow]->Transform()->GetLocalPos();
	Vec3 vCameraLocalFront = pCamera->Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vEmptyObjLocalFront = pEmptyObject->Transform()->GetLocalDir(DIR_TYPE::UP);

#endif

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
		m_fRotateDegree = XMConvertToDegrees(pEmptyObject->Transform()->GetLocalRot().y) - 90.f;
		// 공격 시 무조건 카메라가 바라보는 방향으로 플레이어 회전시키기 (화살 개발 이후 주석 풀기)
		vRot.y = XMConvertToRadians(m_fRotateDegree);	// 5.f 더 회전시킬건지?

#ifndef ARROW_TEST
		m_fArrowSpeed = 200.f;
		m_fArrowCharge = 0.f;
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->Init();
		m_pArrow[m_iCurArrow]->SetActive(true);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK_READY);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetRestorePos(m_pArrow[m_iCurArrow]->Transform()->GetLocalPos());
#endif
		m_eState = PLAYER_STATE::ATTACK_READY;
	}
#ifndef ARROW_TEST
	if (KEY_HOLD(KEY_TYPE::KEY_LBTN)) {
		m_fArrowSpeed += 3000.f * DT;
		if (m_fArrowSpeed > 2000.f) {
			m_fArrowSpeed = 2000.f;
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(true);
		}
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetBowWorldMat(pBow->Transform()->GetWorldMat());
	}
#endif

	if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
		m_eState = PLAYER_STATE::ATTACK;

#ifndef ARROW_TEST
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(false);
		//Vec3 vPos4 = Transform()->GetLocalPos();
		//Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		//Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);

		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);

		Vec3 vDestPos = vPos + Transform()->GetWorldDir(DIR_TYPE::FRONT) * 300.f;
		//vDestPos.y = 0.f;
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDestPos(vDestPos);

		//Vec2 xzValue = GetDiagnal(m_fArcherLocation, vRight.x, vRight.z);

		//float fDegree = pCameraScript->GetDegree();
		//float fDegree2 = fDegree;

		//fDegree *= -1.f;


		//float yValue = sin(XMConvertToRadians(fDegree)) * m_fArcherLocation;

		//Vec3 vArrowPos = Vec3(GetObj()->Transform()->GetLocalPos().x + xzValue.x, GetObj()->Transform()->GetLocalPos().y + 70 + yValue, GetObj()->Transform()->GetLocalPos().z + xzValue.y);


		//m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vArrowPos);
		//fDegree *= -1.f;

		//float fArrowRotYDegree = XMConvertToDegrees(vRot.y) + 90.f;

		//m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(Vec3(vRot.x, XMConvertToRadians(fArrowRotYDegree), vRot.z));
		//if (m_iCurArrow == 0) {
		//	int a = 0;
		//}

		//Vec3 vFront2 = vArrowPos;
		//// Vec3 vRight2 = Vec3(1, 0, 0);
		//Vec3 vRight2 = vArrowDir;
		//auto k = XMLoadFloat3(&vRight2);
		//auto m = XMLoadFloat4x4(&m_pArrow[m_iCurArrow]->Transform()->GetWorldMat());
		//auto r = XMVector3TransformNormal(k, m);
		//XMFLOAT3 result;
		//XMStoreFloat3(&result, XMVector3Normalize(r));

		//float flength = sqrt(pow(result.x, 2) + pow(result.z, 2));


		//vArrowPos.x += result.x;
		//vArrowPos.z += result.z;
		//float xValue = sqrt(pow(m_fArcherLocation, 2) - pow(yValue, 2));
		//float xValue2 = xValue + flength;
		//float fyValue2 = yValue * xValue2 / xValue;

		//float SubeyValue2Value = fyValue2 - yValue;
		//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetFallSpeed(SubeyValue2Value);

		//vArrowPos.y += SubeyValue2Value;
		//Vec3 vTarget = vArrowPos - vFront2;

		//vTarget.Normalize();
		//float vDotValue = Dot(vTarget, result);
		//Vec3 vCrossValue;
		//if (vTarget.y > 0.f) {
		//	vCrossValue = Cross(vTarget, result);
		//}
		//else {
		//	vCrossValue = Cross(result, vTarget);
		//}

		//if (vCrossValue != Vec3(0.f, 0.f, 0.f)) {
		//	float value = XMConvertToRadians(fDegree2);

		//	if (!m_bCheckDegree) {
		//		value *= -1.f;
		//	}
		//	XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), value);
		//	m_pArrow[m_iCurArrow]->Transform()->SetQuaternion(XMQuaternionMultiply(m_pArrow[m_iCurArrow]->Transform()->GetQuaternion(), xmmatrix));

		//}

		m_iCurArrow++;
		m_iPower = 1;
		if (m_iCurArrow > 19) {
			m_iCurArrow = 0;

		}
#else
		//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);

		//Vec3 vPos4 = Transform()->GetLocalPos();
		//Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		//Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
		//m_vRestorePos = vPos4;
		////m_pArrow[m_iCurArrow]->SetActive(true);
		////vRight = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		//Vec3 vArrowDir = pEmptyObject->Transform()->GetLocalDir(DIR_TYPE::FRONT);
		//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vArrowDir);
		//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);
		//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetVelocityX();
		//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetVelocityZ();


		//Vec2 xzValue = GetDiagnal(m_fArcherLocation, vRight.x, vRight.z);

		//float fDegree = pCameraScript->GetDegree();
		//float fDegree2 = fDegree;

		//fDegree *= -1.f;


		//float yValue = sin(XMConvertToRadians(fDegree)) * m_fArcherLocation;

		//Vec3 vArrowPos = Vec3(GetObj()->Transform()->GetLocalPos().x + xzValue.x, GetObj()->Transform()->GetLocalPos().y + 70 + yValue, GetObj()->Transform()->GetLocalPos().z + xzValue.y);


		//m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vArrowPos);
		//fDegree *= -1.f;

		//float fArrowRotYDegree = XMConvertToDegrees(vRot.y) + 90.f;

		//m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(Vec3(vRot.x, XMConvertToRadians(fArrowRotYDegree), vRot.z));
		//if (m_iCurArrow == 0) {
		//	int a = 0;
		//}

		//Vec3 vFront2 = vArrowPos;
		//// Vec3 vRight2 = Vec3(1, 0, 0);
		//Vec3 vRight2 = vArrowDir;
		//auto k = XMLoadFloat3(&vRight2);
		//auto m = XMLoadFloat4x4(&m_pArrow[m_iCurArrow]->Transform()->GetWorldMat());
		//auto r = XMVector3TransformNormal(k, m);
		//XMFLOAT3 result;
		//XMStoreFloat3(&result, XMVector3Normalize(r));

		//float flength = sqrt(pow(result.x, 2) + pow(result.z, 2));


		//vArrowPos.x += result.x;
		//vArrowPos.z += result.z;
		//float xValue = sqrt(pow(m_fArcherLocation, 2) - pow(yValue, 2));
		//float xValue2 = xValue + flength;
		//float fyValue2 = yValue * xValue2 / xValue;

		//float SubeyValue2Value = fyValue2 - yValue;
		//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetFallSpeed(SubeyValue2Value);

		//vArrowPos.y += SubeyValue2Value;
		//Vec3 vTarget = vArrowPos - vFront2;

		//vTarget.Normalize();
		//float vDotValue = Dot(vTarget, result);
		//Vec3 vCrossValue;
		//if (vTarget.y > 0.f) {
		//	vCrossValue = Cross(vTarget, result);
		//}
		//else {
		//	vCrossValue = Cross(result, vTarget);
		//}

		//if (vCrossValue != Vec3(0.f, 0.f, 0.f)) {
		//	float value = XMConvertToRadians(fDegree2);

		//	if (!m_bCheckDegree) {
		//		value *= -1.f;
		//	}
		//	XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), value);
		//	m_pArrow[m_iCurArrow]->Transform()->SetQuaternion(XMQuaternionMultiply(m_pArrow[m_iCurArrow]->Transform()->GetQuaternion(), xmmatrix));

		//}

		//m_iCurArrow++;
		//m_iPower = 1;
		//if (m_iCurArrow > 19) {
		//	m_iCurArrow = 0;

		//}

//test
		//vPos = pCamera->GetScript<CCameraScript>()->GetShootPos();
#endif
	}

	if (m_bMoveCheck) {
		vPos = vPos2;
		m_bMoveCheck = false;
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

	m_FAnimation();

}
#include "Collider3D.h"
void CPlayerScript::m_FColCheck(Vec3 _vBeforePos, Vec3 _vAfterPos)
{
	if (m_bColCheck) {
		for (int i = 0; i < m_arrColObject.size(); ++i) {
			Vec3 vTargetPos = m_arrColObject[i]->Transform()->GetWorldPos();
			m_bMoveCheck=lengthCompare(_vBeforePos, _vAfterPos,GetObj(), vTargetPos);
			if (m_bMoveCheck) {
				return;
			}
		}
	}
}
void CPlayerScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	int a = 0;
	if (_pOther->GetObj()->GetScript<CArrowScript>() != nullptr) {

	}
	else {
		if (_pOther->GetObj()->GetScript<CProjectileScript>() != nullptr) {

		}
		else {
			m_arrColObject.push_back(_pOther->GetObj());
			m_bColCheck = true;
		}
	}
}

void CPlayerScript::OnCollision3D(CCollider3D* _pOther)
{
}

void CPlayerScript::OnCollision3DExit(CCollider3D* _pOther)
{

	if (_pOther->GetObj()->GetScript<CArrowScript>() != nullptr) {

	}
	else {
		vector<CGameObject*>::iterator iter = m_arrColObject.begin();
		for (int i = 0; iter != m_arrColObject.end(); ++iter, ++i) {
			if (m_arrColObject[i] == _pOther->GetObj()) {
				m_arrColObject.erase(iter);
				return;
			}

		}
	}
	

}

CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT), m_bCheckStartMousePoint(false)
, m_bColCheck(false), m_bMoveCheck(false), m_bCheckDegree(false), m_fLerpTime(0.f), m_fMaxLerpTime(10.f)
{
}

CPlayerScript::~CPlayerScript()
{
}
