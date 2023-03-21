#pragma once

#include "Singleton.h"
#include "inc\fmod.hpp"

using namespace FMOD;

enum class SOUND_TYPE
{
	MUSIC,
	ACOUSTIC,
	UI,
	END
};

enum class MUSIC_LIST
{
	LOADING,
	OPENING,
	TITLE,
	MAPTOOL,
	GAME,
	BOSS,
	END
};

enum class ACOUSTIC_LIST
{
	P_WALKING,
	P_DODGE,
	BULLET_SPLASHING,
	PISTOL_FIRING,
	PISTOL_RELOADING,
	GUN1_FIRING,
	GUN1_RELOADING,
	E_B_WALKING,
	E_B_DIE,
	END
};

enum class UI_LIST
{
	COLLISION_BUTTON,
	BUTTON_CLICK,
	GAMESCENE_CLICK,
	END
};

struct SOUND_FILE
{
	SOUND_FILE() = default;
	SOUND_FILE(string a_source, SOUND_TYPE a_soundType, uint16 a_typeValue = 99) : source(move(a_source)), soundType(move(a_soundType)), typeValue(a_typeValue) {}

	string		source;
	SOUND_TYPE	soundType;
	uint16		typeValue;
};

constexpr uint16												MaxBuffer = 32; // 32개 채널을 사용하지 않으면 도중에 음악이 꺼진다...
constexpr uint16												SoundTypeCount = 3;
constexpr uint16												MusicListCount = 6;
constexpr uint16												AcousticListCount = 9;
constexpr uint16												UIListCount = 3;

class SoundController : public Singleton<SoundController>
{
public:
	void														init();
	void														update();
	void														release();

	void														play(const SOUND_FILE& soundFile);
	void														stop(const SOUND_FILE& soundFile) const;
	void														pause(const SOUND_FILE& soundFile) const;
	void														resume(const SOUND_FILE& soundFile) const;
	
	void														controlVolumes(const SOUND_FILE& soundFile, float amount);

	static uint16												musicListToInt(MUSIC_LIST musicList) { return move(static_cast<uint16>(move(musicList))); }
	static uint16												acousticListToInt(ACOUSTIC_LIST acousticList) { return move(static_cast<uint16>(move(acousticList))); }
	static uint16												uiListToInt(UI_LIST uiList) { return move(static_cast<uint16>(move(uiList))); }
	static SOUND_FILE											generateNameToSoundFile(const char* name);
	
private:
	auto&														getSystem() const { return (*_system.get()); }
	float														getVolume(SOUND_TYPE soundType) const { return soundType != SOUND_TYPE::END ? _volumes[soundTypeToInt(move(soundType))] : 0; }

	bool														isPlaying(const SOUND_FILE& soundFile, const uint16 soundTypeValue) const;

	const FMOD_RESULT&											checkResult(const FMOD_RESULT& fmodResult) const { return fmodResult; }	
	Sound*														generateSoundPtr(SOUND_FILE soundFile, bool isLoop);
	void														addSound(SOUND_FILE soundFile, bool isLoop) { _soundPtrsVec2d[soundTypeToInt(soundFile.soundType)][soundFile.typeValue] = generateSoundPtr(soundFile, isLoop); }

	static uint16												soundTypeToInt(SOUND_TYPE soundType) { return move(static_cast<uint16>(move(soundType))); }

private:
	vector<vector<Sound*>>										_soundPtrsVec2d;
	vector<vector<Channel*>>									_channelPtrsVec2d;
	array<float, SoundTypeCount>								_volumes;
	unique_ptr<System*>											_system;
};

