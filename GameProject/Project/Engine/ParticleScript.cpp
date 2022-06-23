#include "pch.h"
#include "ParticleScript.h"

CParticleScript::CParticleScript():CScript((UINT)SCRIPT_TYPE::PARTICLE)
{
}

CParticleScript::~CParticleScript()
{
}

void CParticleScript::Update()
{
	if (CoolTimeCheck(m_cstart, m_iCoolTime)) {
		DeleteObject(GetObj());
	}

}
