#include "pch.h"
#include "PlayerColScript.h"
#include "PlayerScript.h"
CPlayerColScript::CPlayerColScript() :CScript((UINT)SCRIPT_TYPE::COLPLAYER) {


}
CPlayerColScript::~CPlayerColScript(){

}
void CPlayerColScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	m_pPlayer->GetScript<CPlayerScript>()->OnCollision3DEnter(_pOther);
}
void CPlayerColScript::OnCollision3D(CCollider3D* _pOther)
{
	m_pPlayer->GetScript<CPlayerScript>()->OnCollision3D(_pOther);
}
void CPlayerColScript::OnCollision3DExit(CCollider3D* _pOther)
{
	m_pPlayer->GetScript<CPlayerScript>()->OnCollision3DExit(_pOther);
}
void CPlayerColScript::Update() {

	Vec3 vRot = Transform()->GetLocalRot();
	vRot = m_vCalRot;
	Transform()->SetLocalPos(m_pPlayer->Transform()->GetLocalPos());
	Transform()->SetLocalRot(vRot);


	m_vCalRot = Vec3();

}