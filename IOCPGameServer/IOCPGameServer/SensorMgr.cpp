#include "pch.h"
#include "SensorMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Sensor.h"
#include "Transform.h"

CSensorMgr::CSensorMgr():m_LayerCheck{}{}
CSensorMgr::~CSensorMgr() {

}


void CSensorMgr::Init()
{
}

void CSensorMgr::Update(int index)
{
    CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene(index);

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (int j = 0; j < MAX_LAYER; ++j)
		{
			if (m_LayerCheck[i] & (1 << j))
			{
				// i <= j
				DetectionLayer(pCurScene->GetLayer(i), pCurScene->GetLayer(j));
			
			}
		}
	}
}

void CSensorMgr::CheckSensorLayer(int index , const wstring& _strLayerName1, const wstring& _strLayerName2)
{
	CLayer* pLayer1 = CSceneMgr::GetInst()->GetCurScene(index)->FindLayer(_strLayerName1);
	CLayer* pLayer2 = CSceneMgr::GetInst()->GetCurScene(index)->FindLayer(_strLayerName2);

	assert(nullptr != pLayer1 && nullptr != pLayer2);

	int iIdx1 = pLayer1->GetLayerIdx();
	int iIdx2 = pLayer2->GetLayerIdx();

	int iMinIdx = min(iIdx1, iIdx2);
	int iMaxIdx = max(iIdx1, iIdx2);

	m_LayerCheck[iMinIdx] |= (1 << iMaxIdx);
}

void CSensorMgr::CheckSensorLayer(int index, int _strLayer1, int _strLayer2)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene(index);

	assert(pScene->GetLayer(_strLayer1) && pScene->GetLayer(_strLayer2));

	int iMinIdx = min(_strLayer1, _strLayer2);
	int iMaxIdx = max(_strLayer1, _strLayer2);

	m_LayerCheck[iMinIdx] |= (1 << iMaxIdx);
}

void CSensorMgr::DetectionLayer(const CLayer* _pLayer1, const CLayer* _pLayer2)
{
	const vector<CGameObject*>& vecObj1 = _pLayer1->GetObjects();
	const vector<CGameObject*>& vecObj2 = _pLayer2->GetObjects();
	map<DWORD_PTR, bool>::iterator iter;
	bool IsDead = false;
	for (size_t i = 0; i < vecObj1.size(); ++i) {
		CSensor* pSensor1 = vecObj1[i]->Sensor();
		if (nullptr == pSensor1)
			continue;
		size_t j = 0;
		if (_pLayer1 == _pLayer2) // 동일한 레이어 간의 충돌을 검사하는 경우
			j = i + 1;
		for (; j < vecObj2.size(); ++j) {
			CSensor* pSensor2 = vecObj2[j]->Sensor();
			if (nullptr == pSensor2)
				continue;


			tSensorID id;
			id.iSensorID1 = pSensor1->GetSensorID();
			id.iSensorID2 = pSensor2->GetSensorID();
			iter = m_mapSensor.find(id.ID);
			bool IsDead = false;
			if (pSensor1->GetObj()->IsFallDown() || pSensor2->GetObj()->IsFallDown())
				IsDead = true;
			if (IsDetection(pSensor1, pSensor2)) {
				if (m_mapSensor.end() != iter && iter->second == true) {
					if (IsDead) {
						pSensor1->OnDetectionExit(pSensor2->GetObj());
						pSensor2->OnDetectionExit(pSensor1->GetObj());
						iter->second = false;
					}
					else {
						pSensor1->OnDetection(pSensor2->GetObj());
						pSensor2->OnDetection(pSensor1->GetObj());
					}
				}
				else {
					if (IsDead)
						continue;
					pSensor1->OnDetectionEnter(pSensor2->GetObj());
					pSensor2->OnDetectionEnter(pSensor1->GetObj());
					if (m_mapSensor.end() == iter) {
						m_mapSensor.insert(make_pair(id.ID, true));
					}
					else {
						iter->second = true;
					}
				}
			}
			else {
				if (m_mapSensor.end() != iter && true == iter->second)
				{
					pSensor1->OnDetectionExit(pSensor2->GetObj());
					pSensor2->OnDetectionExit(pSensor1->GetObj());
					iter->second = false;
				}
			}

		}


	}

}

bool CSensorMgr::IsDetection(CSensor* _pObject1, CSensor* _pObject2)
{

	return Detection(_pObject1, _pObject2);

}

bool CSensorMgr::Detection(CSensor* _pSensor, CSensor* _pSensor2)
{
	CGameObject* pObject1 = _pSensor->GetObj();
	CGameObject* pObject2 = _pSensor2->GetObj();
	Vec3 vPos1 = pObject1->Transform()->GetWorldPos();
	Vec3 vPos2 = pObject2->Transform()->GetWorldPos();
	float Radius = sqrt(pow(vPos1.x - vPos2.x, 2) + pow(vPos1.z - vPos2.z, 2));
	if (pObject1->Sensor()->GetRadius() == 0.f) {
		if (Radius < _pSensor2->GetRadius())
			return true;
	}
	else {

		if (Radius < _pSensor->GetRadius())
			return true;

	}

	return false;
}
