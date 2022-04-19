#include "pch.h"
#include "CameraScript.h"

#include "Camera.h"
#include "Transform.h"

void CCameraScript::Update()
{
	Vec3 vPos = Transform()->GetWorldPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;
	Vec3 vRot = Transform()->GetLocalRot();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject* pPlayer = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Blue")->GetParentObj()[0]);
	Matrix mPlayerWorldMat = pPlayer->Transform()->GetWorldMat();
	// mPlayerWorldMat *= XMMatrixRotationY(XMConvertToRadians(90.0f));
	Vec3 vPlayerWorldPos = pPlayer->Transform()->GetWorldPos();
	Vec3 vPlayerWorldFront = mPlayerWorldMat.Front();
	Vec3 vPlayerWorldUp = mPlayerWorldMat.Up();
	Vec3 vPlayerWorldRight = mPlayerWorldMat.Right();
	Vec3 vPlayerFront = pPlayer->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vPlayerPos = pPlayer->Transform()->GetLocalPos();

	// 정상적인 dist
	//float fHdist = 40.0f;
	//float fVdist = 40.0f;
	//float fRdist = 10.0f;

	// Pivot 문제 테스트용 dist
	float fHdist = 400.0f;
	float fVdist = 150.0f;
	float fRdist = 10.0f;
	float fTargetDist = 300.0f;
	Vec3 vOffset = Vec3(-300.f, 150.f, 10.f);

	Vec3 vTargetPos = vPlayerPos + vPlayerFront * fTargetDist;

	if (KEY_TAB(KEY_TYPE::KEY_NUM0)) {
		Init();
		m_tEffectType = CAMERA_EFFECT_TYPE::SHAKING;
	}

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
		Init();
		m_tEffectType = CAMERA_EFFECT_TYPE::ZOOMIN;
	}

	if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
		vPos = m_vRestorePos;
		m_tEffectType = CAMERA_EFFECT_TYPE::NONE;
	}

	switch (m_tEffectType)
	{
	case CAMERA_EFFECT_TYPE::NONE:
		if (!KEY_HOLD(KEY_TYPE::KEY_LBTN)) {
			if (!m_bCheckStartMousePoint) {
				m_bCheckStartMousePoint = true;
			}
			else {
				//vPos = vPlayerPos - vPlayerWorldFront * fHdist + vPlayerWorldUp * fVdist + vPlayerWorldRight * fRdist;
				vPos = vPlayerWorldPos + vOffset;

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
		break;
	case CAMERA_EFFECT_TYPE::ZOOMIN:
		vPos = CameraZoom(vPos);	
		break;
	case CAMERA_EFFECT_TYPE::SHAKING:
		vPos = CameraShake(vPos);
		break;
	case CAMERA_EFFECT_TYPE::DEMAGED:
		break;
	case CAMERA_EFFECT_TYPE::LIGHTNING:
		break;
	}

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);

	//Vec3 vLook = vTargetPos - vPos;
	Vec3 vLook = vPlayerWorldPos - vPos;

	Transform()->LookAt(vLook, vRot);
}

Vec3 CCameraScript::CameraShake(Vec3 _vPos)
{
	if (m_fShakeNum < 2.0f * PI)
	{
		_vPos.y = m_vRestorePos.y + 5.5f * sin(m_fShakeNum * 10.0f);
		_vPos.z = m_vRestorePos.z + 5.5f * -sin(m_fShakeNum * 10.0f);

		m_fShakeNum += 1.0f;
	}
	else
	{
		_vPos = m_vRestorePos;
		m_fShakeNum = 0.0f;
		m_tEffectType = CAMERA_EFFECT_TYPE::NONE;
	}

	return _vPos;
}

Vec3 CCameraScript::CameraZoom(Vec3 _vPos)
{
	if (m_fZoomElapsedTime < 20.0f * DT)
	{
		_vPos += m_vZoomRestoreFront * m_fZoomSpeed * DT;
		m_fZoomElapsedTime += DT;
	}
	else
	{
		m_fZoomSpeed = 0.0f;
		m_fZoomElapsedTime = 0.0f;
	}
	return _vPos;
}

void CCameraScript::Init()
{
	m_vRestorePos = Transform()->GetLocalPos();
	m_vZoomRestoreFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	m_fShakeNum = 1.0f;
	m_fZoomSpeed = 100.0f;
	m_fZoomElapsedTime = 0.0f;
}

CCameraScript::CCameraScript() :CScript(0), m_fSpeed(200.f), m_fScaleSpeed(1.f), m_fZoomElapsedTime(0.0f), m_bCheckStartMousePoint(false), m_tEffectType(CAMERA_EFFECT_TYPE::NONE)
{
}

CCameraScript::~CCameraScript()
{
}