#pragma once
#include "Component.h"
class CScene;
class CSensor :
    public CComponent
{

private://변수
    float m_fRadius;
    CScene* m_pCurScene;
    static UINT g_iSensorID;

    int m_iDetectionCount;

    UINT m_iSensorID;
public://변수


private://함수


public://함수
    void SetRadius(float _fRadius) { m_fRadius = _fRadius; }
    float GetRadius() { return m_fRadius; }

    virtual void Update();
    virtual void FinalUpdate();
    void DetectionObject();
    int GetDetectionCount() { return m_iDetectionCount; }

    void OnDetectionEnter(CGameObject* _pOther);
    void OnDetection(CGameObject* _pOther);
    void OnDetectionExit(CGameObject* _pOther);

    UINT GetSensorID() { return m_iSensorID; }


    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);


    CSensor();
    CSensor(const CSensor& _Other);
    virtual ~CSensor();

    CLONE(CSensor);
};

