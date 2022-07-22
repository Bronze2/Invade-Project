#pragma once
class CBlendState;
class CDepthStencilState;
class CCamera;
class CLight3D;

class CRenderTarget24;
class CMRT;

#include "Ptr.h"
#include "Texture.h"


class CRenderMgr
{
	SINGLE(CRenderMgr)
private:
	Ptr<CTexture> m_pRenderTargetTex;
	Ptr<CTexture> m_pDepthStencilTex;

	tResolution m_tResolution;
	CMRT* m_arrMRT[(UINT)MRT_TYPE::END];

	ComPtr< ID3D12Resource> m_pTarget[2];


	tLight2DInfo m_tLight2DInfo;
	
	vector<CLight3D*> m_vecLight3D;
	vector<CCamera*> m_vecCam;
	CCamera* m_pCamera;
	HWND m_hWnd;

	bool m_bWindowed;

	UINT m_iRTVHeapSize;
public:
	void Init(HWND _hWnd, const tResolution& _res, bool _bWindow);
	void Render();
	void Render_Tool();
	void Render_ShadowMap();
	void Render_Lights();
	void Merge_Light();
private:


	void UpdateLight2D();
	void UpdateLight3D();

	void CreateMRT();

public:
	CCamera* GetCamera() { return m_pCamera; }
	void SetCamera(CCamera* _pCamera) { m_pCamera = _pCamera; }
	void RegisterLight2D(const tLight2D& _Light2D) {
		if (100 <= m_tLight2DInfo.iCount)
			return;
		m_tLight2DInfo.arrLight2D[m_tLight2DInfo.iCount++] = _Light2D;
	}
	int RegisterLight3D(CLight3D* _pLight3D){
		if (100 <= m_vecLight3D.size()) {
			return -1;
		}
		m_vecLight3D.push_back(_pLight3D);
		return (int)m_vecLight3D.size() - 1;
	}

	CCamera* GetCamera(int _iIdx) { return m_vecCam[_iIdx]; }
	CCamera* GetMainCam();
	void RegisterCamera(CCamera* _pCamera) {
		m_vecCam.push_back(_pCamera);
	}

	void ClearCamera() { m_vecCam.clear(); }

	HWND GethWnd() { return m_hWnd; }

	tResolution GetResolution() { return m_tResolution; }
	UINT GetRTVHeapSize() { return m_iRTVHeapSize; }
	
	
	CMRT* GetMRT(MRT_TYPE _eType) { return m_arrMRT[(UINT)_eType]; }
	friend class CSceneMgr;




};

