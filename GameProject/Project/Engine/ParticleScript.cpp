#include "pch.h"
#include "ParticleScript.h"

CParticleScript::CParticleScript():CScript((UINT)SCRIPT_TYPE::PARTICLE)
{
}

CParticleScript::~CParticleScript()
{
	if (nullptr != m_pSound) {
		delete m_pSound;
		m_pSound = nullptr;
	}
}

void CParticleScript::Update()
{
	if (CoolTimeCheck(m_cstart, m_iCoolTime)) {
		DeleteObject(GetObj());
	}

}
