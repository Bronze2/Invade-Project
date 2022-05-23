#include "pch.h"
#include "WindSkill1Script.h"

#include "PlayerScript.h"
#include "Collider3D.h"
CWindSkill1Script::CWindSkill1Script() :CScript((UINT)SCRIPT_TYPE::WINDSKILL1), m_bStart(false)
{
}

CWindSkill1Script::~CWindSkill1Script()
{
}

void CWindSkill1Script::Update()
{
	if (nullptr == m_tSkill)return;
	Vec3 vWorldPos=Transform()->GetWorldPos();
	if (!m_bStart) {
		m_bStart=true;
	}
	else {
		m_fLength = sqrt(pow(vWorldPos.x - m_vStartPos.x, 2) + pow(vWorldPos.y - m_vStartPos.y, 2) + pow(vWorldPos.z - m_vStartPos.z, 2));
		if (m_fLength >= 1000.f) {
			GetObj()->SetActive(false);
		}
	}
	Vec3 vPos = Transform()->GetLocalPos();

	if (vPos.y < 0.f) {
		GetObj()->SetActive(false);
	}
	Vec3 vRot = Transform()->GetLocalRot();


	Vec3 vFront = vPos;
	Vec3 vRight = Vec3(1, 0, 0);
	auto p = XMLoadFloat3(&vRight);
	auto m = XMLoadFloat4x4(&Transform()->GetWorldMat());
	auto r = XMVector3TransformNormal(p, m);
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Normalize(r));
	vPos.x += result.x * m_fSpeed * DT;
	vPos.z += result.z * m_fSpeed * DT;



	Transform()->SetLocalPos(vPos);

}

void CWindSkill1Script::OnCollision3DEnter(CCollider3D* _pColldier)
{
	if (nullptr != _pColldier->GetObj()->GetScript<CPlayerScript>()) {
		if (_pColldier->GetObj()->GetLayerIdx() !=_pColldier->GetObj()->GetLayerIdx()) {
			_pColldier->GetObj()->GetScript<CPlayerScript>()->SetDamage(m_tSkill->fDamage);

		}
	}
}
