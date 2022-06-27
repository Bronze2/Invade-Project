#include "pch.h"
#include "AudioSystem.h"
#include <FMOD/fmod_studio.h>
#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod_errors.h>
#include "CSoundClass.h"


CAudioSystem::CAudioSystem():m_pSystem(nullptr), m_LowLevelSystem(nullptr)
{
}

CAudioSystem::~CAudioSystem()
{
}

bool CAudioSystem::Init()
{
    FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR, FMOD_DEBUG_MODE_TTY);
    FMOD_RESULT result;
    result = FMOD::Studio::System::create(&m_pSystem);


    if (result != FMOD_OK)
    {
        assert(nullptr);
    }
    result = m_pSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        assert(nullptr);
    }
    FMOD_3D_ATTRIBUTES listener;
    listener.position=VecToFMOD
    return false;
}

void CAudioSystem::ShutDown()
{

}

void CAudioSystem::LoadBank(const string& _Name)
{
}

void CAudioSystem::UnloadBank(const string& _Name)
{
}

void CAudioSystem::UnLoadAllBanks()
{
}

CSoundClass CAudioSystem::PlayEvent(const string& _Name)
{
    return CSoundClass();
}

void CAudioSystem::Update(float _fDeltaTime)
{
}

void CAudioSystem::SetListener(const Matrix& _ViewMatrix)
{
}

float CAudioSystem::GetBusVolume(const string& _Name) const
{
    return 0.0f;
}

void CAudioSystem::SetBusVolume(const string& _Name, float _fVolume)
{
}

void CAudioSystem::SetBusPaused(const string& _Name, bool _bPaused)
{
}
