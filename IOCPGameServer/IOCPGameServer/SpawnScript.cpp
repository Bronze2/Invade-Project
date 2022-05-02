#include "pch.h"
#include "SpawnScript.h"
#include <chrono>
#include "MinionScript.h"
#include "PlayerScript.h"
#include "Collider3D.h"
#include "Sensor.h"
#include "Server.h"

CGameObject* CSpawnScript::SpawnObject(Vec3 _vLocalPos, Vec3 _vLocalScale, Vec3 _vOffsetPos, Vec3 _vOffsetScale, MINION_ATTACK_TYPE _eAttackRange, CAMP_STATE _eCamp)
{
	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CMinionScript);
	pObject->AddComponent(new CSensor);
	pObject->Sensor()->SetRadius(300.f);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(_vOffsetScale);
	pObject->Collider3D()->SetOffsetPos(_vOffsetPos);
	pObject->Transform()->SetLocalPos(_vLocalPos);
    //if(CAMP_STATE::BLUE == _eCamp)
   //     pObject->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-180.f), 0.f, 0.f));
	pObject->Transform()->SetLocalScale(_vLocalScale);
	pObject->GetScript<CMinionScript>()->SetNexus(m_pNexus);
	pObject->GetScript<CMinionScript>()->SetAttackType(_eAttackRange);
	pObject->GetScript<CMinionScript>()->SetCamp(_eCamp);
	pObject->GetScript<CMinionScript>()->Init();

    pObject->GetScript<CMinionScript>()->m_SetId(SHARED_DATA::g_minionindex);
    CServer::GetInst()->send_spawn_minion_packet(SHARED_DATA::g_minionindex, pObject->Transform()->GetLocalPos().x,
        pObject->Transform()->GetLocalPos().y, pObject->Transform()->GetLocalPos().z, _eAttackRange,_eCamp);

    SHARED_DATA::g_minionindex++;
	return pObject;
}

CGameObject* CSpawnScript::SpawnPlayer(int p_id,Vec3 _vLocalPos, Vec3 _vLocalScale, Vec3 _vOffsetPos, Vec3 _vOffsetScale, CAMP_STATE _eCamp)
{
    CGameObject* pObject = new CGameObject;
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->AddComponent(new CPlayerScript);
    pObject->AddComponent(new CSensor);
    pObject->Sensor()->SetRadius(300.f);
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(_vOffsetScale);
    pObject->Collider3D()->SetOffsetPos(_vOffsetPos);
    pObject->Transform()->SetLocalPos(_vLocalPos);
    pObject->Transform()->SetLocalScale(_vLocalScale);
    pObject->GetScript<CPlayerScript>()->Init();
    pObject->GetScript<CPlayerScript>()->m_SetId(p_id);
    return pObject;
}


