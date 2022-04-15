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
	int							m_iCurClip; // 클립 인덱스
	int m_iFrameIdx;
	int m_iNextFrameIdx;
	float m_fRatio;
	Ptr<CMaterial> m_pBoneMtrl;
	CStructuredBuffer* m_pBoneFinalMat;
	bool m_bFinalMatUpdate;
	double m_dStartFrameTime;

	CAnimation* m_pAnimation;
	float m_fAnimationSpeed; //크면 클수록 느려짐


public:

	void SetAnimation(CAnimation* _pAni) { m_pAnimation = _pAni; }
	CAnimation* GetAnimation() { return m_pAnimation; }

	bool m_bTestCheck = false;
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; }
	const vector<tMTBone>* GetBones() { return m_pVecBones; }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
	void SetClipIndex(const int& _iCurClip) { m_iCurClip = _iCurClip; }
	int GetFrameIdx() { return m_iFrameIdx; }
	void SetFrmaeIdx(const int& _idx) { m_iFrameIdx = _idx; }
	void SetCurTime(const double& _CurTime) { m_vecClipUpdateTime[m_iCurClip] = _CurTime; }
	int GetFrameCount() { return m_iFrameCount; }
	void SetAnimationSpeed(const  float& _fSpeed) { m_fAnimationSpeed = _fSpeed; }
	void SetStartFrameTime(const double& _uStarFrameTime) { m_dStartFrameTime = _uStarFrameTime; }


	void UpdateData();
	void UpdateData_Inst(CStructuredBuffer* _pBoneBuffer, UINT _iRow);

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

