#include "pch.h"
#include "BowScript.h"
#include "MeshRender.h"
#include "Animator3D.h"
#include "ArrowScript.h"
#include "CameraScript.h"

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

#ifdef ARROW_TEST
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject* pEmptyObject = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1]);
	CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];

	//Vec3 vBowFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vBowFront = GetObj()->GetParent()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vArrowDir;

	Vec3 vArrowPos = m_pArrow[m_iCurArrow]->Transform()->GetLocalPos();

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
		m_fArrowSpeed = 200.f;
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->Init();
		m_pArrow[m_iCurArrow]->SetActive(true);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK_READY);
		//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetRestorePos(m_pArrow[m_iCurArrow]->Transform()->GetLocalPos());
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN)) {
		m_fArrowSpeed += 2000.f * DT;
		if (m_fArrowSpeed > 2000.f) {
			m_fArrowSpeed = 2000.f;
			m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(true);
		}
	}

	if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
		//Vec3 vTargetPos = pCamera->GetScript<CCameraScript>()->GetShootPos();
		Vec3 vTargetDir = pCamera->GetScript<CCameraScript>()->GetShootDir();

		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetMaxCharged(false);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);
		//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(Transform()->GetWorldDir(DIR_TYPE::UP));
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vTargetDir);


		Vec3 vArrowPos = m_pArrow[m_iCurArrow]->Transform()->GetWorldPos() + Vec3(0.f, 0.f, 0.f);
		Vec3 vArrowRot = GetObj()->GetParent()->Transform()->GetLocalRot() + Vec3(0.f, XMConvertToRadians(90.f), 0.f);
		Vec3 vArrowRight = GetObj()->GetParent()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		Vec3 vArrowUp = GetObj()->GetParent()->Transform()->GetWorldDir(DIR_TYPE::UP);
		//Vec3 vArrowFront = m_pArrow[m_iCurArrow]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 vPlayerFront = GetObj()->GetParent()->Transform()->GetWorldDir(DIR_TYPE::FRONT);

		m_pArrow[m_iCurArrow]->ClearParent();

		m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vArrowPos);	// vArrowPos
		m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(vArrowRot);
		m_pArrow[m_iCurArrow]->Transform()->SetWorldDir(DIR_TYPE::RIGHT, vArrowRight);
		m_pArrow[m_iCurArrow]->Transform()->SetWorldDir(DIR_TYPE::UP, vArrowUp);
		//m_pArrow[m_iCurArrow]->Transform()->SetWorldDir(DIR_TYPE::FRONT, vArrowFront);
		m_pArrow[m_iCurArrow]->Transform()->SetWorldDir(DIR_TYPE::FRONT, vPlayerFront);

		//Vec3 vDestPos = vTrans + Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500.f;
		//vDestPos.y = 0.f;
		//m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDestPos(Vec3(-300.f, 0.f, 0.f));

		/*Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	
		//Vec3 vArrowDir = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetDir(vArrowDir);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetSpeed(m_fArrowSpeed);
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetVelocityX();
		m_pArrow[m_iCurArrow]->GetScript<CArrowScript>()->SetVelocityZ();


		Vec2 xzValue = GetDiagnal(m_fArcherLocation, vRight.x, vRight.z);

		CCameraScript* p = dynamic_cast<CCameraScript*>(pCamera->GetScripts()[0]);
		float fDegree = p->GetDegree();
		float fDegree2 = fDegree;

		fDegree *= -1.f;


		float yValue = sin(XMConvertToRadians(fDegree)) * m_fArcherLocation;

		Vec3 vArrowPos = Vec3(GetObj()->Transform()->GetLocalPos().x + xzValue.x, GetObj()->Transform()->GetLocalPos().y + 70 + yValue, GetObj()->Transform()->GetLocalPos().z + xzValue.y);


		m_pArrow[m_iCurArrow]->Transform()->SetLocalPos(vArrowPos);
		fDegree *= -1.f;

		float fArrowRotYDegree = XMConvertToDegrees(vRot.y) + 90.f;

		m_pArrow[m_iCurArrow]->Transform()->SetLocalRot(Vec3(vRot.x, XMConvertToRadians(fArrowRotYDegree), vRot.z));
		if (m_iCurArrow == 0) {
			int a = 0;
		}

		Vec3 vFront2 = vArrowPos;
		// Vec3 vRight2 = Vec3(1, 0, 0);
		Vec3 vRight2 = vArrowDir;
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
			float value = XMConvertToRadians(fDegree2);

			if (!m_bCheckDegree) {
				value *= -1.f;
			}
			XMVECTOR xmmatrix = XMQuaternionRotationAxis(XMLoadFloat3(&vCrossValue), value);
			m_pArrow[m_iCurArrow]->Transform()->SetQuaternion(XMQuaternionMultiply(m_pArrow[m_iCurArrow]->Transform()->GetQuaternion(), xmmatrix));

		}*/

		m_iCurArrow++;
		m_iPower = 1;
		if (m_iCurArrow > 19) {
			m_iCurArrow = 0;

		}
		m_bMaxCharged = false;
	}

#endif
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
#ifdef ARROW_TEST
	pBlackTex = CResMgr::GetInst()->FindRes<CTexture>(L"Black");
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (int i = 0; i < 20; ++i) {
		m_pArrow[i] = new CGameObject;
		m_pArrow[i]->SetName(L"Arrow");


		m_pArrow[i]->AddComponent(new CTransform());
		m_pArrow[i]->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		m_pArrow[i]->Transform()->SetLocalScale(Vec3(80.f, 1.f, 1.f));

		m_pArrow[i]->AddComponent(new CMeshRender);
		m_pArrow[i]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
		m_pArrow[i]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
		//	m_pArrow[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlackTex.GetPointer());

		m_pArrow[i]->AddComponent(new CCollider2D);
		m_pArrow[i]->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);

		m_pArrow[i]->AddComponent(new CArrowScript(m_iType));
		pCurScene->FindLayer(L"Blue")->AddGameObject(m_pArrow[i]);
		//pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);
		m_pArrow[i]->SetActive(false);
		m_pArrow[i]->GetScript<CArrowScript>()->SetBow(GetObj());
		GetObj()->AddChild(m_pArrow[i]);
	}

	m_iCurArrow = 0;
	m_iPower = 1;
	m_bMaxCharged = false;
#endif
}


CBowScript::CBowScript() : CScript((UINT)SCRIPT_TYPE::BOWSCRIPT), m_fArcherLocation(20.f)
{
}

CBowScript::~CBowScript()
{
}
