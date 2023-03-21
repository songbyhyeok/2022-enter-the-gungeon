#pragma once

#include "BaseInterface.h"
#include "Statistics.h"
#include "UserInterface.h"
#include "Weapon.h"

enum class PLAYER_STATE
{
	IDLE,
	WALKING,
	DODGE,
	DEATH
};

enum class PLAYER_STANDING_POSITION_STATE 
{
	NONE,
	FRONT,
	FRONTSIDE,
	BACK,
	BACKSIDE
};

class Player : public BaseInterface, public Statistics
{
public:
	void											init(const FPOINT& scale, const POINT& playerSP, const POINT& playerSize, POINT uiSP);
	void											update() override;
	void											render(const HDC& hdc) const override;
	void											release() override;

	void											setMapToolPtr(shared_ptr<MapTool*> mapToolPtr) { _mapToolPtr = mapToolPtr; }

	void											setCurrentAnimSetMotionData(const uint16 frameLength, const uint16 playingIdx, const uint16 stopSectionFrame);
	void											setCurrentAnimation(PLAYER_STATE selectedPlayerState) { (*_currentAnimSet.get()) = &_animSets[toInt(move(selectedPlayerState))]; }
	void											setCurrReverseData(bool isReverseX) { (*(*_currentAnimSet)).image.setReverseData({ isReverseX, false }); }
	const REVERSE_DATA&								getCurrReverseData() const { return (*(*_currentAnimSet)).image.getReverseData(); }
	const	POINT&									getPlayerSize() const { return (*(*_currentAnimSet)).image.getImagePrintSize(); }
	const	ANIMATION_SET&							getCurrentAnimationSet() const { return (*(*_currentAnimSet)); }
	POINT											getStartPos() const { return { _rect.left, _rect.top }; }
	void											setState(PLAYER_STATE selectedPlayerState) { _state = move(selectedPlayerState); }
	const	PLAYER_STATE&							getState() const { return _state; }
	void											setCurrentHandPosition(bool isReverseX);
	const	POINT&									getCurrentHandPosition() const { return _hand.getStartPosition(); }
	void											setStandingPositionState(PLAYER_STANDING_POSITION_STATE selectedPlayerStandingPositionState) { _standingPosState = move(selectedPlayerStandingPositionState); }
	const	PLAYER_STANDING_POSITION_STATE&			getStandingPositionState() const { return _standingPosState; }
	void											setFiredBulletPtrsPosAndIdx(const POINT& movedCount, const POINT& distance) { (*_currentUseWeaponPtr)->setFiredBulletPtrsPosAndIdx(movedCount, distance); }
	void											setCurrentUseWeaponHidable(bool isHidable) { (*_currentUseWeaponPtr)->setCurrentUseWeaponHidable(isHidable); }
	uint16											getCurrentFiredBulletsCount() const { (*_currentUseWeaponPtr)->getCurrentFiredBulletCount(); }
	void											setDamagingValue(bool value) { _damage = value; }
	void											setDodgeSpeed(int16 dodgeSpeed) { _dodgeSpeed = dodgeSpeed; }
	const int16&									getDodgeSpeed() const { return _dodgeSpeed; }
	void											setInvincibility(bool invincibility) { _invincibility = invincibility; }
	bool											isInvincibility() const { return _invincibility; }

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

	bool											isDead() const;
	bool											isPlaying() const { return (*(*_currentAnimSet)).anim.isPlaying(); }
	bool											isDodgeing() const { return getState() == PLAYER_STATE::DODGE && isPlaying(); }
	bool											isDamaging() const { return _damage; }

	void											shotWithWeapon(const int16 angleToFire);
	void											reloadWeapon();
	void											startCurrentAnimation() { (*(*_currentAnimSet)).anim.start(); }
	void											stopCurrentAnimation() { (*(*_currentAnimSet)).anim.stop(); }
	void											resumeCurrentAnimation() { (*(*_currentAnimSet)).anim.resume(); }
	void											playCurrentAnimationSound() const { SoundController::getInstance().play((*(*_currentAnimSet)).soundFile); }
	void											stopCurrentAnimationSound(const PLAYER_STATE& selectedPlayerState) const;
	bool											injured(const uint16 damage);

private:	
	static uint16									toInt(PLAYER_STATE playerState) { return static_cast<uint16>(playerState); }

private:	
	Weapon											_pistol;
	UserInterface									_ui;
	array<ANIMATION_SET, 4>							_animSets;
	Image											_hand;
	RECT											_rect;
	shared_ptr<MapTool*>							_mapToolPtr;
	unique_ptr<Weapon*>								_currentUseWeaponPtr;
	unique_ptr<ANIMATION_SET*>						_currentAnimSet;
	PLAYER_STATE									_state;
	PLAYER_STANDING_POSITION_STATE					_standingPosState;
	int16											_dodgeSpeed;
	bool											_damage;
	bool											_invincibility;
};

