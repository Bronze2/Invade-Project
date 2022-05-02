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
	Transform()->SetLocalPos(SHARED_DATA::g_clients[m_id].Pos);
	Transform()->SetLocalRot(SHARED_DATA::g_clients[m_id].Rot);
}




CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
{
}

CPlayerScript::~CPlayerScript()
{
}

