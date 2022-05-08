#include "pch.h"
#include "ProjectileScript.h"
#include "GameObject.h"
#include "Collider3D.h"
#include "MinionScript.h"
#include "TimeMgr.h"
#include "Server.h"

void CProjectileScript::Update()
{
	if (m_pObject->IsDead()) {
		DeleteObject(GetObj());
		return;
	}
	Vec3 vLocalPos = Transform()->GetLocalPos();
	Vec3 vRestoreLocalPos = vLocalPos;
	
	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vWorldDir = m_pObject->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vLocalDir = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

	if (m_pObject != nullptr) {
		if (m_pObject->GetScript<CMinionScript>() != nullptr) {
			if (m_pObject->GetScript<CMinionScript>()->GetState() == MINION_STATE::WALK) {
				CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
				//DeleteObject(GetObj());
			}
		}
	}
	else {
		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
		DeleteObject(GetObj());
	}

	if (vLocalPos.y < 0) {
		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
		DeleteObject(GetObj());
	}

	switch (m_eProjectileType)
	{
	case PROJECTILE_TYPE::MINION:
	{
		//tMTBone* p = const_cast<tMTBone*>(m_pObject->MeshRender()->GetMesh()->GetBone(m_iBone));
		//int d = m_pObject->Animator3D()->GetFrameIdx();
		//if (p->vecKeyFrame.size() < d) {
		//	return;
		//}

		if (!m_bRotate) {
			Vec3 vRot = Transform()->GetLocalRot();
			float angle = atan2(vPos.x - m_vTargetPos.x, vPos.z - m_vTargetPos.z) * (180 / PI);
			float rotate = angle * 0.0174532925f;
			vRot.y = rotate;

			Transform()->SetLocalRot(vRot);
			m_bRotate = true;
		}

		if (vPos.z != 0.f && vPos.x != 0.f) {
			Vec3 vvalue = vPos - m_vTargetPos;
			vvalue.Normalize();
			int b = 0;
		}
		int a = 0;
		float xvalue = m_vDir.x * 100.f * DT;

		vLocalPos -= m_vDir * 500.f * DT;
		//vLocalPos.y = vRestoreLocalPos.y;
	}

	break;
	case PROJECTILE_TYPE::TOWER:
	{

		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vRot = Transform()->GetLocalRot();

		//cout <<"["<<m_GetId()<<"]"  <<m_pTarget->GetScript<CMinionScript>()->m_GetId() << endl;
		float angle = atan2(vPos.y - m_vTargetPos.y, vPos.z - m_vTargetPos.z) * (180 / PI);
		float rotate = angle * 0.0174532925f;
		vRot.x = rotate;
		Transform()->SetLocalRot(vRot);
		//SetDir(vRot);

		if (!m_bRotate) {
			Vec3 vRot = Transform()->GetLocalRot();
			float angle = atan2(vPos.x - m_vTargetPos.x, vPos.z - m_vTargetPos.z) * (180 / PI);
			float rotate = angle * 0.0174532925f;
			vRot.y = rotate;

			Transform()->SetLocalRot(vRot);
			m_bRotate = true;
		}

		if (vPos.z != 0.f && vPos.x != 0.f) {
			Vec3 vvalue = vPos - m_vTargetPos;
			vvalue.Normalize();
			int b = 0;
		}
		int a = 0;
		float xvalue = m_vDir.x * 100.f * DT;

		vLocalPos += m_vDir * 500.f * DT;
	}
	break;
	default:
		break;
	}

	Transform()->SetLocalPos(vLocalPos);
	SHARED_DATA::g_bullet[m_GetId()].x = vLocalPos.x;
	SHARED_DATA::g_bullet[m_GetId()].y = vLocalPos.y;
	SHARED_DATA::g_bullet[m_GetId()].z = vLocalPos.z;

	CServer::GetInst()->send_projectile_packet(m_GetId(), 1);

}


#include "TowerScript.h"
void CProjectileScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	if (_pOther->GetObj()->GetScript<CMinionScript>() != nullptr) {
		if (nullptr != _pOther->GetObj()) {
			if (m_pObject->GetScript<CMinionScript>() != nullptr) {
				if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() != m_pObject->GetScript<CMinionScript>()->GetCamp())
				{
					_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
					CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
					DeleteObject(GetObj());
				}
			}
			/*else {
				if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCampName() != m_pObject->GetScript<CTowerScript>()->GetCampName())
				{
					_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
					DeleteObject(GetObj());
				}
			}*/
		}

	}
	if (_pOther->GetObj()->GetScript<CTowerScript>() != nullptr&& m_eProjectileType == PROJECTILE_TYPE::MINION) {
		_pOther->GetObj()->GetScript<CTowerScript>()->GetDamage(m_uiDamage);
		CServer::GetInst()->send_projectile_packet(m_GetId(), 2);
		DeleteObject(GetObj());
	}
}

void CProjectileScript::Init()
{
	m_SetId(SHARED_DATA::g_bulletindex);
	SHARED_DATA::g_bullet[m_GetId()] = Transform()->GetLocalPos();
	CServer::GetInst()->send_projectile_packet(m_GetId(), 0);
	SHARED_DATA::g_bulletindex++;
}

CProjectileScript::CProjectileScript() :CScript((UINT)SCRIPT_TYPE::PROJECTILESCRIPT), m_pObject(nullptr), m_fSpeed(), m_uiDamage(), m_fAlpha(0.f)
, m_bRotate(false), m_eProjectileType(PROJECTILE_TYPE::MINION)
{
}

CProjectileScript::~CProjectileScript()
{
}