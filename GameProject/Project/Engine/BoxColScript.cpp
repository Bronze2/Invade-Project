#include "pch.h"
#include "BoxColScript.h"

void CBoxColScript::Update()
{
}

CBoxColScript::CBoxColScript():CScript((UINT)SCRIPT_TYPE::COLBOX)
{
}

CBoxColScript::~CBoxColScript()
{
}

void CBoxColScript::OnCollision3DEnter(CCollider3D* _pOther)
{
}

void CBoxColScript::OnCollision3D(CCollider3D* _pOther)
{
}

void CBoxColScript::OnCollision3DExit(CCollider3D* _pOther)
{
}
