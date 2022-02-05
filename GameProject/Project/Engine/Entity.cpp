#include "pch.h"
#include "Entity.h"

UINT CEntity::g_iID = 0;

CEntity::CEntity():m_iID(g_iID++)
{
}

CEntity::CEntity(const CEntity& _Other):m_strName(_Other.m_strName),m_iID(g_iID++)
{
}
