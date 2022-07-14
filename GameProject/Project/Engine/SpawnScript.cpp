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
CGameObject* CSpawnScript::SpawnObject(const wstring& _strKey, Vec3 _vLocalPos, Vec3 _vLocalScale, Vec3 _vOffsetPos, Vec3 _vOffsetScale, MINION_ATTACK_TYPE _eAttackRange, CAnimation* _pAnimation, CAMP_STATE _eCamp)
{
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(_strKey, _strKey);
	CGameObject* pObject = pMeshData->Instantiate();
	pObject->SetName(L"Minion");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CMinionScript);
	pObject->AddComponent(new CSensor);
	pObject->Sensor()->SetRadius(1500.f);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(_vOffsetScale);
	pObject->Collider3D()->SetOffsetPos(_vOffsetPos);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(_vLocalPos);
	pObject->Transform()->SetLocalScale(_vLocalScale);
	pObject->MeshRender()->SetDynamicShadow(false);
	pObject->Animator3D()->SetAnimation(_pAnimation);
	pObject->Animator3D()->SetAnimClip(_pAnimation->GetAnimClip());
	pObject->GetScript<CMinionScript>()->SetNexus(m_pNexus);
	pObject->GetScript<CMinionScript>()->SetFirstTower(m_pFirstTower);
	pObject->GetScript<CMinionScript>()->SetSecondTower(m_pSecondTower);
	pObject->GetScript<CMinionScript>()->SetAttackType(_eAttackRange);
	pObject->GetScript<CMinionScript>()->SetCamp(_eCamp);
	pObject->GetScript<CMinionScript>()->Init();


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
			CAnimation* pNewAnimation = new CAnimation;
			switch (m_eCampState)
			{
			case CAMP_STATE::RED:
			{
				return;
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 29);
								pNewAnimation->InsertAnimClip(L"WALK", 30, 49);
								pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
								pNewAnimation->InsertAnimClip(L"DIE", 75, 114);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
						
								CGameObject* pObject = SpawnObject(L"MeshData\\sword_min1.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f,10.f,0.f),Vec3(60.f, 100.f, 60.f),MINION_ATTACK_TYPE::MELEE, pNewAnimation, m_eCampState);
							
								
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 29);
								pNewAnimation->InsertAnimClip(L"WALK", 31, 49);
								pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
								pNewAnimation->InsertAnimClip(L"DIE", 100, 149);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
						
								CGameObject* pObject = SpawnObject(L"MeshData\\wizard_min1.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 40.f, 0.f), Vec3(60.f, 100.f, 60.f),MINION_ATTACK_TYPE::RANGE ,pNewAnimation, m_eCampState);
					
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

							m_eSpawnPattern = SPAWN_PATTERN::PATTERN2;
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 29);
								pNewAnimation->InsertAnimClip(L"WALK", 30, 49);
								pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
								pNewAnimation->InsertAnimClip(L"DIE", 75, 114);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
						
								CGameObject* pObject = SpawnObject(L"MeshData\\sword_min1.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 20.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE, pNewAnimation, m_eCampState);
								
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 29);
								pNewAnimation->InsertAnimClip(L"WALK", 31, 49);
								pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
								pNewAnimation->InsertAnimClip(L"DIE", 100, 149);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
						
								CGameObject* pObject = SpawnObject(L"MeshData\\wizard_min1.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 40.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE,pNewAnimation, m_eCampState);
							
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 29);
								pNewAnimation->InsertAnimClip(L"WALK", 30, 49);
								pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
								pNewAnimation->InsertAnimClip(L"DIE", 75, 114);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
							
								CGameObject* pObject = SpawnObject(L"MeshData\\sword_min1.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 20.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE, pNewAnimation, m_eCampState);
								
								CreateObject(pObject, L"Red");
								m_uiCount += 1;
								m_uiSpawnStart = m_uiSpawnEnd;
							}
						}
						break;
						case 3: {
							if (m_uiSpawnInterval == SPAWNINTEVAL) {
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 39);
								pNewAnimation->InsertAnimClip(L"WALK", 41, 74);
								pNewAnimation->InsertAnimClip(L"ATTACK", 75, 99);
								pNewAnimation->InsertAnimClip(L"DIE", 125, 174);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
						
								CGameObject* pObject = SpawnObject(L"MeshData\\Canon_min1.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 60.f, 0.f), Vec3(80.f, 100.f, 80.f), MINION_ATTACK_TYPE::CANON, pNewAnimation, m_eCampState);
							
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
					
							if (m_uiSpawnInterval >= SPAWNINTEVAL) {

						
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 29);

								pNewAnimation->InsertAnimClip(L"WALK", 31, 49);
								pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
								pNewAnimation->InsertAnimClip(L"DIE", 100, 149);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
								if (m_uiCount == 6) {
									int a = 0;
								}
								CGameObject* pObject = SpawnObject(L"MeshData\\wizard_min1.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 20.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE, pNewAnimation, m_eCampState);
							
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 34);
								pNewAnimation->InsertAnimClip(L"WALK", 35, 49);
								pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
								pNewAnimation->InsertAnimClip(L"DIE", 75, 124);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
								CGameObject* pObject = SpawnObject(L"MeshData\\sword_min.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 50.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE, pNewAnimation, m_eCampState);
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 34);
								pNewAnimation->InsertAnimClip(L"WALK", 36, 59);
								pNewAnimation->InsertAnimClip(L"ATTACK", 60, 84);
								pNewAnimation->InsertAnimClip(L"DIE", 110, 159);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
								CGameObject* pObject = SpawnObject(L"MeshData\\wizard_min.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 50.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE, pNewAnimation, m_eCampState);
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

							m_eSpawnPattern = SPAWN_PATTERN::PATTERN2;
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 34);
								pNewAnimation->InsertAnimClip(L"WALK", 35, 49);
								pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
								pNewAnimation->InsertAnimClip(L"DIE", 75, 124);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
		
								CGameObject* pObject = SpawnObject(L"MeshData\\sword_min.mdat", vPos, Vec3(1.f, 1.f, 1.f),Vec3(0.f, 50.f,0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE, pNewAnimation, m_eCampState);
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 34);
								pNewAnimation->InsertAnimClip(L"WALK", 36, 59);
								pNewAnimation->InsertAnimClip(L"ATTACK", 60, 84);
								pNewAnimation->InsertAnimClip(L"DIE", 110, 159);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
					
								CGameObject* pObject = SpawnObject(L"MeshData\\wizard_min.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 50.f, 0.f), Vec3(80.f, 100.f, 80.f), MINION_ATTACK_TYPE::RANGE, pNewAnimation, m_eCampState);
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 34);
								pNewAnimation->InsertAnimClip(L"WALK", 35, 49);
								pNewAnimation->InsertAnimClip(L"ATTACK", 50, 74);
								pNewAnimation->InsertAnimClip(L"DIE", 75, 124);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
							
								CGameObject* pObject = SpawnObject(L"MeshData\\sword_min.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 50.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::MELEE, pNewAnimation, m_eCampState);
								CreateObject(pObject, L"Blue");
								m_uiCount += 1;
								m_uiSpawnStart = m_uiSpawnEnd;
							}
						}
						break;
						case 3: {
							if (m_uiSpawnInterval == SPAWNINTEVAL) {
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 30);
								pNewAnimation->InsertAnimClip(L"WALK", 31, 54);
								pNewAnimation->InsertAnimClip(L"ATTACK", 80, 104);
								pNewAnimation->InsertAnimClip(L"DIE", 105, 154);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
							
								CGameObject* pObject = SpawnObject(L"MeshData\\Canon_min.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 50.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::CANON, pNewAnimation, m_eCampState);
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
								pNewAnimation->InsertAnimClip(L"IDLE", 0, 34);
								pNewAnimation->InsertAnimClip(L"WALK", 36, 59);
								pNewAnimation->InsertAnimClip(L"ATTACK", 60, 84);
								pNewAnimation->InsertAnimClip(L"DIE", 110, 159);
								Vec3 vPos = GetObj()->Transform()->GetLocalPos();
							
								CGameObject* pObject = SpawnObject(L"MeshData\\wizard_min.mdat", vPos, Vec3(1.f, 1.f, 1.f), Vec3(0.f, 50.f, 0.f), Vec3(60.f, 100.f, 60.f), MINION_ATTACK_TYPE::RANGE,pNewAnimation, m_eCampState);
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

CSpawnScript::CSpawnScript() :CScript((UINT)SCRIPT_TYPE::SPAWNSCRIPT),m_bClockStart(false),m_eSpawnPattern(SPAWN_PATTERN::PATTERN3)
,m_bSpawnStart(false),m_uiCount(0),m_bPatternOn(false)
{
}

CSpawnScript::~CSpawnScript()
{
}
