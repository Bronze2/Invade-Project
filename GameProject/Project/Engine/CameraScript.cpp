#include "pch.h"
#include "CameraScript.h"

#include "Camera.h"
#include "Transform.h"
void CCameraScript::Update()
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
//	if (KEY_HOLD(KEY_TYPE::KEY_W))
//	{
//		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
//		vPos += vFront * fSpeed * DT*200;
//	}
//
//	if (KEY_HOLD(KEY_TYPE::KEY_S))
//	{
//		Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
//		vPos += vBack * fSpeed * DT*200;
//	}
//
//	if (KEY_HOLD(KEY_TYPE::KEY_A))
//	{
//		Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
//		vPos += vLeft * fSpeed * DT*200;
//	}
//
//	if (KEY_HOLD(KEY_TYPE::KEY_D))
//	{
//		Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
//		vPos += vRight * fSpeed * DT*200;
//	}
//	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
//	vRot.x -= vDrag.y * DT * 3.f;
//	vRot.y += vDrag.x * DT * 1.5f;
//
//
	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
}

CCameraScript::CCameraScript():CScript(0),m_fSpeed(200.f),m_fScaleSpeed(1.f),m_bCheckStartMousePoint(false)
{
}

CCameraScript::~CCameraScript()
{
}
