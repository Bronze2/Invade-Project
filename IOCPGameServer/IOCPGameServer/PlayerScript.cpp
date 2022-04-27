#include "pch.h"
#include "PlayerScript.h"

void CPlayerScript::Init()
{
}

void CPlayerScript::Awake()
{
}

void CPlayerScript::Update()
{
	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vRot = Transform()->GetLocalRot();
	Vec3 vLocalPos = Transform()->GetLocalPos();
	Vec3 vWorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	vLocalPos.x -= vWorldDir.x * 10.f;
	vLocalPos.z -= vWorldDir.z * 10.f;

	Transform()->SetLocalPos(vLocalPos);
	Transform()->SetLocalRot(vRot);



}




CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
{
}

CPlayerScript::~CPlayerScript()
{
}

