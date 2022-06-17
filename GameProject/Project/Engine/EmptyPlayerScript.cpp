#include "pch.h"
#include "EmptyPlayerScript.h"

void CEmptyPlayerScript::Update()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject * pPlayer = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Red")->GetParentObj()[0]);

	Vec3 vPos = pPlayer->Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();

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