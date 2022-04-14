#include "pch.h"
#include "PlayerScript.h"
#include "ArrowScript.h"
#include "MeshRender.h"
#include "Camera.h"
#include"CameraScript.h"
#include "Animator3D.h"
#include "BowScript.h"

void CPlayerScript::m_FAnimation()
{
	if (m_ePrevState != m_eState) {
		switch (m_eState)
		{
		case PLAYER_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE")) {
				m_pCurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE");
				GetObj()->Animator3D()->SetFrmaeIdx(m_pCurAnimation->StartFrame);
				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
				GetObj()->Animator3D()->SetCurTime(0.f);
				GetObj()->Animator3D()->SetStartFrameTime(time);
				m_ePrevState = PLAYER_STATE::IDLE;
			}
		}
			break;
		case PLAYER_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK")) {
				m_pCurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK");
				GetObj()->Animator3D()->SetFrmaeIdx(m_pCurAnimation->StartFrame);
				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
				GetObj()->Animator3D()->SetCurTime(0.f);
				GetObj()->Animator3D()->SetStartFrameTime(time);
				m_ePrevState = PLAYER_STATE::WALK;
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
		case PLAYER_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK")) {
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

void CPlayerScript::Init()
{
	m_eState = PLAYER_STATE::IDLE;
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE")) {
		m_pCurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"IDLE");
		GetObj()->Animator3D()->SetFrmaeIdx(m_pCurAnimation->StartFrame);
		double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
		GetObj()->Animator3D()->SetCurTime(0.f);
		GetObj()->Animator3D()->SetStartFrameTime(time);
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
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vPos2 = Transform()->GetLocalPos();
	Vec3 vPos3 = Transform()->GetWorldPos();
	Vec3 vRot = Transform()->GetLocalRot();
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
		if (KEY_HOLD(KEY_TYPE::KEY_W)) {
			Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
			vPos += vFront * 200.f * DT;
			m_FColCheck(vPos3, vPos);

			m_eState = PLAYER_STATE::WALK;
		}
		if (KEY_HOLD(KEY_TYPE::KEY_S)) {
			Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
			vPos += vBack * 200.f * DT;
			m_FColCheck(vPos3, vPos);
			m_eState = PLAYER_STATE::WALK;
		}
		if (KEY_HOLD(KEY_TYPE::KEY_A)) {
			Vec3 vLeft = Transform()->GetWorldDir(DIR_TYPE::FRONT);
			vPos += vLeft * 200.f * DT;
			m_FColCheck(vPos3, vPos);
			m_eState = PLAYER_STATE::WALK;
		}
		if (KEY_HOLD(KEY_TYPE::KEY_D)) {
			Vec3 vRight = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
			vPos += vRight * 200.f * DT;
			m_FColCheck(vPos3, vPos);
			m_eState = PLAYER_STATE::WALK;
		}
	}

	if (KEY_AWAY(KEY_TYPE::KEY_W)) {
		m_eState = PLAYER_STATE::IDLE;
	}
	if (KEY_AWAY(KEY_TYPE::KEY_S)) {

		m_eState = PLAYER_STATE::IDLE;
	}
	if (KEY_AWAY(KEY_TYPE::KEY_A)) {

		m_eState = PLAYER_STATE::IDLE;
	}
	if (KEY_AWAY(KEY_TYPE::KEY_D)) {

		m_eState = PLAYER_STATE::IDLE;
	}

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
		//CGameObject* pObj=GetObj()->GetChild()[0];
		//Vec3 vPos4 = pObj->Transform()->GetLocalPos();
		//Vec3 vRight = pObj->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		//Vec3 vFront = pObj->Transform()->GetLocalDir(DIR_TYPE::FRONT);
		//m_vRestorePos = vPos4;
		//vPos4 += vRight * 10.f;
		//vPos4 += vFront * 10.f;
		//pObj->Transform()->SetLocalPos(vPos4);
		m_fArrowSpeed = 200.f;
		
	}
	if (KEY_HOLD(KEY_TYPE::KEY_LBTN)) {
		m_fArrowSpeed += 1000.f*DT;
		if (m_fArrowSpeed > 2000.f) {
			m_fArrowSpeed = 2000.f;
		}
	}

	if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Camera")->GetParentObj()[0]);

		//CGameObject* pObj = GetObj()->GetChild()[0];
		Vec3 vPos4 = Transform()->GetLocalPos();
		Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
		m_vRestorePos = vPos4;
		//vPos4 += vRight * 10.f;
		//vPos4 += vFront * 10.f;
		//pObj->Transform()->SetLocalPos(vPos4);
		m_pArrow[m_iCurArrow]->SetActive(true);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->Init();
		vRight = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vRight);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetVelocityX();
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetVelocityZ();


		Vec2 xzValue = GetDiagnal(m_fArcherLocation, vRight.x, vRight.z);

		CCameraScript* p=dynamic_cast<CCameraScript*>(pCamera->GetScripts()[0]);
		float fDegree= p->GetDegree();
		float fDegree2 = fDegree;

		fDegree *= -1.f;
	
	
		float yValue = sin(XMConvertToRadians(fDegree)) * m_fArcherLocation;

		Vec3 vArrowPos = Vec3(GetObj()->Transform()->GetLocalPos().x+xzValue.x, GetObj()->Transform()->GetLocalPos().y + 70+yValue, GetObj()->Transform()->GetLocalPos().z+xzValue.y);

		
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
			float value= XMConvertToRadians(fDegree2);

			if (!m_bCheckDegree) {
				value *= -1.f;
			}
			XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), value);
			m_pArrow[m_iCurArrow]->Transform()->SetQuaternion(XMQuaternionMultiply(m_pArrow[m_iCurArrow]->Transform()->GetQuaternion(), xmmatrix));
	
		}
		
		m_iCurArrow++;
		m_iPower = 1;
		if (m_iCurArrow > 19) { 
			m_iCurArrow = 0;
		
		}

	}

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	if (!m_bCheckStartMousePoint) {
		m_bCheckStartMousePoint = true;
	}
	else {
		vRot.y += vDrag.x * DT * 1.f;
		float fDegree = XMConvertToDegrees(vRot.y);
		if (fDegree < -360) {
			 fDegree+= 360.f;
			 vRot.y = XMConvertToRadians(fDegree);
		}
		else if (fDegree > 360) {
			fDegree -= 360.f;
			vRot.y = XMConvertToRadians(fDegree);
		}
	}
	if (m_bMoveCheck) {
		vPos = vPos2;
		m_bMoveCheck = false;
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
		m_arrColObject.push_back(_pOther->GetObj());
		m_bColCheck = true;
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

CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT), m_bCheckStartMousePoint(false), m_fArcherLocation(20.f)
,m_bColCheck(false),m_bMoveCheck(false), m_bCheckDegree(false)
{
}

CPlayerScript::~CPlayerScript()
{
}
