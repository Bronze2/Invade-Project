#include "pch.h"
#include "UIBarScript.h"
#include "RenderMgr.h"
#include "Camera.h"

void CUIBarScript::Awake()
{
	m_tRes = CRenderMgr::GetInst()->GetResolution();
}

void CUIBarScript::Update()
{

}

void CUIBarScript::ConvertPos(Vec3 _vPos)
{


}

void CUIBarScript::SaveToScene(FILE* _pFile)
{
}

void CUIBarScript::LoadFromScene(FILE* _pFile)
{
}


CUIBarScript::CUIBarScript() : CScript(0)
{

}

CUIBarScript::~CUIBarScript()
{

}
