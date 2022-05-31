#include "pch.h"
#include "CrossHairScript.h"

#include "StaticUI.h"
#include "Device.h"
#include "ConstantBuffer.h"
#include "RenderMgr.h"
#include "Camera.h"

void CCrossHairScript::Init()
{
}

void CCrossHairScript::Update()
{
	Vec3 vScale = Vec3(100.f, 100.f, 1.f);
	Matrix matProj = m_pMainCam->Camera()->GetProjMat();

	tResolution res = CRenderMgr::GetInst()->GetResolution();
	RECT rtWindow;
	GetWindowRect(CRenderMgr::GetInst()->GethWnd(), &rtWindow);
	Vec3 vTargetPos;

	Vec3 vCrossHairPos = Vec3(rtWindow.left + res.fWidth/2, rtWindow.top - res.fHeight/2, 1.f);

	vTargetPos.x = (((2.f * vCrossHairPos.x) / res.fWidth) - 1.f);
	vTargetPos.y = (((-2.f * vCrossHairPos.y) / res.fHeight) + 1.f);
	vTargetPos.z = 1.f;

	Vec3 vProj = vTargetPos;

	Vec3 vView;
	vView.x = vProj.x / matProj._11;
	vView.y = vProj.y / matProj._22;
	vView.z = vProj.z;

	Matrix matViewInv = m_pMainCam->Camera()->GetViewInvMat();

	m_vDir.x = vView.x * matViewInv._11 + vView.y * matViewInv._21 + vView.z * matViewInv._31;
	m_vDir.y = vView.x * matViewInv._12 + vView.y * matViewInv._22 + vView.z * matViewInv._32;
	m_vDir.z = vView.x * matViewInv._13 + vView.y * matViewInv._23 + vView.z * matViewInv._33;
	m_vDir.Normalize();

	m_vPos = Vec3(matViewInv._41, matViewInv._42, matViewInv._43);

	m_pMainCam->Camera()->SetRay(m_vPos, m_vDir);

	Vec3 vPos = vTargetPos;
	vPos.x -= vScale.x / 2;
	vPos.y += vScale.y / 2;

	Transform()->SetLocalPos(vPos);
}

CCrossHairScript::CCrossHairScript() :CScript(0)
{

}

CCrossHairScript::~CCrossHairScript()
{
}