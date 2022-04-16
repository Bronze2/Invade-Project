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

	vector<float>				m_vecClipUpdateTime;
	int							m_iFrameCount; // 30
	double						m_dCurTime;
	int							m_iCurClip; // ≈¨∏≥ ¿Œµ¶Ω∫
	int m_iFrameIdx;
	int m_iNextFrameIdx;
	float m_fRatio;
	Ptr<CMaterial> m_pBoneMtrl;
	CStructuredBuffer* m_pBoneFinalMat;

	bool m_bFinalMatUpdate;
	double m_dStartFrameTime;

	CAnimation* m_pAnimation;

public:

	void SetAnimation(CAnimation* _pAni) { m_pAnimation = _pAni; }
	CAnimation* GetAnimation() { return m_pAnimation; }

	bool m_bTestCheck = false;
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
	void SetClipIndex(const int& _iCurClip) { m_iCurClip = _iCurClip; }
	int GetFrameIdx() { return m_iFrameIdx; }
	void SetFrameIdx(const int& _idx) { m_iFrameIdx = _idx; }
	void SetCurTime(const double& _CurTime) { m_vecClipUpdateTime[m_iCurClip] = _CurTime; }
	int GetFrameCount() { return m_iFrameCount; }

	void SetStartFrameTime(const double& _uStarFrameTime) { m_dStartFrameTime = _uStarFrameTime; }

	void UpdateData();
	

	//void SetStartFrame(const UINT& _uStartFrame){m_uStartFrame=_uStartFrame};
	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

	CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMat; }

	UINT GetBoneCount() { return(UINT)m_pVecBones->size(); }
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

