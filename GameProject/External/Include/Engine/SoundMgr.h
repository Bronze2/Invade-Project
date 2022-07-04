#pragma once
#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>
#include <FMOD/common.h>


#ifdef _DEBUG
#pragma comment(lib, "FMOD/x64/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/x64/fmod64_vc.lib")
#endif
#include "Resource.h"


//class CSoundMgr
//{
//private:
//	FMOD::System* m_pSystem;
//	map<const UINT&, FMOD::Sound*> m_MapSound;
//	vector<FMOD::Channel*>	m_arrChannel;
//
//
//public:
//	void SetListener(const Matrix& _ViewMatrix, const Vec3& _PlayerPos, const Vec3& PlayerlastPos);
//	FMOD::System* GetSystem() { return m_pSystem; }
//	void Update();
//
//	bool IsFindSound(const UINT& _Sound, const UINT& _eFirst);
//	SINGLE(CSoundMgr);
//
//	void PlayBGM(const eSOUND& _eSound, float _Volume);
//	void PlaySoundLoop(const eSOUND& _eSound, float _Volume,const UINT& _Channel);
//	void PlaySoundOnce(const eSOUND& _eSound, float _Volume, const UINT& _Channel);
//	void Load3D(const eSOUND& _eSound, const wstring& _Path);
//	void Load2D(const eSOUND& _eSound, const wstring& _Path);
//	void StopAll();
//	void StopSound(const eSOUND& _eID);
//};
//
//