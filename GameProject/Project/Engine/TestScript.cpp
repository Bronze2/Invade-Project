#include "pch.h"
#include "TestScript.h"

void CTestScript::Update()
{
}

void CTestScript::OnCollisionEnter(CCollider2D* _pOther)
{
}

void CTestScript::OnCollisionExit(CCollider2D* _pOther)
{
}

CTestScript::CTestScript():CScript((UINT)SCRIPT_TYPE::TESTSCRIPT),m_iLifeCount(2)
{
}

CTestScript::~CTestScript()
{
}
