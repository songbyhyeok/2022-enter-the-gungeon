#include "Monster.h"
#include "MapTool.h"

void Monster::init()
{
	for (auto& animSet : _animSets)
	{
		animSet.anim = {};
		animSet.image = {};
		animSet.soundFile = {};
		animSet.frameLength = 0;
		animSet.frameSpeed = 0;
	}

	_hand = {};
	_moveDirections = { MOVE_DIRECTION::NONE, MOVE_DIRECTION::NONE };
	_currentAnimSet = {};
	_pathIdxVec = {};
	_state = {};
	_standingPosState = {};

	_relativePos = {};
	_handRelativePos = {};

	_currPos = {};
	_currIdx = {};
	_curreAngle = {};
	_hp = {};
	_speed = {};
	_damage = {};
	_defense = {};
	_damage = {};
}

void Monster::update()
{
	if (isDead() == false)
	{
		if (isDamaging())
		{
			static uint16 time = 0;
			if (5 < time)
			{
				time = 0;
				setDamagingValue(false);
				(*(*_currentAnimSet)).image.setTransparency(0);
			}

			++time;
		}

		(*(*_currentAnimSet)).anim.updateKeyFrame((*(*_currentAnimSet)).frameSpeed, (*(*_currentAnimSet)).frameLength);
	}
}

void Monster::render(const HDC& hdc) const
{
	if (isDead() == false)
	{
		switch (!isDamaging())
		{
		case true:
			(*(*_currentAnimSet)).image.renderAnimation(hdc, getRelativePos(), (*(*_currentAnimSet)).anim.getCurrFramePos());
			break;
		case false:
			(*(*_currentAnimSet)).image.renderTransparentAnimation(hdc, getRelativePos(), (*(*_currentAnimSet)).anim.getCurrFramePos());
			break;
		}

		if (_canHands)
			_hand.render(hdc, getHandRelativePos());		
	}
}

void Monster::release()
{
	_pathIdxVec.clear();
}

void Monster::setStateAndAnimation(const MONSTER_STATE& selectedState)
{
	if (selectedState == getState())
		return;

	if (selectedState == MONSTER_STATE::ATTACK && _animSets.size() != 4)
		return;

	stopCurrentAnimationSound(selectedState);
	setState(selectedState);
	setCurrentAnimation(selectedState);
	playCurrentAnimationSound();

	switch (selectedState)
	{
	case MONSTER_STATE::IDLE:
		//_player.setCurrentUseWeaponHidable(false);
		break;
	case MONSTER_STATE::WALKING:
		//_player.setCurrentUseWeaponHidable(false);
		break;
	case MONSTER_STATE::DEATH:
		//changeIdleMotion(MONSTER_STANDING_POSITION_STATE::FRONT);
		break;
	case MONSTER_STATE::ATTACK:
		//_player.setCurrentUseWeaponHidable(false);
		break;
	}
}

void Monster::setRelativeHandPosition(bool isReverseX)
{
	const auto& mRelativePos = getRelativePos();

	if (_canHands)
		_hand.setReverseData({ isReverseX, false });

	switch (isReverseX)
	{
	case true:
		setHandRelativePos({ mRelativePos.x - getSize().x, mRelativePos.y });
		_weapon.setRelativeSP({ getHandRelativePos().x, getHandRelativePos().y + 20});
		break;
	case false:
		setHandRelativePos(mRelativePos);
		_weapon.setRelativeSP({ getHandRelativePos().x + _size.x + 25, getHandRelativePos().y + 20});
		break;
	}
}

bool Monster::isDead() const
{
	if (getState() == MONSTER_STATE::DEATH && !(*(*_currentAnimSet)).anim.isPlaying())
	{
		static uint16 time = 0;
		if (100 < time)
			return true;

		++time;
	}

	return false;
}

bool Monster::isPlayerFound(const POINT& playerCurrIdx) const
{
	if (getDistance(getCurrIdx(), playerCurrIdx) < 6)
	{
		const POINT distance{ _currIdx.x - playerCurrIdx.x, _currIdx.y - playerCurrIdx.y };
		POINT start{}, dest{};
		if (distance.x <= 0)
		{
			start.x = { _currIdx.x };
			dest.x = { _currIdx.x - distance.x };
		}
		else if (0 < distance.x)
		{
			start.x = {_currIdx.x - distance.x};
			dest.x = {_currIdx.x};
		}
		if (distance.y <= 0)
		{
			start.y = { _currIdx.y };
			dest.y = { _currIdx.y - distance.y };
		}
		else if (0 < distance.y)
		{
			start.y = { _currIdx.y - distance.y };
			dest.y = { _currIdx.y };
		}

		for (int i = start.y; i <= dest.y; i++)
		{
			for (int j = start.x; j <= dest.x; j++)
			{
				if ((*_mapToolPtr)->isCurrentPositionMaterialObject({ j, i }))
					return false;
			}
		}

		return true;
	}

	return false;
}

