#pragma once
#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>
#include <FMOD/fmod_common.h>
#include <FMOD/fmod_dsp.h>
#include <FMOD/fmod_dsp_effects.h>
#include <FMOD/fmod_errors.h>
#include <FMOD/fmod_output.h>
#include "CSoundClass.h"
namespace FMOD {
	class System;
	namespace Studio {
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	}
}
namespace {
	FMOD_VECTOR VecToFMOD(const Vec3& _In) {
		FMOD_VECTOR v;
		v.x = _In.x;
		v.y = _In.y;
		v.z = _In.z;
		return v;
	}
}

class CAudioSystem
{
public:
	CAudioSystem();
	~CAudioSystem();

	bool Init();
	void ShutDown();

	void LoadBank(const string& _Name);
	void UnloadBank(const string& _Name);
	void UnLoadAllBanks();
	CSoundClass PlayEvent(const string& _Name);

	void Update(float _fDeltaTime);

	void SetListener(const Matrix& _ViewMatrix);

	float GetBusVolume(const string& _Name)const;
	void SetBusVolume(const string& _Name, float _fVolume);
	void SetBusPaused(const string& _Name, bool _bPaused);

protected:
	friend class CSoundClass;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	static unsigned int sNextID;
	FMOD::Studio::System* m_pSystem;
	FMOD::System* m_LowLevelSystem;
	unordered_map<string, FMOD::Studio::EventDescription*> m_Events;

	unordered_map<string, FMOD::Studio::Bus*> m_Buses;

};

