#include "pch.h"
#include "SpawnScript.h"
#include <chrono>
#include "Ptr.h"
#include "MeshData.h"
#include "ResMgr.h"
#include "MinionScript.h"
#include "Collider3D.h"
#include "MeshRender.h"
#include "Animator3D.h"
CGameObject* CSpawnScript::SpawnObject(const wstring& _strKey, Vec3 _vLocalPos, Vec3 _vLocalScale, Vec3 _vOffsetScale, CAnimation* _pAnimation)
{
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(_strKey, _strKey);
	CGameObject* pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CMinionScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(_vOffsetScale);
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(_vLocalPos);
	pObject->Transform()->SetLocalScale(_vLocalScale);
	pObject->MeshRender()->SetDynamicShadow(true);
	pObject->Animator3D()->SetAnimation(_pAnimation);
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
		m_uiPatternInterval =(m_uiPatternEnd - m_uiPatternStart) / CLOCKS_PER_SEC;

		if (m_uiPatternInterval == PATTERNINTEVAL) {
			if(!m_bPatternOn)
				m_uiPatternStart = m_uiPatternEnd;
			m_bPatternOn = true;
		}
		if (m_bPatternOn) {
			CAnimation* pNewAnimation = new CAnimation;
			switch (m_eCampState)
			{
			case CAMP_STATE::BLUE:
			{
				switch (m_eSpawnPattern)
				{
				case SPAWN_PATTERN::PATTERN1: {
					switch (m_eSpawnCount)
					{
					case SPAWN_COUNT::SIX:
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
									pNewAnimation->InsertAnimation(L"IDLE", 0, 34, true, false);
									pNewAnimation->InsertAnimation(L"WALK", 35, 49, false, false);
									pNewAnimation->InsertAnimation(L"ATTACK", 50, 74, false, false);
									pNewAnimation->InsertAnimation(L"DIE", 75, 124, false, false);
									Vec3 vPos = GetObj()->Transform()->GetLocalPos();
									vPos.z += m_uiCount * 25;
									CGameObject* pObject = SpawnObject(L"MeshData\\sword_min.mdat", vPos, Vec3(0.25f, 0.25f, 0.25f), Vec3(10.f, 40.f, 10.f), pNewAnimation);
									CreateObject(pObject, L"Minion");
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
									pNewAnimation->InsertAnimation(L"IDLE", 0, 34, true, false);
									pNewAnimation->InsertAnimation(L"WALK", 36, 59, false, false);
									pNewAnimation->InsertAnimation(L"ATTACK", 60, 84, false, false);
									pNewAnimation->InsertAnimation(L"DIE", 110, 159, false, false);
									Vec3 vPos = GetObj()->Transform()->GetLocalPos();
									vPos.z += m_uiCount * 25;
									CGameObject* pObject = SpawnObject(L"MeshData\\wizard_min.mdat", vPos, Vec3(0.25f, 0.25f, 0.25f), Vec3(10.f, 40.f, 10.f), pNewAnimation);
									CreateObject(pObject, L"Minion");
									m_uiCount += 1;
									m_uiSpawnStart = m_uiSpawnEnd;
								}
							}
							break;
							case 6:
								m_uiCount = 0;
								m_bPatternOn = false;
								m_bSpawnStart = false;
								break;

							default:
								break;
							}


						}
					}
					break;
					default:
						break;
					}




				}


											break;
				default:
					break;
				}
			}



			break;
			default:
				break;
			}
		}

		
	}
	
	

}

CSpawnScript::CSpawnScript() :CScript((UINT)SCRIPT_TYPE::SPAWNSCRIPT),m_bClockStart(false),m_eSpawnCount(SPAWN_COUNT::SIX), m_eSpawnPattern(SPAWN_PATTERN::PATTERN1)
,m_bSpawnStart(false),m_uiCount(0),m_bPatternOn(false)
{
}

CSpawnScript::~CSpawnScript()
{
}
