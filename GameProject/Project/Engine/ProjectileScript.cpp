#include "pch.h"
#include "ProjectileScript.h"
#include "GameObject.h"
#include "Animator3D.h"
#include "MeshRender.h"
#include "Mesh.h"
#include "Collider3D.h"
#include "MinionScript.h"
#include "SceneMgr.h"

bool CProjectileScript::M_FLengthCheck(const Vec3& _Pos)
{
	float value = sqrt(pow(m_vStartPos.x - _Pos.x, 2) + pow(m_vStartPos.y - _Pos.y, 2) + pow(m_vStartPos.z - _Pos.z, 2));
	if (value > m_fLength) {
		return true;
	}

	return false;
}

void CProjectileScript::Update()
{
	if (!CSceneMgr::GetInst()->get_ProjectileDelete(m_id)) {
		Vec3 vPos = Vec3::Lerp(Transform()->GetLocalPos(), CSceneMgr::GetInst()->get_ProjectilePos(m_id), DT * 10.f);
		Transform()->SetLocalPos(vPos);
	}
	else {
		DeleteObject(GetObj());
	}

}


#include "TowerScript.h"
void CProjectileScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	//if (m_pObject = _pOther->GetObj())
	//	return;
	//if (_pOther->GetObj()->GetScript<CMinionScript>() != nullptr) {
	//	if (nullptr != _pOther->GetObj()) {
	//		if (m_pObject->GetScript<CMinionScript>() != nullptr) {
	//			if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() != m_pObject->GetScript<CMinionScript>()->GetCamp())
	//			{
	//				_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
	//				DeleteObject(GetObj());
	//			}
	//		}
	//		else {
	//			//if ((UINT)(_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp()) != (UINT)(m_pObject->GetScript<CTowerScript>()->GetCamp()))
	//			//{
	//			//	_pOther->GetObj()->GetScript<CMinionScript>()->GetDamage(m_uiDamage);
	//			//	DeleteObject(GetObj());
	//			//}
	//		}
	//	}

	//}

}

void CProjectileScript::Init()
{
	//Vec3 vLocalPos = Transform()->GetLocalPos();
	//Vec3 vLocalRot = Transform()->GetLocalRot();
	//Vec3 vLocalScale = Transform()->GetLocalScale();

	//Matrix matTranslation = XMMatrixTranslation(vLocalPos.x, vLocalPos.y, vLocalPos.z);
	//Matrix matScale = XMMatrixScaling(vLocalScale.x, vLocalScale.y, vLocalScale.z);


	//Matrix matRot = XMMatrixRotationX(vLocalRot.x);
	//matRot *= XMMatrixRotationY(vLocalRot.y);
	//matRot *= XMMatrixRotationZ(vLocalRot.z);
	//Matrix matWorld = matScale * matRot * matTranslation;
	//matWorld *= m_matObjectWorldMatrix;
	//Vec3 vPos = matWorld.Translation();
	//Vec3 vTest = m_vTargetPos - vPos;
	//vTest.Normalize();
	//m_vDir = vTest;

}

CProjectileScript::CProjectileScript() :CScript((UINT)SCRIPT_TYPE::PROJECTILESCRIPT), m_pObject(nullptr), m_fSpeed(), m_uiDamage(), m_fAlpha(0.f)
, m_bRotate(false), m_eProjectileType(PROJECTILE_TYPE::MINION), m_fLength(300.f), m_bUpdate(false), m_bLaunch(0)
{
}

CProjectileScript::~CProjectileScript()
{
}