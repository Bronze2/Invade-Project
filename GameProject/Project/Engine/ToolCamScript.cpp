#include "pch.h"
#include "ToolCamScript.h"

#include "Camera.h"
#include "Transform.h"
void CToolCamScript::Update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;
	Vec3 vRot = Transform()->GetLocalRot();
	if (!m_bCheckStartMousePoint) {
		m_bCheckStartMousePoint = true;
	}
	else {
		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		vRot.x -= vDrag.y * DT;
		m_fDegree=XMConvertToDegrees(vRot.x);
		if (m_fDegree < -15 ) {
			m_fDegree = -15.f;
			vRot.x=XMConvertToRadians(m_fDegree);
		}
		else if (m_fDegree > 15) {
			m_fDegree = 15.f;
			vRot.x = XMConvertToRadians(m_fDegree);
		}

		
	}




	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
}

CToolCamScript::CToolCamScript():CScript(0),m_fSpeed(200.f),m_fScaleSpeed(1.f),m_bCheckStartMousePoint(false)
{
}

CToolCamScript::~CToolCamScript()
{
}
