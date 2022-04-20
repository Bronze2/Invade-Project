#include "pch.h"
#include "EmptyPlayerScript.h"
#include "Network.h"

void CEmptyPlayerScript::Update()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject* pPlayer = dynamic_cast<CGameObject*>(pCurScene->FindLayer(L"Blue")->GetParentObj()[Network::GetInst()->getHostId() ]);

	Vec3 vPos = pPlayer->Transform()->GetLocalPos();
	Vec3 vRot = pPlayer->Transform()->GetLocalRot();

	vRot *= Vec3(0.f, 1.f, XMConvertToRadians(90.f));
	vRot.y += XMConvertToRadians(90.f);

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
}


CEmptyPlayerScript::CEmptyPlayerScript() : CScript(0)
{

}

CEmptyPlayerScript::~CEmptyPlayerScript()
{
}