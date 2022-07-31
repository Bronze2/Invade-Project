#include "pch.h"
#include "Sound.h"

#include "ResMgr.h"


FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2)
{
	FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
	CSound* pSound = nullptr;

	switch (controltype)
	{
	case FMOD_CHANNELCONTROL_CALLBACK_END:
	{
		cppchannel->getUserData((void**)&pSound);
		pSound->RemoveChannel(cppchannel);
	}
	break;
	}

	return FMOD_OK;
}


FMOD::System* CSound::g_pFMOD = nullptr;


CSound::CSound()
	: CResource(RES_TYPE::SOUND)
	, m_pSound(nullptr)
{
}

CSound::~CSound()
{
	if (nullptr != m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}
}

void CSound::Play(int _iRoopCount, float _fVolume, bool _bOverlap)
{
	if (_iRoopCount <= -1)
	{
		assert(nullptr);
	}

	// �ߺ� ��� ��� ���� ���

	if (!_bOverlap && !m_listChannel.empty())
	{
		return;
	}

	FMOD::Channel* pChannel = nullptr;
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);

	pChannel->setCallback(CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	//	pChannel->setLoopCount(_iRoopCount);
	pChannel->setVolume(_fVolume);
	if (m_listChannel.empty())
		m_listChannel.push_back(pChannel);
	else
	{

	}

}

void CSound::PlaySoundOnce(const float& _fVolume)
{
	FMOD::Channel* pChannel = nullptr;
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);
	pChannel->setCallback(CHANNEL_CALLBACK);
	pChannel->setUserData(this);
	pChannel->setVolume(_fVolume);
	if (m_listChannel.empty())
		m_listChannel.push_back(pChannel);
}

void CSound::PlaySound3D(const Vec3& _Pos, const float& _fVolume)
{
	FMOD::Channel* pChannel = nullptr;
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);
	pChannel->setCallback(CHANNEL_CALLBACK);
	pChannel->setUserData(this);
	FMOD_VECTOR Pos = VecToFMOD(_Pos);
	FMOD_VECTOR vel = { 0.f,0.f,0.f };
	pChannel->set3DAttributes(&Pos, &vel);
	pChannel->setVolume(_fVolume);
	if (m_listChannel.empty())
		m_listChannel.push_back(pChannel);
}

void CSound::Stop()
{
	list<FMOD::Channel*>::iterator iter;

	while (!m_listChannel.empty())
	{
		iter = m_listChannel.begin();
		(*iter)->stop();
	}
}

void CSound::RemoveChannel(FMOD::Channel* _pTargetChannel)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();
	for (; iter != m_listChannel.end(); ++iter)
	{
		if (*iter == _pTargetChannel)
		{
			m_listChannel.erase(iter);
			return;
		}
	}
}

void CSound::Load(const wstring& _strFilePath)
{
	string path(_strFilePath.begin(), _strFilePath.end());

	if (FMOD_OK != g_pFMOD->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_pSound))
	{
		assert(nullptr);
	}
}

void CSound::Load3D(const wstring& _strFilePath)
{
	string path(_strFilePath.begin(), _strFilePath.end());

	if (FMOD_OK != g_pFMOD->createSound(path.c_str(), FMOD_3D, nullptr, &m_pSound))
	{
		assert(nullptr);
	}

	m_pSound->set3DMinMaxDistance(0.5f, 5000.f);
}

