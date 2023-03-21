#include "PlayerController.h"
#include "KeyController.h"
#include "MapTool.h"
#include "Debugging.h"
#include "MouseHandler.h"

void PlayerController::init(const FPOINT& scale, POINT startPosition)
{
	const auto& subResolution = ScreenManager::getInstance().getSubResolution();
	const POINT& mapToolSize = (*(*_mapToolPtr)).getTileSize();
	_startLocation = { (*(*_mapToolPtr)).getStartLocation() };
	_endLocation = { (_startLocation.x + subResolution.x) - mapToolSize.x, (_startLocation.y + subResolution.y) - mapToolSize.y };

	_player.setMapToolPtr(_mapToolPtr);
	_player.init(scale, { (_startLocation.x + subResolution.x) / 2, (_startLocation.y + subResolution.y) / 2 }, { static_cast<int>(mapToolSize.x * 1.0f),  static_cast<int>(mapToolSize.y * 1.3f)}, move(startPosition));
	_player.setCurrIdx((*_mapToolPtr)->convertPosToIndex(_player.getStartPos(), _player.getPlayerSize()));
		
	_rotateSpeed = 0;
}

void PlayerController::update()
{
	if (KeyController::isToggleKey(VK_Pkey))
		_player.setInvincibility(!_player.isInvincibility());

	if (_player.isDead())
		return;

	static POINT moveCount{};

	const int16& dodgeSpeed = _player.getDodgeSpeed();
	switch(_player.isDodgeing())
	{
	case true:	
 		(*(*_mapToolPtr)).updateMapForPlaying( { (moveCount.x * dodgeSpeed), (moveCount.y * dodgeSpeed) });
		break;
	case false:
		setMoveCount(moveCount);
		switch (moveCount.x != 0 || moveCount.y != 0)
		{
		case true:
			switch (MouseHandler::isRightClick())
			{
			case true:
				setStateAndAnimation(PLAYER_STATE::DODGE, moveCount);
				break;
			case false:
				setStateAndAnimation(PLAYER_STATE::WALKING, moveCount);
				break;
			}
			break;
		case false:
			setStateAndAnimation(PLAYER_STATE::IDLE);
			break;
		}

		changeGunDirection();
		changeStandingPosition(moveCount);
		break;
	}
	
	useWeapon();
	reloadWeapon();

	_player.update();
}

void PlayerController::render(const HDC& hdc) const
{
	_player.render(hdc);
	Debugging::renderInteger(hdc, "µ¸¿ò", 40, { 500, 500 }, WhiteRGB, true);
}

void PlayerController::release()
{
	_player.release();
}

void PlayerController::setCurrPosAndIdx(const POINT& distance)
{
	_player.setCurrPos(move((*_mapToolPtr)->convertCurrPos(_player.getStartPos(), distance)));
	_player.setCurrIdx(move((*_mapToolPtr)->convertPosToIndex(_player.getCurrPos(), _player.getPlayerSize())));
}

void PlayerController::takeDamage(const int16 damage)
{
	if (_player.injured(damage))
	{
		setStateAndAnimation(PLAYER_STATE::DEATH);
		changeMovement(_player.getStandingPositionState(), _player.getCurrReverseData().x);
	}
}

void PlayerController::setMoveCount(POINT& moveCount)
{
	moveCount = { 0, 0 };
	if (KeyController::isStayKeyDown(VK_Akey))
	{
		moveCount.x += -1;
	}
	if (KeyController::isStayKeyDown(VK_Dkey))
	{
		moveCount.x += 1;
	}
	if (KeyController::isStayKeyDown(VK_Wkey))
	{
		moveCount.y += -1;
	}
	if (KeyController::isStayKeyDown(VK_Skey))
	{
		moveCount.y += 1;
	}
}

void PlayerController::setStateAndAnimation(const PLAYER_STATE& selectedPlayerState, const POINT& moveCount)
{
	if (_player.getState() == PLAYER_STATE::DEATH)
		return;

	_player.stopCurrentAnimationSound(selectedPlayerState);
	_player.setState(selectedPlayerState);
	_player.setCurrentAnimation(selectedPlayerState);
	_player.playCurrentAnimationSound();

	const int16& speed = _player.getSpeed();
	switch (selectedPlayerState)
	{
	case PLAYER_STATE::IDLE:
		_player.setCurrentUseWeaponHidable(false);
		(*(*_mapToolPtr)).updateMapForPlaying({});
		break;
	case PLAYER_STATE::WALKING:
		_player.setCurrentUseWeaponHidable(false);
		(*(*_mapToolPtr)).updateMapForPlaying({ (moveCount.x * speed), (moveCount.y * speed) });
		break;
	case PLAYER_STATE::DODGE:
		_player.resumeCurrentAnimation();
		_player.setCurrentUseWeaponHidable(true);
		(*(*_mapToolPtr)).updateMapForPlaying({ (moveCount.x * speed), (moveCount.y * speed) });
		break;
	case PLAYER_STATE::DEATH:
		_player.setCurrentUseWeaponHidable(true);
		break;
	}
}

