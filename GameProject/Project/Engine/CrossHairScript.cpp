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
	/*CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject* pEmptyObject = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1]);
	CGameObject* pCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[0];
	CGameObject* pEmptyCamera = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[1];

	tResolution res = CRenderMgr::GetInst()->GetResolution();

	POINT pMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec3 vMousePos = Vec3(-res.fWidth / 2 + pMousePos.x, res.fHeight / 2 - pMousePos.y, 1);
	Transform()->SetLocalPos(vMousePos);*/

	// CrossHair의 3D상 좌표를 구해볼까..

	Vec3 vScale = Vec3(100.f, 100.f, 1.f);
	Matrix matProj = m_pMainCam->Camera()->GetProjMat();


	tResolution res = CRenderMgr::GetInst()->GetResolution();
	RECT rtWindow;
	GetWindowRect(CRenderMgr::GetInst()->GethWnd(), &rtWindow);


	POINT ptCursorPos;
	GetCursorPos(&ptCursorPos);
	ScreenToClient(CRenderMgr::GetInst()->GethWnd(), &ptCursorPos);
	Vec3 vMousePos;
	POINT pMousePos = CKeyMgr::GetInst()->GetMousePos();

	Vec3 vCrossHairPos = Vec3(rtWindow.left + res.fWidth/2 /*+ vScale.x / 2*/, rtWindow.top - res.fHeight/2/* - vScale.y / 2*/, 1.f);

	vMousePos.x = (((2.f * vCrossHairPos.x) / res.fWidth) - 1.f);// -matProj._31;
	vMousePos.y = (((-2.f * vCrossHairPos.y) / res.fHeight) + 1.f);// -matProj._32;
	//vMousePos.x = (((2.0f * ptCursorPos.x - 2.0f * rtWindow.left) / rtWindow.right) - 1.f);// -matProj._31;
	//vMousePos.y = (-((2.0f * ptCursorPos.y - 2.0f * rtWindow.top) / rtWindow.bottom) - 1.f);// -matProj._32;
	vMousePos.z = 1.f;

	//vMousePos.x = ((vScale.x / 2 - res.fWidth / 2));
	//vMousePos.y = -((vScale.y / 2 - res.fHeight / 2));
	//vMousePos.z = 1.f;

	//Vec3 vProj = Transform()->GetLocalPos();
	Vec3 vProj = vMousePos;

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

	Vec3 vPos = vMousePos;
	vPos.x -= vScale.x / 2;
	vPos.y += vScale.y / 2;

	Transform()->SetLocalPos(vPos);
}

CCrossHairScript::CCrossHairScript() :CScript(0)
{
	//m_pRay = new SimpleMath::Ray;
}

CCrossHairScript::~CCrossHairScript()
{
	//if (nullptr != m_pRay)
	//{
	//	delete m_pRay;
	//}
}