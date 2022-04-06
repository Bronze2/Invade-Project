#include "pch.h"
#include "Sensor.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
#include "Script.h"
UINT CSensor::g_iSensorID = 0;

void CSensor::Update()
{

}

void CSensor::FinalUpdate()
{
}

void CSensor::DetectionObject()
{

}

void CSensor::OnDetectionEnter(CGameObject* _pOther)
{
	m_iDetectionCount += 1;
	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i) {
		vecScripts[i]->OnDetectionEnter(_pOther);
	}
}

void CSensor::OnDetection(CGameObject* _pOther)
{
	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i) {
		vecScripts[i]->OnDetection(_pOther);
	}
}

void CSensor::OnDetectionExit(CGameObject* _pOther)
{
	m_iDetectionCount -= 1;
	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
		vecScripts[i]->OnDetectionExit(_pOther);
}

void CSensor::SaveToScene(FILE* _pFile)
{
}

void CSensor::LoadFromScene(FILE* _pFile)
{
}

CSensor::CSensor():CComponent(COMPONENT_TYPE::SENSOR),m_iSensorID(g_iSensorID++),m_iDetectionCount(0),m_fRadius(0)
{
}

CSensor::CSensor(const CSensor& _Other):CComponent(COMPONENT_TYPE::SENSOR),m_iSensorID(g_iSensorID),m_iDetectionCount(0), m_fRadius(_Other.m_fRadius)
{
}

CSensor::~CSensor()
{
}