void Monster::initWeapon(const char* gunSource, SOUND_FILE firingSound, SOUND_FILE reloadingSound, const uint16 bulletN, const uint16 bulletSpeed)
{
	_weapon.init("", "Images/Weapons/Gun1.bmp", move(firingSound), move(reloadingSound), _scale, {}, {}, bulletN, bulletSpeed, 100, WEAPON_NAME::PISTOL);
	_weapon.setMapToolPtr(_mapToolPtr);
}

void Monster::reloadWeapon()
{
	if (_weapon.getCurrentFiredBulletCount() != _weapon.getMaxBulletCount())
	{
		_weapon.reload();
	}
}

void Monster::lookAt(const POINT& playerCurrIdx)
{
	const int16 angleToFire = getAngle(getCurrIdx(), playerCurrIdx);
	const int16 currAngle = filterPreciseAngleToSimple(angleToFire);
	setCurrAngle(currAngle);
	switch (currAngle)
	{
	case 0:
		changeMovement(MONSTER_STANDING_POSITION_STATE::FRONTSIDE, false);
		break;
	case 45:
		changeMovement(MONSTER_STANDING_POSITION_STATE::BACKSIDE, false);
		break;
	case 90:
		changeMovement(MONSTER_STANDING_POSITION_STATE::BACK, false);
		break;
	case 135:
		changeMovement(MONSTER_STANDING_POSITION_STATE::BACKSIDE, true);
		break;
	case 180:
		changeMovement(MONSTER_STANDING_POSITION_STATE::FRONTSIDE, true);
		break;
	case 225:
		changeMovement(MONSTER_STANDING_POSITION_STATE::FRONTSIDE, true);
		break;
	case 270:
		changeMovement(MONSTER_STANDING_POSITION_STATE::FRONT, false);
		break;
	case 315:
		changeMovement(MONSTER_STANDING_POSITION_STATE::FRONTSIDE, false);
		break;
	}

	useWeapon(currAngle);
}

void Monster::useWeapon(const int16 shotAngle)
{
	static int16 count = 0;
	if (15 < count)
	{
		setStateAndAnimation(MONSTER_STATE::ATTACK);

		if (_weapon.use(shotAngle) == false)
			reloadWeapon();

		count = 0;
	}

	++count;
}

