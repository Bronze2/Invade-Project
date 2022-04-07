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

	if (!KEY_HOLD(KEY_TYPE::KEY_LBTN)) {
		if (!m_bCheckStartMousePoint) {
			m_bCheckStartMousePoint = true;
		}
		else {
			Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
			vRot.x -= vDrag.y * DT;
			m_fDegree = XMConvertToDegrees(vRot.x);
			if (m_fDegree < -15) {
				m_fDegree = -15.f;
				vRot.x = XMConvertToRadians(m_fDegree);
			}
			else if (m_fDegree > 15) {
				m_fDegree = 15.f;
				vRot.x = XMConvertToRadians(m_fDegree);
			}
		}
	}
	/*	if (KEY_HOLD(KEY_TYPE::KEY_UP))
	//	{
	//		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	//		vPos += vFront * fSpeed * DT;
	//	}
	//
	//	if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
	//	{
	//		Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
	//		vPos += vBack * fSpeed * DT;
	//	}
	//
	//	if (KEY_HOLD(KEY_TYPE::KEY_LEFT))
	//	{
	//		Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	//		vPos += vLeft * fSpeed * DT;
	//	}
	//
	//	if (KEY_HOLD(KEY_TYPE::KEY_RIGHT))
	//	{
	//		Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	//		vPos += vRight * fSpeed * DT;
	//	}
	//	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	//	vRot.x -= vDrag.y * DT * 3.f;
	//	vRot.y += vDrag.x * DT * 1.5f;
	//
	*/

	if (m_bCheckCameraShake)
	{
		if (m_fShakeNum < 2.0f * PI) 
		{
			//vPos.x *= 3.05f * sin(m_fShakeNum * 1.f);// *powf(0.5f, m_fShakeNum);
			 //vPos.x = m_vRestorePos.x + 6.5f * sin(m_fShakeNum * 5.0f) * powf(0.5f, m_fShakeNum);
			 vPos.y = m_vRestorePos.y + 6.5f * sin(m_fShakeNum * 5.0f) * powf(0.5f, m_fShakeNum);
			 vPos.z = m_vRestorePos.z + 6.5f * sin(m_fShakeNum * 5.0f) * powf(0.5f, m_fShakeNum);

			m_fShakeTime -= DT;
			m_fShakeNum += 1.0f;
		}
		else 
		{
			vPos = m_vRestorePos;
			m_fShakeTime = 0.0f;
			m_fShakeNum = 0.0f;
			m_bCheckCameraShake = false;
		}
	}

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
}


void CCameraScript::CameraShake(float _fTime)
{
	m_vRestorePos = Transform()->GetLocalPos();
	m_fShakeTime = _fTime;
	m_fShakeNum = 1.0f;
	
	m_bCheckCameraShake = true;
}

CCameraScript::CCameraScript() :CScript(0), m_fSpeed(200.f), m_fScaleSpeed(1.f), m_bCheckStartMousePoint(false), m_bCheckCameraShake(false), m_fShakeNum(0.0f)
{
}

CCameraScript::~CCameraScript()
{
}