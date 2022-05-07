#include "pch.h"
#include "ProjectileScript.h"
#include "GameObject.h"
#include "Animator3D.h"
#include "MeshRender.h"
#include "Mesh.h"
#include "Collider3D.h"
#include "MinionScript.h"

void CProjectileScript::Update()
{
	if (m_pObject->IsDead()) {
		DeleteObject(GetObj());
		return;
	}

	Vec3 vLocalPos = Transform()->GetLocalPos();
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
		//tMTBone* p = const_cast<tMTBone*>(m_pObject->MeshRender()->GetMesh()->GetBone(m_iBone));
		//int d = m_pObject->Animator3D()->GetFrameIdx();
		//if (p->vecKeyFrame.size() < d) {
		//	return;
		//}
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

	if (!m_bRotate) {
		Vec3 vRot = Transform()->GetLocalRot();
		float angle = atan2(vPos.x - m_vTargetPos.x, vPos.z - m_vTargetPos.z) * (180 / PI);
		float rotate = angle * 0.0174532925f;
		vRot.y = rotate;

		Transform()->SetLocalRot(vRot);
		m_bRotate = true;
	}

	if (vPos.z != 0.f&& vPos.x != 0.f) {
		Vec3 vvalue = vPos - m_vTargetPos;
		vvalue.Normalize();
		int b = 0;
	}
	int a = 0;
	float xvalue = m_vDir.x * 100.f * DT;

	// 이렇게 하면 행복해요 (저는...)
	//vLocalPos -= vWorldDir * 500.f * DT;
	vLocalPos -= m_vDir * 500.f * DT;
	vLocalPos.y = vRestoreLocalPos.y;

	Transform()->SetLocalPos(vLocalPos);

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
					DeleteObject(GetObj());
				}
			}
			else {
				if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCampName() != m_pObject->GetScript<CTowerScript>()->GetCampName())
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
	Vec3 vLocalPos = Transform()->GetLocalPos();
	//Vec3 vLocalRot = Transform()->GetLocalRot();
	//Vec3 vLocalScale = Transform()->GetLocalScale();

	//Matrix matTranslation = XMMatrixTranslation(vLocalPos.x, vLocalPos.y, vLocalPos.z);
	//Matrix matScale = XMMatrixScaling(vLocalScale.x, vLocalScale.y, vLocalScale.z);


	//Matrix matRot = XMMatrixRotationX(vLocalRot.x);
	//matRot *= XMMatrixRotationY(vLocalRot.y);
	//matRot *= XMMatrixRotationZ(vLocalRot.z);
	//Matrix matWorld = matScale * matRot  * matTranslation;
	//matWorld *= m_matObjectWorldMatrix;
	//Vec3 vPos = matWorld.Translation();
	//Vec3 vTest = m_vTargetPos  - vPos;
	//Vec3 vTest = m_vTargetPos - Transform()->GetWorldPos();
	//vTest.Normalize();
	//m_vDir = vTest;
}

CProjectileScript::CProjectileScript():CScript((UINT)SCRIPT_TYPE::PROJECTILESCRIPT),m_pObject(nullptr),m_fSpeed(),m_uiDamage(),m_fAlpha(0.f)
,m_bRotate(false), m_eProjectileType(PROJECTILE_TYPE::MINION)
{
}

CProjectileScript::~CProjectileScript()
{
}
