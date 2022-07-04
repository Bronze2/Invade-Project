#include "pch.h"
//#include "SoundMgr.h"
//#include "TimeMgr.h"
//
//CSoundMgr::CSoundMgr() {
//
//	FMOD::System_Create(&m_pSystem);
//
//	if (nullptr == m_pSystem)
//	{
//		assert(nullptr);
//	}
//
//	// 32개 채널 생성
//	m_pSystem->init(32, FMOD_DEFAULT, nullptr);
//	m_pSystem->set3DSettings(1.0, 1.0f, 1.0f);
//}
//CSoundMgr::~CSoundMgr() {
//	for (auto& p : m_MapSound) {
//		p.second->release();
//		p.second = nullptr;
//	}
//	m_pSystem->close();
//	m_pSystem->release();
//
//}
//
//void CSoundMgr::SetListener(const Matrix& _ViewMatrix, const Vec3& _PlayerPos, const Vec3& _PlayerlastPos)
//{
//	Matrix invView = _ViewMatrix;
//	invView.Invert();
//	FMOD_3D_ATTRIBUTES listener;
//	listener.position = VecToFMOD(_PlayerPos);
//	Vec3 vZAxis = Vec3(invView.m[2][0], invView.m[2][1], invView.m[2][2]);
//	vZAxis.Normalize();
//	Vec3 vYAxis = Vec3(invView.m[1][0], invView.m[1][1], invView.m[1][2]);
//	vYAxis.Normalize();
//	listener.forward = VecToFMOD(vZAxis);
//	listener.up = VecToFMOD(vYAxis);
//	listener.velocity = { (_PlayerPos.x - _PlayerlastPos.x) * DT,(_PlayerPos.y - _PlayerlastPos.y) * DT,(_PlayerPos.z - _PlayerlastPos.z) * DT };
//	m_pSystem->set3DListenerAttributes(0, &listener.position, &listener.velocity, &listener.forward, &listener.up);
//}
//
//void CSoundMgr::Update()
//{
//	m_pSystem->update();
//}
//
//bool CSoundMgr::IsFindSound(const UINT& _Sound, const UINT& _eFirst)
//{
//	if (_Sound == _eFirst)
//		return true;
//	return false;
//	
//}
//
//void CSoundMgr::PlayBGM(const eSOUND& _eSound, float _Volume)
//{
//	if (m_MapSound.empty())
//		return;
//}
//
//void CSoundMgr::PlaySoundLoop(const eSOUND& _eSound, float _Volume, const UINT& _Channel)
//{
//	if (_Volume < 0.f)_Volume = 0.1f;
//	if (m_MapSound.empty())
//		return;
//
//	const UINT& SoundFind = (UINT)_eSound;
//	auto iter = find_if(m_MapSound.begin(), m_MapSound.end(),
//		[&SoundFind](auto& MyPair) {
//			return SoundFind == MyPair.first;
//		}
//	);
//
//	if (m_MapSound.end() == iter)
//	{
//		MessageBox(0, TEXT("Not Find SoundKey!"), TEXT("System Message"), MB_OK);
//		return;
//	}
//	m_pSystem->playSound(iter->second, nullptr, false, &m_arrChannel[_Channel]);
//	m_arrChannel[_Channel]->setVolume(_Volume);
//	m_arrChannel[_Channel]->setMode(FMOD_LOOP_NORMAL);
//}
//
//
//void CSoundMgr::PlaySoundOnce(const eSOUND& _eSound, float _Volume, const UINT& _Channel)
//{
//	if (_Volume < 0.f)_Volume = 0.1f;
//	if (m_MapSound.empty())
//		return;
//
//	const UINT& SoundFind = (UINT)_eSound;
//	auto iter = find_if(m_MapSound.begin(), m_MapSound.end(),
//		[&SoundFind](auto& MyPair){
//			return SoundFind == MyPair.first;
//		}
//		);
//
//	if (m_MapSound.end() == iter)
//	{
//		MessageBox(0, TEXT("Not Find SoundKey!"), TEXT("System Message"), MB_OK);
//		return;
//	}
//	m_pSystem->playSound(iter->second, nullptr, false, &m_arrChannel[_Channel]);
//	m_arrChannel[_Channel]->setVolume(_Volume);
//	m_arrChannel[_Channel]->setLoopCount(0);
//
//
//}
//
//void CSoundMgr::Load3D(const eSOUND& _eSound, const wstring& _Path)
//{
//	string path(_Path.begin(), _Path.end());
//	FMOD::Sound* pSound = nullptr;
//
//	if (FMOD_OK != m_pSystem->createSound(path.c_str(), FMOD_3D, nullptr, &pSound));
//	{
//		assert(nullptr);
//	}
//
//
//}
//void CSoundMgr::Load2D(const eSOUND& _eSound, const wstring& _Path)
//{
//}
//
//
//void CSoundMgr::StopAll()
//{
//
//	for (int i = 0; i < (UINT)eSOUND::MAX; ++i) {
//		m_arrChannel[i]->stop();
//	}
//}
//
//void CSoundMgr::StopSound(const eSOUND& _eID)
//{
//	m_arrChannel[(UINT)_eID]->stop();
//}

