#pragma once

#include "BaseInterface.h"
#include "Statistics.h"
#include "AnimationManager.h"
#include "Weapon.h"

enum class MONSTER_STATE
{
	IDLE,
	WALKING,
	DEATH,
	ATTACK
};

enum class MONSTER_STANDING_POSITION_STATE
{
	NONE,
	FRONT,
	FRONTSIDE,
	BACK,
	BACKSIDE
};

enum class ENEMY_NAME
{
	BULLET = 0,
	GIGI = 2,
	KEY = 4,
	RED_SHOTGUN = 6,
	BLUE_SHOTGUN = 8,
	BOOK = 10,
	END = 99
};

enum class BOSS_NAME
{
	SMILEY = 0,
	SHADES = 2,
	END = 99
};

enum class MOVE_DIRECTION
{
	NONE,
	LEFT,
	UP,
	RIGHT,
	DOWN,
	END
};

class Monster : public BaseInterface, public Statistics
{
public:
	void											init();
	void											update() override;
	void											render(const HDC& hdc) const override;
	void											release() override;
	
	void											setMapToolPtr(shared_ptr<MapTool*> mapToolPtr) { _mapToolPtr = mapToolPtr; }

	void											setRect(RECT rect) { _rect = move(rect); }
	const RECT&										getRect() const { return _rect; }
	const POINT&									getSize() const { return (*_currentAnimSet)->image.getImagePrintSize(); }	
	POINT											getStartPos() const { return { getRect().left, getRect().top }; }
	void											setRelativePos(POINT relativePos) { _relativePos = move(relativePos); }	
	const POINT&									getRelativePos() const { return _relativePos; }
	void											setRelativeHandPosition(bool isReverseX);
	const POINT&									getHandRelativePos() const { return _handRelativePos; }
	const auto&										getCurrReverseData() const { return (*(*_currentAnimSet)).image.getReverseData(); }
	void											setFiredBulletPtrsPos(POINT pos) { _weapon.setFiredBulletPtrsPos(move(pos)); }
	void											setFiredBulletPtrsPosAndIdx(const POINT& movedCount, const POINT& distance) { _weapon.setFiredBulletPtrsPosAndIdx(movedCount, distance); }
	void											setDamagingValue(bool value) { _damage = value; }
	const MONSTER_STATE&							getState() const { return _state; }
	void											setPathVec(vector<POINT> pathVec) { _pathIdxVec = move(pathVec); }
	const vector<POINT>&							getPathVec() const { return _pathIdxVec; }
	const MONSTER_STANDING_POSITION_STATE&			getStandingPosState() const { return _standingPosState; }
	void											setStateAndAnimation(const MONSTER_STATE& selectedState);

	// Statistics을(를) 통해 상속됨
	void											setCurrPos(POINT currPos) override { _currPos = move(currPos); }
	const POINT&									getCurrPos() const override { return _currPos; }
	void											setCurrIdx(POINT currIdx) override { _currIdx = move(currIdx); }
	const POINT&									getCurrIdx() const override { return _currIdx; }
	void											setCurrAngle(int16 currAngle) override { _curreAngle = currAngle; }
	const int16&									getCurrAngle() const override { return _curreAngle; }
	void											setHP(int16 hp) override { _hp = hp; }
	const int16&									getHP() const override { return _hp; }
	void											setSpeed(int16 speed) override { _speed = speed; }
	const int16&									getSpeed() const override { return _speed; }
	void											setDamage(int16 damage) override { _damage = damage; }
	const int16&									getDamage() const override { return _damage; }
	void											setDefense(int16 defense) override { _defense = defense; }
	const int16&									getDefense() const override { return _defense; }

	bool											isDamaging() const { return _damage; }
	bool											isDead() const;
	bool											isPlayerFound(const POINT& playerCurrIdx) const;
	
	void											initWeapon(const char* gunSource, SOUND_FILE firingSound, SOUND_FILE reloadingSound, const uint16 bulletN, const uint16 bulletSpeed);
	void											reloadWeapon();
	void											lookAt(const POINT& playerCurrIdx);
	void											useWeapon(const int16 shotAngle);
	void											moveToPath();
	bool											injured(const uint16 damage);	
	void											changeMovement(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX);
	
	static uint16									toInt(MONSTER_STATE monsterState) { return static_cast<uint16>(monsterState); }
	static ENEMY_NAME								toEnemyName(uint16 enemyNameIdx) { return static_cast<ENEMY_NAME>(enemyNameIdx); }
	static BOSS_NAME								toBossName(uint16 bossNameIdx) { return static_cast<BOSS_NAME>(bossNameIdx); }

	virtual											~Monster() {};

protected:
	void											setHandRelativePos(POINT handRelativePos) { _handRelativePos = move(handRelativePos); }
	void											setState(MONSTER_STATE selectedMonsterState) { _state = move(selectedMonsterState); }
	void											setCurrReverseData(bool isReverseX) { (*(*_currentAnimSet)).image.setReverseData({ isReverseX, false }); }
	void											setStandingPosState(MONSTER_STANDING_POSITION_STATE selectedMonsterStandingPositionState) { _standingPosState = move(selectedMonsterStandingPositionState); }
	void											setCurrentAnimation(MONSTER_STATE selectedMonsterState) { (*_currentAnimSet.get()) = &_animSets[toInt(move(selectedMonsterState))]; }
	void											setCurrentAnimSetMotionData(const uint16 frameLength, const uint16 playingIdx, const uint16 stopSectionFrame);

	void											die();
	void											playCurrentAnimationSound() const { SoundController::getInstance().play((*(*_currentAnimSet)).soundFile); }
	void											stopCurrentAnimationSound(const MONSTER_STATE& selectedState) const;
	void											startCurrentAnimation() { (*(*_currentAnimSet)).anim.start(); }
	void											stopCurrentAnimation() { (*(*_currentAnimSet)).anim.stop(); }
	void											resumeCurrentAnimation() { (*(*_currentAnimSet)).anim.resume(); }
	bool											checkMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingPosState, bool isReverseX);
	virtual void									changeIdleMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX) = 0;
	virtual void									changeWalkingMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX) = 0;
	virtual void									changeDeathMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX) = 0;	
	virtual void									changeAttackMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX) = 0;

protected:
	Weapon											_weapon;
	vector<ANIMATION_SET>							_animSets;
	Image											_hand;
	RECT											_rect;
	vector<POINT>									_pathIdxVec;
	array<MOVE_DIRECTION, 2>						_moveDirections;
	shared_ptr<MapTool*>							_mapToolPtr;
	unique_ptr<ANIMATION_SET*>						_currentAnimSet;
	FPOINT											_scale;
	POINT											_size;
	POINT											_relativePos;
	POINT											_handRelativePos;
	MONSTER_STATE									_state;
	MONSTER_STANDING_POSITION_STATE					_standingPosState;
	bool											_isDamaging;
	bool											_canHands;
};

