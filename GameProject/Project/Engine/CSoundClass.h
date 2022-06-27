#pragma once
#include <string>
#include <math.h>
class AudioSystem;
class CSoundClass
{
private:

public:
	
	
	CSoundClass();
	bool IsValid();
	void Restart();
	void Stop(bool _bAllowFadeOut = true);
	void SetPaused(bool _bPause);
	void SetVolume(float _fValue);
	void SetPitch(float _fValue);
	void SetParameter(const string& _sName, float _fValue);

	bool GetPaused()const;
	float GetVolume()const;
	float GetPitch()const;
	float GetParameter(const string& _sName);
	bool Is3D()const;
	void Set3DAttributes(const Matrix& _WorldTrans);


protected:
	friend class AudioSystem;
	CSoundClass(AudioSystem* _pSystem, unsigned int _Id);

private:
	AudioSystem* m_pSystem;
	unsigned int m_ID;



};

