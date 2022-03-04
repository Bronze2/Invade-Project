#include "pch.h"
#include "ToolCamScript.h"

#include "Camera.h"
#include "Transform.h"
void CToolCamScript::Update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;


	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x -= vDrag.y * DT * 3.f;



	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
}

CToolCamScript::CToolCamScript():CScript(0),m_fSpeed(200.f),m_fScaleSpeed(1.f)
{
}

CToolCamScript::~CToolCamScript()
{
}