void PlayerController::useWeapon()
{
	static int16 count = 0;
	if (MouseHandler::isLeftClick())
	{
		if (6 < count)
		{
			lookAt();
			_player.shotWithWeapon((*(*_mapToolPtr)).getMapPreciseAngleByMouse());
			count = 0;
		}
	}

	if (MouseHandler::isStayKeyDownByLeftClick())
	{
		if (9 < count)
		{
			lookAt();
			_player.shotWithWeapon((*(*_mapToolPtr)).getMapPreciseAngleByMouse());
			count = 0;
		}
	}

	++count;
}

void PlayerController::reloadWeapon()
{
	if (KeyController::isOnceKeyUp(VK_Rkey))
	{
		_player.reloadWeapon();
	}
}

void PlayerController::lookAt()
{
	const int16 currAngle = filterPreciseAngleToSimple(((*_mapToolPtr))->getMapPreciseAngleByMouse());
	_player.setCurrAngle(currAngle);
	PLAYER_STANDING_POSITION_STATE selectedPosSt{};
	bool isReverseX = false;
	switch (currAngle)
	{
	case 0:
		selectedPosSt = PLAYER_STANDING_POSITION_STATE::FRONTSIDE;
		isReverseX = false;
		break;
	case 45:
		selectedPosSt = PLAYER_STANDING_POSITION_STATE::BACKSIDE;
		isReverseX = false;
		break;
	case 90:
		selectedPosSt = PLAYER_STANDING_POSITION_STATE::BACK;
		isReverseX = false;
		break;
	case 135:
		selectedPosSt = PLAYER_STANDING_POSITION_STATE::BACKSIDE;
		isReverseX = true;
		break;
	case 180:
		selectedPosSt = PLAYER_STANDING_POSITION_STATE::FRONTSIDE;
		isReverseX = true;
		break;
	case 225:
		selectedPosSt = PLAYER_STANDING_POSITION_STATE::FRONTSIDE;
		isReverseX = true;
		break;
	case 270:
		selectedPosSt = PLAYER_STANDING_POSITION_STATE::FRONT;
		isReverseX = false;
		break;
	case 315:
		selectedPosSt = PLAYER_STANDING_POSITION_STATE::FRONTSIDE;
		isReverseX = false;
		break;
	}

	_player.setStandingPositionState(move(selectedPosSt));
	_player.setCurrReverseData(isReverseX);
	_player.setCurrentHandPosition(isReverseX);
}

bool PlayerController::checkMotion(PLAYER_STANDING_POSITION_STATE& appliedState, PLAYER_STANDING_POSITION_STATE selectedPlayerPosState, bool isReverseX)
{
	if (appliedState == selectedPlayerPosState && _player.getCurrentAnimationSet().image.getReverseData().x == isReverseX)
		return false;

	_player.setCurrReverseData(isReverseX);
	if (_player.getState() != PLAYER_STATE::DODGE)
	{
		_player.setCurrentHandPosition(isReverseX);
		_player.stopCurrentAnimation();
	}

	appliedState = selectedPlayerPosState;
	return true;
}

void PlayerController::changeMovement(const PLAYER_STANDING_POSITION_STATE& selectedPlayerPosState, bool isReverseX)
{
	_player.setStandingPositionState(selectedPlayerPosState);

	switch (_player.getState())
	{
	case PLAYER_STATE::IDLE:
		changeIdleMotion(selectedPlayerPosState, isReverseX);
		break;
	case PLAYER_STATE::WALKING:
		changeWalkingMotion(selectedPlayerPosState, isReverseX);
		break;
	case PLAYER_STATE::DODGE:
		changeDodgeMotion(selectedPlayerPosState, isReverseX);
		break;
	case PLAYER_STATE::DEATH:
		changeDeathMotion(selectedPlayerPosState, isReverseX);
		break;
	}
}

void PlayerController::changeStandingPosition(const POINT& moveCount)
{
	if (_player.getState() == PLAYER_STATE::DEATH)
		return;

	switch (_player.getState() != PLAYER_STATE::DODGE)
	{
	case true:
	{
		const int16 currentAngle = (*(*_mapToolPtr)).getMapAngleByMouse();
		if (currentAngle != -1)
		{
			Debugging::setPrintValue(currentAngle);
			switch (currentAngle)
			{
			case 0:
			case 315:
				changeMovement(PLAYER_STANDING_POSITION_STATE::FRONTSIDE, false);
				break;
			case 45:
				changeMovement(PLAYER_STANDING_POSITION_STATE::BACKSIDE, false);
				break;
			case 90:
				changeMovement(PLAYER_STANDING_POSITION_STATE::BACK, false);
				break;
			case 135:
				changeMovement(PLAYER_STANDING_POSITION_STATE::BACKSIDE, true);
				break;
			case 180:
			case 225:
				changeMovement(PLAYER_STANDING_POSITION_STATE::FRONTSIDE, true);
				break;
			case 270:
				changeMovement(PLAYER_STANDING_POSITION_STATE::FRONT, false);
				break;
			}
		}
	}
	break;
	case false:
		if (moveCount.x < 0)
		{
			if (moveCount.y < 0)
			{
				changeMovement(PLAYER_STANDING_POSITION_STATE::BACKSIDE, true);
				return;
			}
			else if (moveCount.y > 0)
			{
				changeMovement(PLAYER_STANDING_POSITION_STATE::FRONTSIDE, true);
				return;
			}

			changeMovement(PLAYER_STANDING_POSITION_STATE::FRONTSIDE, true);
		}
		else if (moveCount.x > 0)
		{
			if (moveCount.y < 0)
			{
				changeMovement(PLAYER_STANDING_POSITION_STATE::BACKSIDE, false);
				return;
			}
			else if (moveCount.y > 0)
			{
				changeMovement(PLAYER_STANDING_POSITION_STATE::FRONTSIDE, false);
				return;
			}

			changeMovement(PLAYER_STANDING_POSITION_STATE::FRONTSIDE, false);
		}
		if (moveCount.y < 0)
			changeMovement(PLAYER_STANDING_POSITION_STATE::BACK, false);
		else if (moveCount.y > 0)
			changeMovement(PLAYER_STANDING_POSITION_STATE::FRONT, false);
		break;
	}
}

