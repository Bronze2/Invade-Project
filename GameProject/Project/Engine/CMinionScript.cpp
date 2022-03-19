#include "pch.h"
#include "CMinionScript.h"

void CMinionScript::Init()
{
}

void CMinionScript::Update()
{
}

CMinionScript::CMinionScript():CScript((UINT)SCRIPT_TYPE::MINIONSCRIPT)
{
}

CMinionScript::CMinionScript(float _fSpeed, float _fRange, MINION_STATE _eState, MINION_CAMP _eCamp)
	: CScript((UINT)SCRIPT_TYPE::MINIONSCRIPT),m_fSpeed(_fSpeed),m_fRange(_fRange),m_eState(_eState),m_eCamp(_eCamp)
{
}

CMinionScript::~CMinionScript()
{
}
