#include "pch.h"
#include "EmptyPlayerScript.h"

void CEmptyPlayerScript::Update()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject * pPlayer = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Blue")->GetParentObj()[0]);

	Vec3 vPos = pPlayer->Transform()->GetLocalPos();
	//Vec3 vRot = pPlayer->Transform()->GetLocalRot();
	//Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();

	//vRot *= Vec3(0.f, 1.f, XMConvertToRadians(90.f));
	//vRot.y += XMConvertToRadians(90.f);  -> Player에서 하려면 -= 해줘야겟지 머... 위에도 -90.f

	//vPos = pPlayer->Transform()->GetWorldPos() - vRot * Vec3(-140, 80, -20);

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	if (!m_bCheckStartMousePoint) {
		m_bCheckStartMousePoint = true;
	}
	else {
		if (KEY_NONE(KEY_TYPE::KEY_LBTN)) {
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
		}
	}


	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
}


CEmptyPlayerScript::CEmptyPlayerScript() : CScript(0)
{

}

CEmptyPlayerScript::~CEmptyPlayerScript()
{

}