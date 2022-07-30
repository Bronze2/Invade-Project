#include "pch.h"
#include "CrossHairScript.h"

#include "Device.h"
#include "ConstantBuffer.h"
#include "RenderMgr.h"
#include "Camera.h"
#include "BowScript.h"

void CCrossHairScript::Init()
{
    m_vRestoreScale = Transform()->GetLocalScale();
}

void CCrossHairScript::Update()
{
    Vec3 vScale = Transform()->GetLocalScale();
    Matrix matProj = m_pMainCam->Camera()->GetProjMat();

    tResolution res = CRenderMgr::GetInst()->GetResolution();
    RECT rtClient;
    GetClientRect(CRenderMgr::GetInst()->GethWnd(), &rtClient);
    Vec3 vTargetPos;

    Vec3 vCrossHairPos = Vec3(rtClient.left + res.fWidth / 2.f, rtClient.top - res.fHeight / 2.f + res.fHeight / 4.f, 1.f);

    vTargetPos.x = ((2.f * vCrossHairPos.x) / res.fWidth - 1.f);
    vTargetPos.y = ((-2.f * vCrossHairPos.y) / res.fHeight + 1.f);
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

    //m_vPos = XMVector2TransformCoord(Vec3(0.f, 0.f, 0.f), matViewInv);
    //m_vDir = XMVector3TransformNormal(vView, matViewInv);
    m_vDir.Normalize();

    m_vPos = vCrossHairPos + m_vDir * 100.f;


    if (KEY_HOLD(KEY_TYPE::KEY_LBTN)) {
        if (vScale.x >= 30.f && vScale.y >= 30.f) {
            vScale.x -= 170.f * DT;
            vScale.y -= 170.f * DT;
        }
    }
    if (KEY_AWAY(KEY_TYPE::KEY_LBTN)) {
        vScale = m_vRestoreScale;
    }

    Vec3 vPos = vTargetPos;
    //vPos.x -= m_vRestoreScale.x / 2.f;
    vPos.y += m_vRestoreScale.y / 2.f;

    Transform()->SetLocalPos(vPos);
    Transform()->SetLocalScale(vScale);
}

CCrossHairScript::CCrossHairScript() :CScript(0)
{

}

CCrossHairScript::~CCrossHairScript()
{
}