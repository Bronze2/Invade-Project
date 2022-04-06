#pragma once
class CLayer;
union tSensorID {
	struct {
		UINT iSensorID1;
		UINT iSensorID2;
	};
	DWORD_PTR ID;
};
class CSensor;
class CSensorMgr
{
	SINGLE(CSensorMgr);
private:
	UINT m_LayerCheck[MAX_LAYER];
	map<DWORD_PTR, bool>m_mapSensor;
public:
	void Init();
	void Update();
	void CheckSensorLayer(const wstring& _strLayerName1, const wstring& _strLayerName2);
	void CheckSensorLayer(int _strLayer1, int _strLayer2);

private:
	void DetectionLayer(const CLayer* _pLayer1, const CLayer* _pLayer2);
	bool IsDetection(CSensor* _pObject1, CSensor* _pObject2);
	bool Detection(CSensor* _pSensor, CSensor* _pSensor2);
};

