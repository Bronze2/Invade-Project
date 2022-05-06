#include "pch.h"
#include "SpawnScript.h"
#include <chrono>
#include "Ptr.h"
#include "MeshData.h"
#include "ResMgr.h"
#include "Collider3D.h"
#include "MeshRender.h"
#include "Animator3D.h"
#include "Sensor.h"
<<<<<<< HEAD
#include "ProjectileScript.h"

CGameObject* CSpawnScript::SpawnObject(const wstring& _strKey, Vec3 _vLocalPos, 
    Vec3 _vLocalScale, Vec3 _vOffsetPos, Vec3 _vOffsetScale, MINION_ATTACK_TYPE _eAttackRange,
    CAnimation* _pAnimation, CAMP_STATE _eCamp)
=======
CGameObject* CSpawnScript::SpawnObject(const wstring& _strKey, Vec3 _vLocalPos, Vec3 _vLocalScale, Vec3 _vOffsetPos, Vec3 _vOffsetScale, MINION_ATTACK_TYPE _eAttackRange, CAnimation* _pAnimation, CAMP_STATE _eCamp)
>>>>>>> parent of 1dcb62e (Minion_Attack_Pos)
{
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(_strKey, _strKey);
	CGameObject* pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CMinionScript);
	pObject->AddComponent(new CSensor);
	//pObject->Sensor()->SetRadius(300.f);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(_vOffsetScale);
	pObject->Collider3D()->SetOffsetPos(_vOffsetPos);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(_vLocalPos);
	pObject->Transform()->SetLocalScale(_vLocalScale);
   // if (CAMP_STATE::BLUE == _eCamp)
   //     pObject->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-180.f), 0.f, 0.f));
    pObject->MeshRender()->SetDynamicShadow(true);
	pObject->Animator3D()->SetAnimation(_pAnimation);

	pObject->GetScript<CMinionScript>()->SetNexus(m_pNexus);
	pObject->GetScript<CMinionScript>()->SetAttackType(_eAttackRange);
	pObject->GetScript<CMinionScript>()->SetCamp(_eCamp);
	pObject->GetScript<CMinionScript>()->Init();


	return pObject;
}

void CSpawnScript::Update()
{
}

void CSpawnScript::SpawnObject_Red(int id, Vec3 Pos , MINION_ATTACK_TYPE type)
{
    CAnimation* pNewAnimation = new CAnimation;
    switch (type)
    {
    case MINION_ATTACK_TYPE::MELEE:
    {
        pNewAnimation->InsertAnimClip(L"WALK", 30, 49);
        pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
        pNewAnimation->InsertAnimClip(L"DIE", 75, 114);
        Vec3 vPos = GetObj()->Transform()->GetLocalPos();
        CGameObject* pObject = SpawnObject(L"MeshData\\sword_min1.mdat",
            vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f),
            MINION_ATTACK_TYPE::MELEE, pNewAnimation, CAMP_STATE::RED);
        pObject->SetId(id);
        CreateObject(pObject, L"Red");
    }
        break;
    case MINION_ATTACK_TYPE::RANGE:
    {
        pNewAnimation->InsertAnimClip(L"WALK", 31, 49);
        pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
        pNewAnimation->InsertAnimClip(L"DIE", 100, 149);
        Vec3 vPos = GetObj()->Transform()->GetLocalPos();
<<<<<<< HEAD
        CGameObject* pObject = SpawnObject(L"MeshData\\wizard_min1.mdat", 
            vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f),
            MINION_ATTACK_TYPE::RANGE, pNewAnimation, CAMP_STATE::RED);
=======
        CGameObject* pObject = SpawnObject(L"MeshData\\wizard_min1.mdat", Pos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE, pNewAnimation, CAMP_STATE::RED);
>>>>>>> parent of 1dcb62e (Minion_Attack_Pos)
        pObject->SetId(id);
        CreateObject(pObject, L"Red");
    }
        break;
    case MINION_ATTACK_TYPE::CANON:
    {
        pNewAnimation->InsertAnimClip(L"WALK", 41, 74);
        pNewAnimation->InsertAnimClip(L"ATTACK", 75, 99);
        pNewAnimation->InsertAnimClip(L"DIE", 125, 174);
        Vec3 vPos = GetObj()->Transform()->GetLocalPos();
<<<<<<< HEAD
        CGameObject* pObject = SpawnObject(L"MeshData\\Canon_min1.mdat", 
            vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f),
            MINION_ATTACK_TYPE::CANON, pNewAnimation, CAMP_STATE::RED);
