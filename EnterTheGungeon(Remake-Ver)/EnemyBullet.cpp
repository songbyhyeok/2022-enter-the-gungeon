#include "EnemyBullet.h"

void EnemyBullet::init(FPOINT scale, const POINT& sp, POINT size, POINT currPos, POINT currIdx)
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

	_canHands = true;
	_hand = ImageManager::makeImage("Images/Enemys/Bullet/Hand.bmp", { sp.x + _size.x - 10, sp.y + (_size.y / 2) + 10 }, _scale, { 18, 18 }, true, { false, false }, MagentaRGB);

	_animSets.resize(3);
	const uint16 idleN = toInt(MONSTER_STATE::IDLE);
	_animSets[idleN].image = ImageManager::makeImage("Images/Enemys/Bullet/Idle.bmp", sp, _scale, { _size.x * 2, _size.y * 3 }, { 2, 3 }, true, { false, false }, MagentaRGB);
	_animSets[idleN].anim = AnimationManager::makeAnimation(_size, { 2, 3 }, 6.5, 0, 0, false, true);
	_animSets[idleN].frameSpeed = 0.02f;
	_animSets[idleN].frameLength = 2;
	_animSets[idleN].soundFile = { "", SOUND_TYPE::END };

	const uint16 walkingN = toInt(MONSTER_STATE::WALKING);
	_animSets[walkingN].image = ImageManager::makeImage("Images/Enemys/Bullet/Walking.bmp", sp, _scale, { _size.x * 7, _size.y * 3 }, { 7, 3 }, true, { false, false }, MagentaRGB);
	_animSets[walkingN].anim = AnimationManager::makeAnimation(_size, { 7, 3 }, 6.5, 0, 0, false, true);
	_animSets[walkingN].frameSpeed = 0.03f;
	_animSets[walkingN].frameLength = 0;
	_animSets[walkingN].soundFile = { "Sounds/Acoustic/Enemys/Bullet/Walking.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::E_B_WALKING) };

	const uint16 deathN = toInt(MONSTER_STATE::DEATH);
	_animSets[deathN].image = ImageManager::makeImage("Images/Enemys/Bullet/Death.bmp", sp, _scale, { _size.x * 4, _size.y * 1 }, { 4, 1 }, true, { false, false }, MagentaRGB);
	_animSets[deathN].anim = AnimationManager::makeAnimation(_size, { 4, 1 }, 6.5, 0, 0, false, false);
	_animSets[deathN].frameSpeed = 0.04f;
	_animSets[deathN].frameLength = 0;
	_animSets[deathN].soundFile = { "Sounds/Acoustic/Enemys/Bullet/Die.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::E_B_DIE) };

	setState(MONSTER_STATE::IDLE);
	setStandingPosState(MONSTER_STANDING_POSITION_STATE::FRONT);
	_currentAnimSet = make_unique<ANIMATION_SET*>();
	setCurrentAnimation(MONSTER_STATE::IDLE);

	startCurrentAnimation();
}

void EnemyBullet::update()
{
	Monster::update();
	_weapon.update();
}

void EnemyBullet::render(const HDC& hdc) const
{
	Monster::render(hdc);
	_weapon.render(hdc);
}

void EnemyBullet::release()
{
	Monster::release();
}

void EnemyBullet::changeIdleMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX)
{
	if (checkMotion(selectedStandingState, isReverseX))
	{
		switch (selectedStandingState)
		{
		case MONSTER_STANDING_POSITION_STATE::FRONT:
			setCurrentAnimSetMotionData(2, 0, 0);
			break;
		case MONSTER_STANDING_POSITION_STATE::FRONTSIDE:
			setCurrentAnimSetMotionData(4, 2, 2);
			break;
		case MONSTER_STANDING_POSITION_STATE::BACK:
		case MONSTER_STANDING_POSITION_STATE::BACKSIDE:
			setCurrentAnimSetMotionData(6, 4, 4);
			break;
		}
	}
}

void EnemyBullet::changeWalkingMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX)
{
	if (checkMotion(selectedStandingState, isReverseX))
	{
		switch (selectedStandingState)
		{
		case MONSTER_STANDING_POSITION_STATE::FRONT:
			setCurrentAnimSetMotionData(7, 0, 0);
			break;
		case MONSTER_STANDING_POSITION_STATE::FRONTSIDE:
			setCurrentAnimSetMotionData(14, 7, 7);
			break;
		case MONSTER_STANDING_POSITION_STATE::BACK:
		case MONSTER_STANDING_POSITION_STATE::BACKSIDE:
			setCurrentAnimSetMotionData(21, 14, 14);
			break;
		}
	}
}

void EnemyBullet::changeDeathMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX)
{
	if (checkMotion(selectedStandingState, isReverseX) || getState() == MONSTER_STATE::DEATH)
	{
		switch (selectedStandingState)
		{
		case MONSTER_STANDING_POSITION_STATE::FRONT:
		case MONSTER_STANDING_POSITION_STATE::FRONTSIDE:
		case MONSTER_STANDING_POSITION_STATE::BACK:
		case MONSTER_STANDING_POSITION_STATE::BACKSIDE:
			setCurrentAnimSetMotionData(4, 0, 3);
			break;
		}
	}
}
