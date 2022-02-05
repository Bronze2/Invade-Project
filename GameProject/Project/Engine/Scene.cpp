#include "pch.h"
#include "Scene.h"

#include "Layer.h"
#include "GameObject.h"
#include "Camera.h"
#include "EventMgr.h"
CScene::CScene():m_arrLayer{}
{
    for (UINT i = 0; i < MAX_LAYER; ++i) {
        AddLayer(L"", i);
    }
}

CScene::~CScene()
{
    Safe_Delete_Array(m_arrLayer);
}


void CScene::Awake()
{
    for (UINT i = 0; i < MAX_LAYER; ++i) {
        if (nullptr != m_arrLayer[i])
            m_arrLayer[i]->Awake();
    }
}

void CScene::Start()
{
    for (UINT i = 0; i < MAX_LAYER; ++i) {
        if (nullptr != m_arrLayer[i])
            m_arrLayer[i]->Start();
    }
}

void CScene::Update()
{
    for (UINT i = 0; i < MAX_LAYER; ++i) {
        if (nullptr != m_arrLayer[i])
            m_arrLayer[i]->Update();
    }
}

void CScene::LateUpdate()
{
    for (UINT i = 0; i < MAX_LAYER; ++i) {
        if (nullptr != m_arrLayer[i])
            m_arrLayer[i]->LateUpdate();
    }
}

void CScene::FinalUpdate()
{
    for (UINT i = 0; i < MAX_LAYER; ++i) {
        if (nullptr != m_arrLayer[i])
            m_arrLayer[i]->FinalUpdate();
    }
}

void CScene::SetLayerName(int _iIdx, const wstring& _strName)
{
    assert(-1 < _iIdx && _iIdx <= 31);
    m_arrLayer[_iIdx]->SetName(_strName);
}

void CScene::AddGameObject(const wstring& _strLayerName, CGameObject* _pObject, bool _bMoveAll)
{
    CLayer* pLayer = FindLayer(_strLayerName);
    assert(pLayer);
    pLayer->AddGameObject(_pObject, _bMoveAll);

}

void CScene::AddGameObject(int _iLayerIdx, CGameObject* _pObject, bool _bMoveAll)
{
    assert(-1 < _iLayerIdx && _iLayerIdx <= 31);
    CLayer* pLayer = GetLayer(_iLayerIdx);
    pLayer->AddGameObject(_pObject, _bMoveAll);
}

CLayer* CScene::FindLayer(const wstring& _strLayerName)
{
    for (UINT i = 0; i < MAX_LAYER; ++i) {
        if (nullptr == m_arrLayer[i])
            continue;
        if (_strLayerName == m_arrLayer[i]->GetName())
            return m_arrLayer[i];
    }
    return nullptr;
}

void CScene::AddLayer(const wstring& _strLayerName, int _iIdx)
{
    assert(-1 < _iIdx && _iIdx <= 31 && (nullptr == m_arrLayer[_iIdx]));
    m_arrLayer[_iIdx] = new CLayer;
    m_arrLayer[_iIdx]->SetName(_strLayerName);
    m_arrLayer[_iIdx]->SetLayerIdx(_iIdx);
}

