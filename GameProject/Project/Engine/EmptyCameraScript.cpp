#include "pch.h"
#include "EmptyCameraScript.h"

void CEmptyCameraScript::Update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
		Init();
	}

	if (KEY_NONE(KEY_TYPE::KEY_LBTN)) {
		if (!m_bCheckStartMousePoint) {
			m_bCheckStartMousePoint = true;
		}
		else {
			Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
			vRot.x -= vDrag.y * DT;
			m_fDegree = XMConvertToDegrees(vRot.x);
			if (m_fDegree < -5.f) {
				m_fDegree = -5.f;
				vRot.x = XMConvertToRadians(m_fDegree);
			}
			else if (m_fDegree > 5) {
				m_fDegree = 5.f;
				vRot.x = XMConvertToRadians(m_fDegree);
			}
		}
	}
	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
}

void CEmptyCameraScript::Init()
{

}

CEmptyCameraScript::CEmptyCameraScript() :CScript(0)
{

}

CEmptyCameraScript::~CEmptyCameraScript()
{

}