#include "myAudio.h"

bool KdAudioManager::Init()
{
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_EnvironmentalReverb | DirectX::AudioEngine_ReverbUseFilters;

	m_audioEng = std::make_unique<DirectX::AudioEngine>(eflags);
	m_audioEng->SetReverb(DirectX::Reverb_Default);

	if (m_audioEng) { return true; }
	return false;
}

void KdAudioManager::Relese()
{
	m_audioEng = nullptr;
}

void KdAudioManager::Update()
{
	//サウンドエンジンの更新
	if (m_audioEng != nullptr)
	{
		m_audioEng->Update();
	}
	//再生中ではないインスタンスは終了元としてリストから削除
	for (auto itr = m_playList.begin(); itr != m_playList.end();)
	{
		if (itr->second->IsPlaying())
		{
			itr = m_playList.erase(itr);
			continue;
		}
		++itr;
	}
}

bool KdSoundEffect::Load(const std::string& filename)
{
	if (KD_AUDIO.GetAudioEngine().get() != nullptr)
	{
		try
		{
			//wstringに変換
			std::wstring wFilename = sjis_to_wide(filename);

			m_soundEffects = (std::make_unique<DirectX::SoundEffect>(KD_AUDIO_ENGINE.get(), wFilename.c_str()));

		}
		catch (...)
		{
			assert(0 && "Sound File Load Error");
		}
	}
	else
	{
		return false;
	}
	return true;
}

void KdSoundInstance::Init(const std::shared_ptr<KdSoundEffect>& soundEffect)
{
	if (!soundEffect) { return; }
	DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default;
	m_instance = (soundEffect->CreateInstance(flags));
	m_soundData = soundEffect;
}

void KdSoundInstance::Play(bool loop)
{
	if (!m_instance) { return; }
	m_instance->Play(loop);
	KD_AUDIO.AddPlayList(shared_from_this());
}

bool KdAudioManager::Play(const std::string& rName,bool loop)
{
	if (!m_audioEng) { return false; }
	std::shared_ptr<KdSoundEffect> soundData = ResFac.GetSound(rName);
	if (!soundData) { return false; }
	std::shared_ptr<KdSoundInstance> instance = std::make_shared<KdSoundInstance>();
	if (!instance) { return false; }
	instance->Init(soundData);
	instance->Play(loop);
	return true;
}