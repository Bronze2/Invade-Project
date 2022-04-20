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

	Vec3 vPlayerWorldPos = pPlayer->Transform()->GetWorldPos();
	Vec3 vPlayerWorldFront = pPlayer->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vPlayerWorldUp = pPlayer->Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vPlayerWorldRight = pPlayer->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vPlayerPos = pPlayer->Transform()->GetLocalPos();
	Vec3 vPlayerRot = pPlayer->Transform()->GetLocalRot();

	// Monster.fbx기준  dist
	//float fHdist = 40.0f;
	//float fVdist = 40.0f;
	//float fRdist = 10.0f;

	// Pivot 문제 테스트용 dist
	float fHdist = 400.0f;
	float fVdist = 150.0f;
	float fRdist = 10.0f;
	float fTargetDist = 200.0f;
	Vec3 vTargetPos = vPlayerWorldPos + vPlayerWorldFront * fTargetDist;
	Vec3 vOffset = Vec3(-400.f, 150.f, 10.f);

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
		if (KEY_NONE(KEY_TYPE::KEY_LBTN)) {
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

				//if (!m_bBackMode) // W
				//{
				//	vPos = vPlayerWorldPos - vPlayerWorldFront * fHdist + vPlayerWorldUp * fVdist + vPlayerWorldRight * fRdist;

				//	float fRotDegree = atan2(vPlayerWorldPos.x - vPos.x, vPlayerWorldPos.z - vPos.z) * (180 / PI);
				//	vRot.y = XMConvertToRadians(fRotDegree);
				//}
				//else // A S D
				//{
				//	vPos = vPlayerWorldPos + vPlayerWorldFront * 100.f * DT;
				//	float fRotDegree = atan2(vPlayerWorldPos.x - vPos.x, vPlayerWorldPos.z - vPos.z) * (180 / PI);
				//	vRot.y = XMConvertToRadians(fRotDegree);
				//}
				vPos = vPlayerWorldPos - vPlayerWorldFront * fHdist + vPlayerWorldUp * fVdist + vPlayerWorldRight * fRdist;

				if ((KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_D))) {
					vPos += vPlayerWorldFront * 100.f * DT;
				}
				else
				{

				}
				float fRotDegree = atan2(vPlayerWorldPos.x - vPos.x, vPlayerWorldPos.z - vPos.z) * (180 / PI);
				vRot.y = XMConvertToRadians(fRotDegree);
			}
		}
		break;
	case CAMERA_EFFECT_TYPE::ZOOMIN:
		vPos = CameraZoom(vPos);	
		break;
	case CAMERA_EFFECT_TYPE::SHAKING:
		vPos = CameraShake(vPos, 6.0f, 20.f);
		break;
	case CAMERA_EFFECT_TYPE::DAMAGED:
		break;
	case CAMERA_EFFECT_TYPE::LIGHTNING:
		break;
	}

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
}

Vec3 CCameraScript::CameraShake(Vec3 _vPos, float _DamageTime, float _fDamageSize)
{
	float fShakeFactor = 5.f;
	if (m_fShakeNum < _DamageTime * PI * fShakeFactor)
	{
		_vPos.y = m_vRestorePos.y + _fDamageSize * sin(m_fShakeNum);
		_vPos.z = m_vRestorePos.z + _fDamageSize * -sin(m_fShakeNum);

		m_fShakeNum += fShakeFactor;
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
	if (m_fZoomElapsedTime < 18.0f * DT)
	{
		_vPos += Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_fZoomSpeed * DT;
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
	m_fZoomSpeed = 200.0f;
	m_fZoomElapsedTime = 0.0f;
}

CCameraScript::CCameraScript() :CScript(0), m_fSpeed(200.f), m_fScaleSpeed(1.f), m_fZoomElapsedTime(0.0f), m_bCheckStartMousePoint(false), m_tEffectType(CAMERA_EFFECT_TYPE::NONE)
{
}

CCameraScript::~CCameraScript()
{
}