void PlayerController::changeIdleMotion(PLAYER_STANDING_POSITION_STATE selectedPlayerPosState, bool isReverseX)
{
	static PLAYER_STANDING_POSITION_STATE appliedState = PLAYER_STANDING_POSITION_STATE::NONE;
	if (checkMotion(appliedState, selectedPlayerPosState, isReverseX))
	{
		switch (appliedState)
		{
		case PLAYER_STANDING_POSITION_STATE::FRONT:
			_player.setCurrentAnimSetMotionData(4, 0, 0);
			break;
		case PLAYER_STANDING_POSITION_STATE::FRONTSIDE:
			_player.setCurrentAnimSetMotionData(8, 4, 4);
			break;
		case PLAYER_STANDING_POSITION_STATE::BACK:
			_player.setCurrentAnimSetMotionData(12, 8, 8);
			break;
		case PLAYER_STANDING_POSITION_STATE::BACKSIDE:
			_player.setCurrentAnimSetMotionData(16, 12, 12);
			break;
		}
	}
}

void PlayerController::changeWalkingMotion(PLAYER_STANDING_POSITION_STATE selectedPlayerPosState, bool isReverseX)
{
	static PLAYER_STANDING_POSITION_STATE appliedState = PLAYER_STANDING_POSITION_STATE::NONE;
	if (checkMotion(appliedState, selectedPlayerPosState, isReverseX))
	{
		switch (appliedState)
		{
		case PLAYER_STANDING_POSITION_STATE::FRONT:
			_player.setCurrentAnimSetMotionData(6, 0, 0);
			break;
		case PLAYER_STANDING_POSITION_STATE::FRONTSIDE:
			_player.setCurrentAnimSetMotionData(12, 6, 6);
			break;
		case PLAYER_STANDING_POSITION_STATE::BACK:
			_player.setCurrentAnimSetMotionData(18, 12, 12);
			break;
		case PLAYER_STANDING_POSITION_STATE::BACKSIDE:
			_player.setCurrentAnimSetMotionData(24, 18, 18);
			break;
		}
	}
}

void PlayerController::changeDodgeMotion(PLAYER_STANDING_POSITION_STATE selectedPlayerPosState, bool isReverseX)
{
	static PLAYER_STANDING_POSITION_STATE appliedState = PLAYER_STANDING_POSITION_STATE::NONE;
	if (checkMotion(appliedState, selectedPlayerPosState, isReverseX))
	{
		switch (appliedState)
		{
		case PLAYER_STANDING_POSITION_STATE::FRONT:
			_player.setCurrentAnimSetMotionData(9, 0, 0);
			break;
		case PLAYER_STANDING_POSITION_STATE::FRONTSIDE:
			_player.setCurrentAnimSetMotionData(18, 9, 9);
			break;
		case PLAYER_STANDING_POSITION_STATE::BACK:
			_player.setCurrentAnimSetMotionData(27, 18, 18);
			break;
		case PLAYER_STANDING_POSITION_STATE::BACKSIDE:
			_player.setCurrentAnimSetMotionData(36, 27, 27);
			break;
		}
	}
}

void PlayerController::changeDeathMotion(PLAYER_STANDING_POSITION_STATE selectedPlayerPosState, bool isReverseX)
{
	static PLAYER_STANDING_POSITION_STATE appliedState = PLAYER_STANDING_POSITION_STATE::NONE;
	if (checkMotion(appliedState, selectedPlayerPosState, isReverseX))
	{
		switch (appliedState)
		{
		case PLAYER_STANDING_POSITION_STATE::FRONT:
		case PLAYER_STANDING_POSITION_STATE::FRONTSIDE:
		case PLAYER_STANDING_POSITION_STATE::BACK:
		case PLAYER_STANDING_POSITION_STATE::BACKSIDE:
			_player.setCurrentAnimSetMotionData(7, 0, 6);
			break;
		}
	}
}

void PlayerController::changeGunDirection()
{
	_player.setCurrAngle(-((*_mapToolPtr))->getMapPreciseAngleByMouse());
}


