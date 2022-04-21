#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Animation3D.h"

class CStructuredBuffer;
class CAnimator3D :
	public CComponent
{
private:
	const vector<tMTBone>* m_pVecBones;
	const vector<tMTAnimClip>* m_pVecClip;

	vector<float>					m_vecClipUpdateTime;

	int							m_iFrameCount; // 30
	double						m_dCurTime;
	int							m_iCurClip; // ≈¨∏≥ ¿Œµ¶Ω∫
	int							m_iNextClip;
	double						m_dNextTime;

	int m_iFrameIdx;
	int m_iNextFrameIdx;
	float m_fRatio;
	Ptr<CMaterial> m_pBoneMtrl;
	CStructuredBuffer* m_pBoneFinalMat;

	bool m_bFinalMatUpdate;
	double m_dStartFrameTime;
	double m_dStartNextFrameTime;

	bool m_bBlendAnimation;
	float m_fBlendFrame;
	float m_fBlendMaxFrame;

	CAnimation* m_pAnimation;

public:

	void SetAnimation(CAnimation* _pAni) { m_pAnimation = _pAni; }
	CAnimation* GetAnimation() { return m_pAnimation; }

	bool m_bTestCheck = false;
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);

	void SetCurClipIndex(const UINT& _iCurClip) { m_iCurClip = _iCurClip; }
	void SetNextClipIndex(const UINT& _iNextClip) { m_iNextClip = _iNextClip; }
	int GetFrameIdx() { return m_iFrameIdx; }
	void SetFrameIdx(const int& _idx) { m_iFrameIdx = _idx; }
	void SetNextFrameIdx(const int& _idx) { m_iNextFrameIdx = _idx; }
	int GetNextFrameIdx() { return m_iNextFrameIdx; }

	void SetCurTime(const double& _CurTime) { m_vecClipUpdateTime[m_iCurClip] = _CurTime; }
	void SetCurTime(const UINT& _iClipIdx, const double& _CurTime) { m_vecClipUpdateTime[_iClipIdx] = _CurTime; }
	double GetCurTime() { return m_vecClipUpdateTime[m_iCurClip]; }
	double GetCurTime(const UINT& _iClipIdx) { return m_vecClipUpdateTime[_iClipIdx]; }
	int GetFrameCount() { return m_iFrameCount; }

	void SetStartFrameTime(const double& _uStarFrameTime) { m_dStartFrameTime = _uStarFrameTime; }
	void SetStartNextFrameTime(const double& _uStartFrameTime) { m_dStartNextFrameTime = _uStartFrameTime; }

	void UpdateData();

	//void SetStartFrame(const UINT& _uStartFrame){m_uStartFrame=_uStartFrame};
	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

	void SetBlendState(bool _bTrue) { m_bBlendAnimation = _bTrue; }
	bool GetBlendState() { return m_bBlendAnimation; }
	float GetBlendMaxFrame() { return m_fBlendMaxFrame; }

	CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMat; }

	UINT GetBoneCount() { return(UINT)m_pVecBones->size(); }

	float GetRatio() { return m_fRatio; }

	void UpdateData_Inst(CStructuredBuffer* _pBoneBuffer, UINT _iRow);

private:
	void Check_Mesh(Ptr<CMesh>_pMesh);
public:
	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
	CAnimator3D* Clone() { return new CAnimator3D(*this); }

public:
	CAnimator3D();
	virtual ~CAnimator3D();
};

