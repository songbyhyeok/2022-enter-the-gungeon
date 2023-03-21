#include "SoundController.h"

void SoundController::init()
{
	_system = make_unique<System*>();	
	assert(checkResult(System_Create(&getSystem())) == FMOD_OK);
	assert(checkResult(getSystem()->init(MaxBuffer, FMOD_INIT_NORMAL, nullptr)) == FMOD_OK);

	const uint16 musicTypeValue{ soundTypeToInt(SOUND_TYPE::MUSIC) };
	const uint16 acousticTypeValue{ soundTypeToInt(SOUND_TYPE::ACOUSTIC) };
	const uint16 uiTypeValue{ soundTypeToInt(SOUND_TYPE::UI) };
	_volumes[musicTypeValue] = 0.5;
	_volumes[acousticTypeValue] = 0.7;
	_volumes[uiTypeValue] = 0.4;

	_soundPtrsVec2d.resize(SoundTypeCount);
	_soundPtrsVec2d[musicTypeValue].resize(MusicListCount);
	_soundPtrsVec2d[acousticTypeValue].resize(AcousticListCount);
	_soundPtrsVec2d[uiTypeValue].resize(UIListCount);

	_channelPtrsVec2d.resize(SoundTypeCount);
	_channelPtrsVec2d[musicTypeValue].resize(MusicListCount);
	_channelPtrsVec2d[acousticTypeValue].resize(AcousticListCount);
	_channelPtrsVec2d[uiTypeValue].resize(UIListCount);
	
	addSound({	"Sounds/Music/Opening.mp3", SOUND_TYPE::MUSIC, musicListToInt(MUSIC_LIST::OPENING) }, true);
	addSound({	"Sounds/Music/Loading.mp3", SOUND_TYPE::MUSIC, musicListToInt(MUSIC_LIST::LOADING) }, true);
	addSound({	"Sounds/Music/Title.mp3", SOUND_TYPE::MUSIC, musicListToInt(MUSIC_LIST::TITLE) }, true);
	addSound({	"Sounds/Music/Maptool.mp3", SOUND_TYPE::MUSIC, musicListToInt(MUSIC_LIST::MAPTOOL) }, true);
	addSound({	"Sounds/Music/Game.mp3", SOUND_TYPE::MUSIC, musicListToInt(MUSIC_LIST::GAME) }, true);
	addSound({	"Sounds/Music/Boss.mp3", SOUND_TYPE::MUSIC, musicListToInt(MUSIC_LIST::BOSS) }, true);

	addSound({ "Sounds/Acoustic/Player/Dodge.mp3", SOUND_TYPE::ACOUSTIC, acousticListToInt(ACOUSTIC_LIST::P_DODGE) }, false);
	addSound({ "Sounds/Acoustic/Player/Walking.mp3", SOUND_TYPE::ACOUSTIC, acousticListToInt(ACOUSTIC_LIST::P_WALKING) }, false);

	addSound({ "Sounds/Acoustic/Weapons/BulletSplashing.mp3", SOUND_TYPE::ACOUSTIC, acousticListToInt(ACOUSTIC_LIST::BULLET_SPLASHING) }, false);
	addSound({ "Sounds/Acoustic/Weapons/PistolFiring.mp3", SOUND_TYPE::ACOUSTIC, acousticListToInt(ACOUSTIC_LIST::PISTOL_FIRING) }, false);
	addSound({ "Sounds/Acoustic/Weapons/PistolReloading.mp3", SOUND_TYPE::ACOUSTIC, acousticListToInt(ACOUSTIC_LIST::PISTOL_RELOADING) }, false);
	addSound({ "Sounds/Acoustic/Weapons/gun1Firing.mp3", SOUND_TYPE::ACOUSTIC, acousticListToInt(ACOUSTIC_LIST::GUN1_FIRING) }, false);
	addSound({ "Sounds/Acoustic/Weapons/gun1Reloading.mp3", SOUND_TYPE::ACOUSTIC, acousticListToInt(ACOUSTIC_LIST::GUN1_RELOADING) }, false);

	addSound({ "Sounds/Acoustic/Enemys/Bullet/Walking.mp3", SOUND_TYPE::ACOUSTIC, acousticListToInt(ACOUSTIC_LIST::E_B_WALKING) }, false);
	addSound({ "Sounds/Acoustic/Enemys/Bullet/Die.mp3", SOUND_TYPE::ACOUSTIC, acousticListToInt(ACOUSTIC_LIST::E_B_DIE) }, false);

	addSound({ "Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI,uiListToInt(UI_LIST::COLLISION_BUTTON) }, false);
	addSound({ "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI,uiListToInt(UI_LIST::BUTTON_CLICK) }, false);
	addSound({ "Sounds/UI/GameSceneClick.mp3", SOUND_TYPE::UI,uiListToInt(UI_LIST::GAMESCENE_CLICK) }, false);
}

void SoundController::update()
{
	assert(checkResult(getSystem()->update()) == FMOD_OK);
}

void SoundController::release()
{
	for (int i = 0; i < SoundTypeCount; i++)
	{
		for (auto& soundPtr : _soundPtrsVec2d[i])
		{
			soundPtr->release();
		}

		_soundPtrsVec2d[i].clear();
	}	
	_soundPtrsVec2d.clear();
	
	getSystem()->close();
	getSystem()->release();
}

void SoundController::play(const SOUND_FILE& soundFile)
{
	if (soundFile.source == "" || soundFile.soundType == SOUND_TYPE::END || soundFile.typeValue == 99)
		return;

	const uint16& soundTypeValue = soundTypeToInt(soundFile.soundType);
	if (!isPlaying(soundFile, soundTypeValue))
	{
		getSystem()->playSound(_soundPtrsVec2d[soundTypeValue][soundFile.typeValue], 0, false, &_channelPtrsVec2d[soundTypeValue][soundFile.typeValue]);
		_channelPtrsVec2d[soundTypeValue][soundFile.typeValue]->setVolume(_volumes[soundTypeValue]);
	}
}

void SoundController::stop(const SOUND_FILE& soundFile) const
{
	if (soundFile.source == "" || soundFile.soundType == SOUND_TYPE::END || soundFile.typeValue == 99)
		return;

	const uint16& soundTypeValue = soundTypeToInt(soundFile.soundType);
	if (isPlaying(soundFile, soundTypeValue))
	{		
		_channelPtrsVec2d[soundTypeValue][soundFile.typeValue]->stop();
	}
}

void SoundController::pause(const SOUND_FILE& soundFile) const
{
	const uint16& soundTypeValue = soundTypeToInt(soundFile.soundType);
	if (isPlaying(soundFile, soundTypeValue))
	{
		_channelPtrsVec2d[soundTypeValue][soundFile.typeValue]->setPaused(true);
	}
}

void SoundController::resume(const SOUND_FILE& soundFile) const
{
	const uint16& soundTypeValue = soundTypeToInt(soundFile.soundType);
	if (isPlaying(soundFile, soundTypeValue))
	{
		_channelPtrsVec2d[soundTypeValue][soundFile.typeValue]->setPaused(false);
	}
}

void SoundController::controlVolumes(const SOUND_FILE& soundFile, float amount)
{
	if (soundFile.soundType == SOUND_TYPE::END)
		return;

	const uint16 soundTypeValue = soundTypeToInt(soundFile.soundType);
	_volumes[soundTypeValue] = amount;
	for (auto& channelPtr : _channelPtrsVec2d[soundTypeValue])
	{
		if (channelPtr != nullptr)
			channelPtr->setVolume(amount);
	}
}

SOUND_FILE SoundController::generateNameToSoundFile(const char* name)
{
	SOUND_FILE soundFile { "", SOUND_TYPE::END};
	if (name == "Music Volume")
	{
		soundFile.soundType = SOUND_TYPE::MUSIC;
	}
	else if (name == "SFX Volume")
	{
		soundFile.soundType = SOUND_TYPE::ACOUSTIC;
	}
	else if (name == "UI Volume")
	{
		soundFile.soundType = SOUND_TYPE::UI;
	}

	return soundFile;
}

bool SoundController::isPlaying(const SOUND_FILE& soundFile, const uint16 soundTypeValue) const
{
	bool isPlaying = false;
	_channelPtrsVec2d[soundTypeValue][soundFile.typeValue]->isPlaying(&isPlaying);
	return isPlaying;
}

Sound* SoundController::generateSoundPtr(SOUND_FILE soundFile, bool isLoop)
{
	Sound* soundPtr = nullptr;
	switch (isLoop)
	{
	case true:
		getSystem()->createStream(soundFile.source.c_str(), FMOD_LOOP_NORMAL, 0, &soundPtr);
		break;
	case false:
		getSystem()->createSound(soundFile.source.c_str(), FMOD_DEFAULT, 0, &soundPtr);
		break;
	}

	return soundPtr;
}
