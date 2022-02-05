#include "pch.h"
#include "MonsterScript.h"

void CMonsterScript::Update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	if (vPos.x > 600.f)
		m_iDir = -1;
	else if (vPos.x < -600.f)
		m_iDir = 1;
	vPos.x += DT * 100.f * m_iDir;
	Transform()->SetLocalPos(vPos);
}



void CMonsterScript::OnCollisionEnter(CCollider2D* _pOther)
{
	if (L"Bullet Object" == _pOther->GetObj()->GetName())
		DeleteObject(GetObj());
}

void CMonsterScript::OnCollisionExit(CCollider2D* _pOther)
{
}

CMonsterScript::CMonsterScript():CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT),m_iDir(1)
{
}

CMonsterScript::~CMonsterScript()
{
}