void Monster::moveToPath()
{	
	if (_pathIdxVec.empty())
		return;

	if (getState() != MONSTER_STATE::DEATH && !isDamaging())
	{
		if (getState() != MONSTER_STATE::WALKING)
		{
			setStateAndAnimation(MONSTER_STATE::WALKING);
			changeMovement(getStandingPosState(), getCurrReverseData().x);
		}

		const auto& guidePath{ _pathIdxVec[_pathIdxVec.size() - 1] };

		auto& verticalDir{ _moveDirections[0] };
		if (guidePath.y != _currIdx.y)
		{
			if (verticalDir == MOVE_DIRECTION::NONE)
			{
				if (guidePath.y < _currIdx.y)
					verticalDir = MOVE_DIRECTION::UP;
				else
					verticalDir = MOVE_DIRECTION::DOWN;
			}

			switch (verticalDir)
			{
			case MOVE_DIRECTION::UP:
				_rect.top = _rect.top - _speed;
				break;
			case MOVE_DIRECTION::DOWN:
				_rect.top = _rect.top + _speed;
				break;
			}
		}
		else
		{
			verticalDir = MOVE_DIRECTION::END;
		}

		auto& horizontalDir{ _moveDirections[1] };
		if (guidePath.x != _currIdx.x)
		{
			if (horizontalDir == MOVE_DIRECTION::NONE)
			{
				if (guidePath.x < _currIdx.x)
					horizontalDir = MOVE_DIRECTION::LEFT;
				else
					horizontalDir = MOVE_DIRECTION::RIGHT;
			}

			switch (horizontalDir)
			{
			case MOVE_DIRECTION::LEFT:
				_rect.left = _rect.left - _speed;
				break;
			case MOVE_DIRECTION::RIGHT:
				_rect.left = _rect.left + _speed;
				break;
			}
		}
		else
		{
			horizontalDir = MOVE_DIRECTION::END;
		}

		if (verticalDir != MOVE_DIRECTION::NONE && horizontalDir != MOVE_DIRECTION::NONE)
		{
			switch (verticalDir)
			{
			case MOVE_DIRECTION::UP:
				switch (horizontalDir)
				{
				case MOVE_DIRECTION::LEFT:
					changeMovement(MONSTER_STANDING_POSITION_STATE::BACKSIDE, true);
					break;
				case MOVE_DIRECTION::RIGHT:
					changeMovement(MONSTER_STANDING_POSITION_STATE::BACKSIDE, false);
					break;
				}
				break;
			case MOVE_DIRECTION::DOWN:
				switch (horizontalDir)
				{
				case MOVE_DIRECTION::LEFT:
					changeMovement(MONSTER_STANDING_POSITION_STATE::FRONTSIDE, true);
					break;
				case MOVE_DIRECTION::RIGHT:
					changeMovement(MONSTER_STANDING_POSITION_STATE::FRONTSIDE, false);
					break;
				}
				break;
			case MOVE_DIRECTION::END:
				if (horizontalDir == MOVE_DIRECTION::END)
				{
					_moveDirections = { MOVE_DIRECTION::NONE, MOVE_DIRECTION::NONE };

					_pathIdxVec.pop_back();
					if (_pathIdxVec.empty())
					{
						setStateAndAnimation(MONSTER_STATE::IDLE);
						changeMovement(getStandingPosState(), getCurrReverseData().x);
					}
				}
				break;
			}
		}
		else if (verticalDir != MOVE_DIRECTION::NONE && horizontalDir == MOVE_DIRECTION::END)
		{
			switch (verticalDir)
			{
			case MOVE_DIRECTION::UP:
				changeMovement(MONSTER_STANDING_POSITION_STATE::BACK, false);
				break;
			case MOVE_DIRECTION::DOWN:
				changeMovement(MONSTER_STANDING_POSITION_STATE::FRONT, false);
				break;
			}
		}
		else if (horizontalDir != MOVE_DIRECTION::NONE && verticalDir == MOVE_DIRECTION::END)
		{
			switch (horizontalDir)
			{
			case MOVE_DIRECTION::LEFT:
				changeMovement(MONSTER_STANDING_POSITION_STATE::FRONTSIDE, true);
				break;
			case MOVE_DIRECTION::RIGHT:
				changeMovement(MONSTER_STANDING_POSITION_STATE::FRONTSIDE, false);
				break;
			}
		}
	}
}

bool Monster::injured(const uint16 damage)
{
	(*(*_currentAnimSet)).image.setTransparency(50);
	setDamagingValue(true);
	setHP(getHP() - damage);
	if (getHP() <= 0)
	{
		die();
		return true;
	}

	return false;
}

void Monster::changeMovement(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX)
{
	switch (getState())
	{
	case MONSTER_STATE::IDLE:
		changeIdleMotion(selectedStandingState, isReverseX);
		break;
	case MONSTER_STATE::WALKING:
		changeWalkingMotion(selectedStandingState, isReverseX);
		break;
	case MONSTER_STATE::DEATH:
		changeDeathMotion(selectedStandingState, isReverseX);
		break;
	case MONSTER_STATE::ATTACK:
		changeAttackMotion(selectedStandingState, isReverseX);
		break;
	}
}

void Monster::setCurrentAnimSetMotionData(const uint16 frameLength, const uint16 playingIdx, const uint16 stopSectionFrame)
{
	(*(*_currentAnimSet)).frameLength = frameLength;
	(*(*_currentAnimSet)).anim.start(playingIdx, stopSectionFrame);
}

void Monster::die()
{
	setStateAndAnimation(MONSTER_STATE::DEATH);
	changeMovement(getStandingPosState(), getCurrReverseData().x);
	_pathIdxVec.clear();
}

void Monster::stopCurrentAnimationSound(const MONSTER_STATE& selectedState) const
{
	if (getState() == MONSTER_STATE::WALKING && selectedState != MONSTER_STATE::WALKING)
	{
		SoundController::getInstance().stop((*(*_currentAnimSet)).soundFile);
	}
}

bool Monster::checkMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingPosState, bool isReverseX)
{
	if (getStandingPosState() == selectedStandingPosState && getCurrReverseData().x == isReverseX)
		return false;

	setCurrReverseData(isReverseX);
	stopCurrentAnimation();
	setStandingPosState(selectedStandingPosState);

	return true;
}