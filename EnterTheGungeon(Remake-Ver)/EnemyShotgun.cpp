#include "EnemyShotgun.h"

void EnemyShotgun::init(FPOINT scale, const POINT& sp, POINT size, POINT currPos, POINT currIdx)
{
	Monster::init();
	_scale = move(scale);
	_size = move(size);

	setCurrPos(move(currPos));
	setCurrIdx(move(currIdx));
	setHP(4);
	setSpeed(5);
	setDamage(1);
	setDefense(1);
	setCurrAngle(0);

	_canHands = false;	

	_animSets.resize(3);
	const uint16 idleN = toInt(MONSTER_STATE::IDLE);
	_animSets[idleN].image = ImageManager::makeImage("Images/Enemys/Shotgun1/Idle.bmp", sp, _scale, { _size.x * 4, _size.y * 1 }, { 4, 1 }, true, { false, false }, MagentaRGB);
	_animSets[idleN].anim = AnimationManager::makeAnimation(_size, { 4, 1 }, 6.5, 0, 0, false, true);
	_animSets[idleN].frameSpeed = 0.02f;
	_animSets[idleN].frameLength = 2;
	_animSets[idleN].soundFile = { "", SOUND_TYPE::END };

	const uint16 walkingN = toInt(MONSTER_STATE::WALKING);
	_animSets[walkingN].image = ImageManager::makeImage("Images/Enemys/Shotgun1/Walking.bmp", sp, _scale, { _size.x * 6, _size.y * 4 }, { 6, 4 }, true, { false, false }, MagentaRGB);
	_animSets[walkingN].anim = AnimationManager::makeAnimation(_size, { 6, 4 }, 6.5, 0, 0, false, true);
	_animSets[walkingN].frameSpeed = 0.03f;
	_animSets[walkingN].frameLength = 0;
	_animSets[walkingN].soundFile = { "Sounds/Acoustic/Enemys/Bullet/Walking.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::E_B_WALKING) };

	const uint16 deathN = toInt(MONSTER_STATE::DEATH);
	_animSets[deathN].image = ImageManager::makeImage("Images/Enemys/Shotgun1/Death.bmp", sp, _scale, { _size.x * 7, _size.y * 1 }, { 7, 1 }, true, { false, false }, MagentaRGB);
	_animSets[deathN].anim = AnimationManager::makeAnimation(_size, { 7, 1 }, 6.5, 0, 0, false, false);
	_animSets[deathN].frameSpeed = 0.04f;
	_animSets[deathN].frameLength = 0;
	_animSets[deathN].soundFile = { "Sounds/Acoustic/Enemys/Bullet/Die.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::E_B_DIE) };

	setState(MONSTER_STATE::IDLE);
	setStandingPosState(MONSTER_STANDING_POSITION_STATE::FRONT);
	_currentAnimSet = make_unique<ANIMATION_SET*>();
	setCurrentAnimation(MONSTER_STATE::IDLE);

	startCurrentAnimation();
}

void EnemyShotgun::update()
{
	Monster::update();
	_weapon.update();
}

void EnemyShotgun::render(const HDC& hdc) const
{
	Monster::render(hdc);
	_weapon.render(hdc);
}

void EnemyShotgun::release()
{
	Monster::release();
}

void EnemyShotgun::changeDeathMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX)
{
}
