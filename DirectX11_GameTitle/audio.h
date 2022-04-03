#pragma once

#define NOMINMAX
#include <xaudio2.h>
#include "gameObject.h"


class CAudio : public CGameObject
{

private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice;
	BYTE*					m_SoundData;

	int						m_Length;
	int						m_PlayLength;

public:

	static void InitMaster();
	static void UninitMaster();

	void Init() {};
	void Uninit();
	void Update() {};
	void Draw() {};

	void Load(const char *FileName);
	void Play(bool Loop = false);
	void SetVolume(float volume);


};

