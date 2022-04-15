#include "pch.h"
#include "ProjectileScript.h"
#include "GameObject.h"
#include "Animator3D.h"
#include "MeshRender.h"
#include "Mesh.h"

void CProjectileScript::Update()
{
	vector<tMTBone>* c = const_cast<vector<tMTBone>*>(m_pObject->MeshRender()->GetMesh()->GetBones());
	tMTBone* p= const_cast<tMTBone*>(m_pObject->MeshRender()->GetMesh()->GetBone(m_iBone));

	int d = m_pObject->Animator3D()->GetFrameIdx();
	if (p->vecKeyFrame.size() < d) {
		return;
	}
	
	Transform()->SetLocalPos(p->vecKeyFrame[m_pObject->Animator3D()->GetFrameIdx()].vTranslate);
//	Transform()->SetLocalRot(p->vecKeyFrame[m_pObject->Animator3D()->GetFrameIdx()].qRot);
}

void CProjectileScript::FinalUpdate()
{
}

void CProjectileScript::OnCollision3DEnter(CCollider3D* _pOther)
{
}

CProjectileScript::CProjectileScript():CScript((UINT)SCRIPT_TYPE::PROJECTILESCRIPT),m_pObject(nullptr),m_fSpeed(),m_uiDamage()
{
}

CProjectileScript::~CProjectileScript()
{
}