void CSpawnScript::Update()
{
    if (!m_bClockStart) {
        m_uiPatternStart = clock();
        m_bClockStart = true;
    }
    else {
        m_uiPatternEnd = clock();
        m_uiPatternInterval = (m_uiPatternEnd - m_uiPatternStart) / CLOCKS_PER_SEC;

        if (m_uiPatternInterval == PATTERNINTEVAL) {
            if (!m_bPatternOn)
                m_uiPatternStart = m_uiPatternEnd;
            m_bPatternOn = true;
        }
        if (m_bPatternOn) {
            switch (m_eCampState)
            {
            case CAMP_STATE::RED:
            {
                switch (m_eSpawnPattern)
                {
                case SPAWN_PATTERN::PATTERN1:
                {
                    if (!m_bSpawnStart) {
                        m_uiSpawnStart = clock();
                        m_bSpawnStart = true;
                    }
                    else {
                        m_uiSpawnEnd = clock();
                        m_uiSpawnInterval = (m_uiSpawnEnd - m_uiSpawnStart) / CLOCKS_PER_SEC;
                        switch (m_uiCount)
                        {
                        case 0:
                        case 1:
                        case 2:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {
 
                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject(vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE, m_eCampState);
                                CreateObject(pObject, L"Red");


                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                                
                                
                            }
                        }
                        break;
                        case 3:
                        case 4:
                        case 5:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {

                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject( vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE, m_eCampState);
                                CreateObject(pObject, L"Red");
                                
                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 6:
                            m_uiCount = 0;
                            m_bPatternOn = false;
                            m_bSpawnStart = false;

                            m_eSpawnPattern = SPAWN_PATTERN::END;
                            break;

                        default:
                            break;
                        }


                    }
                }

                break;
                case SPAWN_PATTERN::PATTERN2:
                {
                    if (!m_bSpawnStart) {
                        m_uiSpawnStart = clock();
                        m_bSpawnStart = true;
                    }
                    else {
                        m_uiSpawnEnd = clock();
                        m_uiSpawnInterval = (m_uiSpawnEnd - m_uiSpawnStart) / CLOCKS_PER_SEC;
                        switch (m_uiCount)
                        {
                        case 0:
                        case 1:
                        case 2:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {
            
                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject( vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE, m_eCampState);

                                CreateObject(pObject, L"Red");

                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 3:
                        case 4:
                        case 5:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {
                      
                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject( vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE, m_eCampState);

                                CreateObject(pObject, L"Red");

      
                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 6:
                            m_uiCount = 0;
                            m_bPatternOn = false;
                            m_bSpawnStart = false;
                            
                            m_eSpawnPattern = SPAWN_PATTERN::PATTERN3;
                            break;

                        default:
                            break;
                        }
                    }
                }
                break;
                case SPAWN_PATTERN::PATTERN3:
                {
                    if (!m_bSpawnStart) {
                        m_uiSpawnStart = clock();
                        m_bSpawnStart = true;
                    }
                    else {
                        m_uiSpawnEnd = clock();
                        m_uiSpawnInterval = (m_uiSpawnEnd - m_uiSpawnStart) / CLOCKS_PER_SEC;
                        switch (m_uiCount)
                        {
                        case 0:
                        case 1:
                        case 2:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {

                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject(vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE,m_eCampState);

                                CreateObject(pObject, L"Red");

         
                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 3: {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {

                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject( vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::CANON,  m_eCampState);

                                CreateObject(pObject, L"Red");

                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }

                        }
                              break;
                        case 4:
                        case 5:
                        case 6:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {
 
                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject( vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 10.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE, m_eCampState);

                                CreateObject(pObject, L"Red");

       
                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 7:
                            m_uiCount = 0;
                            m_bPatternOn = false;
                            m_bSpawnStart = false;

                            m_eSpawnPattern = SPAWN_PATTERN::END;
                            break;

                        default:
                            break;
                        }


                    }

                }

                break;
                }
            }
            break;

            case CAMP_STATE::BLUE:
            {
                switch (m_eSpawnPattern)
                {
                case SPAWN_PATTERN::PATTERN1:
                {
                    if (!m_bSpawnStart) {
                        m_uiSpawnStart = clock();
                        m_bSpawnStart = true;
                    }
                    else {
                        m_uiSpawnEnd = clock();
                        m_uiSpawnInterval = (m_uiSpawnEnd - m_uiSpawnStart) / CLOCKS_PER_SEC;
                        switch (m_uiCount)
                        {
                        case 0:
                        case 1:
                        case 2:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {
     
                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();
                                CGameObject* pObject = SpawnObject( vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 20.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE,m_eCampState);
                                CreateObject(pObject, L"Blue");

    

                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 3:
                        case 4:
                        case 5:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {
  
                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();
                                CGameObject* pObject = SpawnObject( vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 25.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE,  m_eCampState);
                                CreateObject(pObject, L"Blue");

 
                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 6:
                            m_uiCount = 0;
                            m_bPatternOn = false;
                            m_bSpawnStart = false;

                            m_eSpawnPattern = SPAWN_PATTERN::END;
                            break;

                        default:
                            break;
                        }


                    }
                }

                break;
                case SPAWN_PATTERN::PATTERN2:
                {
                    if (!m_bSpawnStart) {
                        m_uiSpawnStart = clock();
                        m_bSpawnStart = true;
                    }
                    else {
                        m_uiSpawnEnd = clock();
                        m_uiSpawnInterval = (m_uiSpawnEnd - m_uiSpawnStart) / CLOCKS_PER_SEC;
                        switch (m_uiCount)
                        {
                        case 0:
                        case 1:
                        case 2:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {

                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject(vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 20.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE, m_eCampState);
                                CreateObject(pObject, L"Blue");

       
                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 3:
                        case 4:
                        case 5:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {

                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject( vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 25.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE,  m_eCampState);
                                CreateObject(pObject, L"Blue");

                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 6:
                            m_uiCount = 0;
                            m_bPatternOn = false;
                            m_bSpawnStart = false;

                            m_eSpawnPattern = SPAWN_PATTERN::PATTERN3;
                            break;

                        default:
                            break;
                        }
                    }
                }
                break;
                case SPAWN_PATTERN::PATTERN3:
                {
                    if (!m_bSpawnStart) {
                        m_uiSpawnStart = clock();
                        m_bSpawnStart = true;
                    }
                    else {
                        m_uiSpawnEnd = clock();
                        m_uiSpawnInterval = (m_uiSpawnEnd - m_uiSpawnStart) / CLOCKS_PER_SEC;
                        switch (m_uiCount)
                        {
                        case 0:
                        case 1:
                        case 2:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {
        
                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject(vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 20.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE, m_eCampState);
                                CreateObject(pObject, L"Blue");

                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 3: {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {
          
                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject( vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 25.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::CANON,  m_eCampState);
                                CreateObject(pObject, L"Blue");

  
                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }

                        }
                              break;
                        case 4:
                        case 5:
                        case 6:
                        {
                            if (m_uiSpawnInterval == SPAWNINTEVAL) {
     
                                Vec3 vPos = GetObj()->Transform()->GetLocalPos();

                                CGameObject* pObject = SpawnObject( vPos, Vec3(0.3f, 0.3f, 0.3f), Vec3(0.f, 25.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE,  m_eCampState);
                                CreateObject(pObject, L"Blue");

        
                                m_uiCount += 1;
                                m_uiSpawnStart = m_uiSpawnEnd;
                            }
                        }
                        break;
                        case 7:
                            m_uiCount = 0;
                            m_bPatternOn = false;
                            m_bSpawnStart = false;

                            m_eSpawnPattern = SPAWN_PATTERN::END;
                            break;

                        default:
                            break;
                        }


                    }

                }

                break;
                }

            }
            break;


            }


        }



    }

}


CSpawnScript::CSpawnScript() :CScript((UINT)SCRIPT_TYPE::SPAWNSCRIPT),m_bClockStart(false),m_eSpawnPattern(SPAWN_PATTERN::PATTERN1)
,m_bSpawnStart(false),m_uiCount(0),m_bPatternOn(false)
{
}

CSpawnScript::~CSpawnScript()
{
}


