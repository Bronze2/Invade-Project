#include "pch.h"
#include "CSoundClass.h"
#include "Sound.h"

CSoundClass::CSoundClass():m_pSystem(nullptr),m_ID(0)
{
}


bool CSoundClass::IsValid()
{
	return (m_pSystem&&m_pSystem->getev);
}

void CSoundClass::Restart()
{
}

void CSoundClass::Stop(bool _bAllowFadeOut)
{
}

void CSoundClass::SetPaused(bool _bPause)
{
}

void CSoundClass::SetVolume(float _fValue)
{
}

void CSoundClass::SetPitch(float _fValue)
{
}

void CSoundClass::SetParameter(const string& _sName, float _fValue)
{
}

bool CSoundClass::GetPaused() const
{
	return false;
}

float CSoundClass::GetVolume() const
{
	return 0.0f;
}

float CSoundClass::GetPitch() const
{
	return 0.0f;
}

float CSoundClass::GetParameter(const string& _sName)
{
	return 0.0f;
}

bool CSoundClass::Is3D() const
{
	return false;
}

void CSoundClass::Set3DAttributes(const Matrix& _WorldTrans)
{
}

CSoundClass::CSoundClass(AudioSystem* _pSystem, unsigned int _Id):m_pSystem(_pSystem),m_ID(_id)
{
}
