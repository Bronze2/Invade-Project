#include "pch.h"
#include "ProjectileScript.h"
#include "GameObject.h"
#include "Collider3D.h"
#include "MinionScript.h"
#include "TimeMgr.h"
#include "Server.h"

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
		/*vector<tMTBone>* c = const_cast<vector<tMTBone>*>(m_pObject->MeshRender()->GetMesh()->GetBones());
		tMTBone* p = const_cast<tMTBone*>(m_pObject->MeshRender()->GetMesh()->GetBone(m_iBone));
		int d = m_pObject->Animator3D()->GetFrameIdx();
		if (p->vecKeyFrame.size() < d) {
			return;
		}
		Transform()->SetLocalPos(p->vecKeyFrame[m_pObject->Animator3D()->GetFrameIdx()].vTranslate);
		if (0 == m_bLaunch)
			return;
		else if (1 == m_bLaunch) {
			if (m_pTarget->IsDead())
			{
				DeleteObject(GetObj());
				return;
			}
			if (nullptr == m_pTarget) {
				DeleteObject(GetObj());
				return;
			}

			Vec3 vObjPos = m_pObject->Transform()->GetLocalPos();
			Vec3 vObjRot = m_pObject->Transform()->GetLocalRot();
			Matrix matTranslation = XMMatrixTranslation(vObjPos.x, vObjPos.y, vObjPos.z);
			Vec3 vObjScale = m_pObject->Transform()->GetLocalScale();
			Matrix matScale = XMMatrixScaling(vObjScale.x, vObjScale.y, vObjScale.z);
			vObjRot = m_pObject->Transform()->GetLocalRot();
			vObjRot.y -= m_fRoty;
			Matrix matRot = XMMatrixRotationX(vObjRot.x);
			matRot *= XMMatrixRotationY(vObjRot.y);
			matRot *= XMMatrixRotationZ(vObjRot.z);

			m_matObjectWorldMatrix = matScale * matRot * matTranslation;

			Transform()->SetObjectMatrix(m_matObjectWorldMatrix);

			Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
			float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
			float rotate = angle * 0.0174532925f;
			vRot.y = rotate;
			Transform()->SetLocalRot(vRot);
			++m_bLaunch;
			return;
		}
		else {

		}*/


	}

	break;
	case PROJECTILE_TYPE::TOWER:
	{

	}
	break;
	default:
		break;
	}



	Vec3 vvalue;
	if (m_bUpdate) {
		if (M_FLengthCheck(vPos)) {
			DeleteObject(GetObj());
			return;
		}
	}

	Vec3 vWorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);



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
		float beforey = m_pObject->Transform()->GetLocalRot().y;
		float aftery = XMConvertToDegrees(m_pObject->Transform()->GetLocalRot().y);
		//	vLocalPos.x += vWorldDir.x * m_fSpeed * DT;
		vLocalPos.y += m_vDir.y * m_fSpeed * DT;
		if (m_vDir.x) {

		}
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


	//cout <<"[ "<< m_id<< " ,Bullet Pos]" << vLocalPos.x << "," << vLocalPos.y << "," << vLocalPos.z << endl;
	Transform()->SetLocalPos(vLocalPos);
	CServer::GetInst()->send_projectile_packet(m_GetId(), 1);
	if (!m_bUpdate)
	{
		m_bUpdate = true;
	}

}


#include "TowerScript.h"
void CProjectileScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	if (m_pObject = _pOther->GetObj())
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
	Vec3 vLocalPos = Transform()->GetLocalPos();
	Vec3 vLocalRot = Transform()->GetLocalRot();
	Vec3 vLocalScale = Transform()->GetLocalScale();

	Matrix matTranslation = XMMatrixTranslation(vLocalPos.x, vLocalPos.y, vLocalPos.z);
	Matrix matScale = XMMatrixScaling(vLocalScale.x, vLocalScale.y, vLocalScale.z);


	Matrix matRot = XMMatrixRotationX(vLocalRot.x);
	matRot *= XMMatrixRotationY(vLocalRot.y);
	matRot *= XMMatrixRotationZ(vLocalRot.z);
	Matrix matWorld = matScale * matRot * matTranslation;
	matWorld *= m_matObjectWorldMatrix;
	Vec3 vPos = matWorld.Translation();
	Vec3 vTest = m_vTargetPos - vPos;
	vTest.Normalize();
	m_vDir = vTest;

	std::cout << "Bullet Init  ";
	cout << SHARED_DATA::g_bulletindex << endl;
	m_id = SHARED_DATA::g_bulletindex;
}

CProjectileScript::CProjectileScript() :CScript((UINT)SCRIPT_TYPE::PROJECTILESCRIPT), m_pObject(nullptr), m_fSpeed(), m_uiDamage(), m_fAlpha(0.f)
, m_bRotate(false), m_eProjectileType(PROJECTILE_TYPE::MINION), m_fLength(300.f), m_bUpdate(false), m_bLaunch(0)
{
}

CProjectileScript::~CProjectileScript()
{
}