=======
        CGameObject* pObject = SpawnObject(L"MeshData\\Canon_min1.mdat", Pos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::CANON, pNewAnimation, CAMP_STATE::RED);
>>>>>>> parent of 1dcb62e (Minion_Attack_Pos)
        pObject->SetId(id);
        CreateObject(pObject, L"Red");
    }
        break;
    default:
        break;
    }

}

void CSpawnScript::SpawnObject_Blue(int id, Vec3 Pos, MINION_ATTACK_TYPE type)
{

    CAnimation* pNewAnimation = new CAnimation;

    switch (type)
    {
    case MINION_ATTACK_TYPE::MELEE:
    {
        pNewAnimation->InsertAnimClip(L"WALK", 35, 49);
        pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
        pNewAnimation->InsertAnimClip(L"DIE", 75, 124);
        Vec3 vPos = GetObj()->Transform()->GetLocalPos();
        CGameObject* pObject = SpawnObject(L"MeshData\\sword_min.mdat", Pos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 20.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE, pNewAnimation, CAMP_STATE::BLUE);
        pObject->SetId(id);
        CreateObject(pObject, L"Blue");
    }
        break;
    case MINION_ATTACK_TYPE::RANGE:
    {
        pNewAnimation->InsertAnimClip(L"WALK", 36, 59);
        pNewAnimation->InsertAnimClip(L"ATTACK", 60, 84);
        pNewAnimation->InsertAnimClip(L"DIE", 110, 159);
        Vec3 vPos = GetObj()->Transform()->GetLocalPos();
        CGameObject* pObject = SpawnObject(L"MeshData\\wizard_min.mdat", Pos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 25.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE, pNewAnimation, CAMP_STATE::BLUE);
        pObject->SetId(id);
        CreateObject(pObject, L"Blue");
    }
        break;
    case MINION_ATTACK_TYPE::CANON:
    {
        pNewAnimation->InsertAnimClip(L"WALK", 31, 54);
        pNewAnimation->InsertAnimClip(L"ATTACK", 80, 104);
        pNewAnimation->InsertAnimClip(L"DIE", 105, 154);
        Vec3 vPos = GetObj()->Transform()->GetLocalPos();
        CGameObject* pObject = SpawnObject(L"MeshData\\Canon_min.mdat", Pos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 25.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::CANON, pNewAnimation, CAMP_STATE::BLUE);
        pObject->SetId(id);
        CreateObject(pObject, L"Blue");
    }
        break;
    default:
        break;
    }

}
void CSpawnScript::SpawnObject_Pro(int id, Vec3 Pos)
{
    Vec3 vPos = GetObj()->Transform()->GetLocalPos();
    CGameObject* pObject = SpawnObject_Proejectile(L"MeshData\\redball.mdat", Pos,
        Vec3(0.1f, 0.1f, 0.1f), Vec3(0.f, 0.f, 0.f), Vec3(100.f, 100.f, 100.f));
    pObject->GetScript<CProjectileScript>()->m_SetId(id);
    CreateObject(pObject, L"Blue");
    std::cout << "Åº»ý" << endl;
}



CGameObject* CSpawnScript::SpawnObject_Proejectile(const wstring& _strKey, Vec3 _vLocalPos,
    Vec3 _vLocalScale, Vec3 _vOffsetPos, Vec3 _vOffsetScale)
{
    Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(_strKey, _strKey);
    CGameObject* pObject = pMeshData->Instantiate();
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->AddComponent(new CProjectileScript);
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(_vOffsetScale);
    pObject->Collider3D()->SetOffsetPos(_vOffsetPos);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(_vLocalPos);
    pObject->Transform()->SetLocalScale(_vLocalScale);
    pObject->MeshRender()->SetDynamicShadow(true);
    pObject->GetScript<CProjectileScript>()->Init();


    return pObject;
}








CSpawnScript::CSpawnScript() :CScript((UINT)SCRIPT_TYPE::SPAWNSCRIPT),m_bClockStart(false),m_eSpawnPattern(SPAWN_PATTERN::PATTERN1)
,m_bSpawnStart(false),m_uiCount(0),m_bPatternOn(false)
{
}

CSpawnScript::~CSpawnScript()
{
}


