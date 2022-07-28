#include "pch.h"
#include "CTowerColScript.h"
#include "TowerScript.h"
void CTowerColScript::Update()
{

	Vec3 vRot=m_pTower->Transform()->GetLocalRot();


	Transform()->SetLocalRot(Vec3(0.f, vRot.y, 0.f));
}

CTowerColScript::CTowerColScript():CScript((UINT)SCRIPT_TYPE::COLTOWER)
{
}

CTowerColScript::~CTowerColScript()
{
}

void CTowerColScript::OnCollision3DEnter(CCollider3D* _pOther)
{

}

void CTowerColScript::OnCollision3D(CCollider3D* _pOther)
{
}

void CTowerColScript::OnCollision3DExit(CCollider3D* _pOther)
{
}
