#pragma once

#include "Player.h"

class MapTool;
class PlayerController : BaseInterface
{
public:
	void						init(const FPOINT& scale, POINT startPosition);
	void						update() override;
	void						render(const HDC& hdc) const override;
	void						release() override;

	void						setMapToolPtr(MapTool* mapToolPtr) { _mapToolPtr = make_shared<MapTool*>(mapToolPtr); }

	void						setFiredBulletPtrsPosAndIdx(const POINT& movedCount, const POINT& distance) { _player.setFiredBulletPtrsPosAndIdx(movedCount, distance); }
	void						setCurrPosAndIdx(const POINT& distance);
	const POINT&				getSize() const { return _player.getPlayerSize(); }
	POINT						getStartPos() const { return _player.getStartPos(); }
	const POINT&				getCurrPos() const { return _player.getCurrPos(); }
	const POINT&				getCurrIdx() const { return _player.getCurrIdx(); }
	const PLAYER_STATE&			getPlayerCurrState() const { return _player.getState(); }

	void						takeDamage(const int16 damage);

private:	
	void						setMoveCount(POINT& moveCount);
	void						setStateAndAnimation(const PLAYER_STATE& selectedPlayerState, const POINT& moveCount = {});

	void						useWeapon();
	void						reloadWeapon();
	void						lookAt();
	bool						checkMotion(PLAYER_STANDING_POSITION_STATE& appliedState, PLAYER_STANDING_POSITION_STATE selectedPlayerPosState, bool isReverseX);
	void						changeMovement(const PLAYER_STANDING_POSITION_STATE& selectedPlayerPosState, bool isReverseX);
	void						changeStandingPosition(const POINT& moveCount);	
	void						changeIdleMotion(PLAYER_STANDING_POSITION_STATE selectedPlayerPosState, bool isReverseX);
	void						changeWalkingMotion(PLAYER_STANDING_POSITION_STATE selectedPlayerPosState, bool isReverseX);
	void						changeDodgeMotion(PLAYER_STANDING_POSITION_STATE selectedPlayerPosState, bool isReverseX);
	void						changeDeathMotion(PLAYER_STANDING_POSITION_STATE selectedPlayerPosState, bool isReverseX);
	void						changeGunDirection();

private:
	Player						_player;	
	shared_ptr<MapTool*>		_mapToolPtr;
	POINT						_startLocation;
	POINT						_endLocation;
	uint16						_rotateSpeed;
};

