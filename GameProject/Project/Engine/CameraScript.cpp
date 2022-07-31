#include "pch.h"
#include "CameraScript.h"

#include "Camera.h"
#include "Transform.h"


void CCameraScript::Update()
{
    CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

    if (GetObj()->Camera()->GetProjType() == PROJ_TYPE::PERSPECTIVE && pCurScene->GetCurScene() == SCENE_TYPE::INGAME)
    {
        Vec3 vPos = Transform()->GetLocalPos();
        Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Vec3 vRot = Transform()->GetLocalRot();

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
            if (KEY_NONE(KEY_TYPE::KEY_LBTN) && !m_bWind0Camera) {
                if (!m_bCheckStartMousePoint) {
                    m_bCheckStartMousePoint = true;
                }
                else {
                    Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
                    vRot.x -= vDrag.y * DT;
                    m_fDegree = XMConvertToDegrees(vRot.x);
                    if (m_fDegree < -5) {
                        m_fDegree = -5.f;
                        vRot.x = XMConvertToRadians(m_fDegree);
                    }
                    else if (m_fDegree > 10) {
                        m_fDegree = 10.f;
                        vRot.x = XMConvertToRadians(m_fDegree);
                    }
                }
            }
            else if (m_bWind0Camera) {
                vRot.x = 0.f;
            }
            break;
        case CAMERA_EFFECT_TYPE::ZOOMIN:
            //vRot.x = 0.f;
            vPos = CameraZoom(vPos);
            break;
        case CAMERA_EFFECT_TYPE::SHAKING:
            vPos = CameraShake(vPos, 6.0f, 3.f);
            break;
        case CAMERA_EFFECT_TYPE::DAMAGED:
            break;
        case CAMERA_EFFECT_TYPE::LIGHTNING:
            break;
        }

        GetObj()->Transform()->SetLocalPos(vPos);
        GetObj()->Transform()->SetLocalRot(vRot);
    }

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
	if (m_fZoomElapsedTime < 20.0f * DT)
	{
		Vec3 vDir = Transform()->GetLocalDir(DIR_TYPE::FRONT);
		//Vec3 vDir = Vec3::Right;

		_vPos += vDir * m_fZoomSpeed * DT;
        if (m_bWind0Camera) {
            _vPos.y = m_vRestorePos.y;
        }
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
	m_vZoomRestoreFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	m_fShakeNum = 1.0f;
	m_fZoomSpeed = 200.0f;
	m_fZoomElapsedTime = 0.0f;
}

void CCameraScript::SetDistanceOffset(CGameObject* _pObject)
{
	m_pPlayer = _pObject;

}



CCameraScript::CCameraScript() :CScript(0), m_fSpeed(200.f), m_fScaleSpeed(1.f), m_fZoomElapsedTime(0.0f), m_bCheckStartMousePoint(false), m_tEffectType(CAMERA_EFFECT_TYPE::NONE), m_bWind0Camera(false)
{
}

CCameraScript::~CCameraScript()
{
}
