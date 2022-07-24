#include "pch.h"
#include "SafeZone.h"

#include "Transform.h"
#include "CollisionMgr.h"
#include "PlayerScript.h"
#include "Collider3D.h"
#include "InGameScene.h"
CSafeZone::CSafeZone():CScript((UINT)SCRIPT_TYPE::SAFEZONE)
{
}

CSafeZone::~CSafeZone()
{
}

void CSafeZone::Update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	Collision(vPos);
}

void CSafeZone::Collision(const Vec3& _vPos)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	Vec3 vFinalPos = Collider3D()->GetOffsetPos();
	vFinalPos = vFinalPos / Transform()->GetWorldScale();
	Matrix matColTranslation = XMMatrixTranslation(vFinalPos.x, vFinalPos.y, vFinalPos.z);
	Matrix matColScale = XMMatrixScaling(Collider3D()->GetOffsetScale().x, Collider3D()->GetOffsetScale().y, Collider3D()->GetOffsetScale().z);
	Matrix MatColWorld = matColScale * matColTranslation;
	Matrix matTranslation = XMMatrixTranslation(_vPos.x, _vPos.y, _vPos.z);
	Matrix matScale = XMMatrixScaling(Transform()->GetLocalScale().x, Transform()->GetLocalScale().y, Transform()->GetLocalScale().z);
	Matrix matRot = XMMatrixRotationX(Transform()->GetLocalRot().x);
	matRot *= XMMatrixRotationY(Transform()->GetLocalRot().y);
	matRot *= XMMatrixRotationZ(Transform()->GetLocalRot().z);
	Matrix matWorld = matScale * matRot * matTranslation;
	MatColWorld *= matWorld;
	bool bTrue = false;
	for (int i = (UINT)INGAME_LAYER::BLUE; i < (UINT)INGAME_LAYER::RED + 1; ++i) {
		const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();
		if (i == 6)
			continue;
		for (int j = 0; j < vecObj.size(); ++j) {
			if (vecObj[j] == GetObj())
				continue;
			if (vecObj[j]->IsDead())
				continue;
			if (!vecObj[j]->Collider3D())
				continue;
			if (L"Projectile" == vecObj[j]->GetName())
				continue;
			if (L"Minion" == vecObj[j]->GetName())
				continue;
			if (nullptr != vecObj[j]->GetScript<CPlayerScript>()) {
				if ((UINT)vecObj[j]->GetScript<CPlayerScript>()->GetCampState() == (UINT)m_eState)
				{
					bool bTrue=CCollisionMgr::GetInst()->CollisionCubeMatrix(MatColWorld, vecObj[j]->Collider3D()->GetColliderWorldMatNotify());
					if (bTrue) {
						vecObj[j]->GetScript<CPlayerScript>()->SetCanAttack(true);
					}
					else {
						vecObj[j]->GetScript<CPlayerScript>()->SetCanAttack(false);
					}
				}
			}
		}

	}


	

}
