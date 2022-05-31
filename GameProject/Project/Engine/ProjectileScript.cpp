#include "pch.h"
#include "ProjectileScript.h"
#include "GameObject.h"
#include "Animator3D.h"
#include "MeshRender.h"
#include "Mesh.h"
#include "Collider3D.h"
#include "MinionScript.h"

bool CProjectileScript::M_FLengthCheck(const Vec3& _Pos)
{
	float value = sqrt(pow(m_vStartPos.x - _Pos.x, 2)+ pow(m_vStartPos.y - _Pos.y, 2) + pow(m_vStartPos.z - _Pos.z, 2));
	if (value > m_fLength) {
		return true;
	}

	return false;
}

void CProjectileScript::Update()
{

	if (nullptr == m_pTarget)
	{
		DeleteObject(GetObj());
		return;
	}
	
	if (m_pTarget->IsDead()) {
		DeleteObject(GetObj());
		return;
	}
	if (m_pObject->IsDead()) {
		DeleteObject(GetObj());
		return;
	}

	Vec3 vLocalPos = Transform()->GetLocalPos();

	Vec3 vRot = Transform()->GetLocalRot();
	Vec3 vPos = Transform()->GetWorldPos();

	Vec3 vRestoreLocalPos = vLocalPos;

	if (m_pObject != nullptr) {
		if (m_pObject->GetScript<CMinionScript>() != nullptr) {
			if (m_pObject->GetScript<CMinionScript>()->GetState() == MINION_STATE::WALK) {
				DeleteObject(GetObj());
			}
		}
	}
	else {
		DeleteObject(GetObj());
	}

	if (vLocalPos.y < 0) {
		DeleteObject(GetObj());
	}
	switch (m_eProjectileType)
	{
	case PROJECTILE_TYPE::MINION:
	{

	}

	break;
	case PROJECTILE_TYPE::TOWER:
	{

	}
	break;
	default:
		break;
	}


	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vWorldDir = m_pObject->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vLocalDir = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);


	
	if (m_eProjectileType == PROJECTILE_TYPE::MINION) {
		if (!m_bRotate) {
			Vec3 vRot = Transform()->GetLocalRot();
			float angle = atan2(vPos.x - m_vTargetPos.x, vPos.z - m_vTargetPos.z) * (180 / PI);
			float rotate = angle * 0.0174532925f;
			vRot.y = rotate;

			Transform()->SetLocalRot(vRot);
			m_bRotate = true;
		}

		vLocalPos.x -= vWorldDir.x * m_fSpeed * DT;
		vLocalPos.y += m_vDir.y * m_fSpeed * DT;
		vLocalPos.z -= vWorldDir.z * m_fSpeed * DT;
	}
	else {
		if (!m_bRotate) {
			Vec3 vRot = Transform()->GetLocalRot();
			float angle = atan2(vPos.x - m_vTargetPos.x, vPos.z - m_vTargetPos.z) * (180 / PI);
			float rotate = angle * 0.0174532925f;
			vRot.y = rotate;

			Transform()->SetLocalRot(vRot);
			m_bRotate = true;
		}
		float beforey=m_pObject->Transform()->GetLocalRot().y;
		float aftery=XMConvertToDegrees(m_pObject->Transform()->GetLocalRot().y);
	//	vLocalPos.x += vWorldDir.x * m_fSpeed * DT;
		vLocalPos.y +=  m_vDir.y*m_fSpeed * DT;
		
		if (m_vDir.z > 0) {
			m_vDir.z *= -1.f;
		}
		if (m_vDir.x > 0) {
			m_vDir.x *= -1.f;
		}
		float value = 0.f;
		if (m_vDir.x < m_vDir.z) {
			value = m_vDir.x;
		}
		else {
			value = m_vDir.z;
		}
		vLocalPos.z += value * m_fSpeed * DT;
	}

	int a = 0;
	float xvalue = m_vDir.x * 100.f * DT;

	vLocalPos -= m_vDir * 500.f * DT;
	vLocalPos.y = vRestoreLocalPos.y;


	Transform()->SetLocalPos(vLocalPos);
	if(!m_bUpdate)
	{
		m_bUpdate = true;
	}

}


#include "TowerScript.h"
void CProjectileScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	if (m_pObject == _pOther->GetObj())
		return;
	if (_pOther->GetObj()->GetScript<CMinionScript>() != nullptr) {
		if (nullptr != _pOther->GetObj()) {
			if (m_pObject->GetScript<CMinionScript>() != nullptr) {
				if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() != m_pObject->GetScript<CMinionScript>()->GetCamp())
				{
					_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
					DeleteObject(GetObj());
				}
			}
			else {
				if ((UINT)(_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp()) != (UINT)(m_pObject->GetScript<CTowerScript>()->GetCamp()))
				{
					_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
					DeleteObject(GetObj());
				}
			}
		}
		
	}
	if (_pOther->GetObj()->GetScript<CTowerScript>() != nullptr) {
		_pOther->GetObj()->GetScript<CTowerScript>()->GetDamage(m_uiDamage);
		DeleteObject(GetObj());
	}
}

void CProjectileScript::Init()
{

}

CProjectileScript::CProjectileScript():CScript((UINT)SCRIPT_TYPE::PROJECTILESCRIPT),m_pObject(nullptr),m_fSpeed(),m_uiDamage(),m_fAlpha(0.f)
,m_bRotate(false), m_eProjectileType(PROJECTILE_TYPE::MINION),m_fLength(350.f),m_bUpdate(false),m_bLaunch(0)
{
}

CProjectileScript::~CProjectileScript()
{
}
