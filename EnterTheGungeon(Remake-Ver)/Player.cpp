#include "Player.h"

void Player::init(const FPOINT& scale, const POINT& playerSP, const POINT& playerSize, POINT uiSP)
{	
	_hand = ImageManager::makeImage("Images/Player/Hand.bmp", { playerSP.x + playerSize.x - 10, playerSP.y + (playerSize.y / 2) + 10}, scale, { 18, 18 }, true, { false, false }, MagentaRGB);
	
	_pistol.init("Images/Weapons/PistolRotation.bmp", "Images/Weapons/Pistols.bmp", {"Sounds/Acoustic/Weapons/PistolFiring.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::PISTOL_FIRING) }, { "Sounds/Acoustic/Weapons/PistolReloading.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::PISTOL_RELOADING) }, scale, { playerSP.x + playerSize.x + 20, playerSP.y + (playerSize.y / 2) - 10 }, {}, 6, 30, 100, WEAPON_NAME::PISTOL);
	_currentUseWeaponPtr = make_unique<Weapon*>(&_pistol);
	(*_currentUseWeaponPtr)->setMapToolPtr(_mapToolPtr);

	const uint16 idleN = toInt(PLAYER_STATE::IDLE);
	_animSets[idleN].image = ImageManager::makeImage("Images/Player/Idle.bmp", playerSP, scale, { playerSize.x * 4, playerSize.y * 4 }, { 4, 4 }, true, { false, false }, IvoryRGB);
	_animSets[idleN].image.setTransparency(255);
	_animSets[idleN].anim = AnimationManager::makeAnimation(playerSize, { 4, 4 }, 6.5, 0, 0, false, true);
	_animSets[idleN].frameSpeed = 0.02f;
	_animSets[idleN].frameLength = 0;
	_animSets[idleN].soundFile = { "", SOUND_TYPE::END};

	const uint16 walkingN = toInt(PLAYER_STATE::WALKING);
	_animSets[walkingN].image = ImageManager::makeImage("Images/Player/Walking.bmp", playerSP, scale, { playerSize.x * 6, playerSize.y * 4 }, { 6, 4 }, true, { false, false }, IvoryRGB);
	_animSets[walkingN].image.setTransparency(255);
	_animSets[walkingN].anim = AnimationManager::makeAnimation(playerSize, { 6, 4 }, 6.5, 0, 0, false, true);
	_animSets[walkingN].frameSpeed = 0.03f;
	_animSets[walkingN].frameLength = 0;
	_animSets[walkingN].soundFile = { "Sounds/Acoustic/Player/Walking.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::P_WALKING)};

	const uint16 dodgeN = toInt(PLAYER_STATE::DODGE);
	const POINT& dodgeSize{ static_cast<int>(playerSize.x * 1.5f), static_cast<int>(playerSize.y * 1.5f) };
	_animSets[dodgeN].image = ImageManager::makeImage("Images/Player/Dodge.bmp", playerSP, scale, { dodgeSize.x * 9, dodgeSize.y * 4 }, { 9, 4 }, true, { false, false }, IvoryRGB);
	_animSets[dodgeN].image.setTransparency(255);
	_animSets[dodgeN].anim = AnimationManager::makeAnimation(dodgeSize, { 9, 4 }, 6.5, 0, 0, false, false);
	_animSets[dodgeN].frameSpeed = 0.07f;
	_animSets[dodgeN].frameLength = 0;
	_animSets[dodgeN].soundFile = { "Sounds/Acoustic/Player/Dodge.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::P_DODGE) };

	const uint16 deathN = toInt(PLAYER_STATE::DEATH);
	_animSets[deathN].image = ImageManager::makeImage("Images/Player/Death.bmp", playerSP, scale, { playerSize.x * 7, playerSize.y * 1 }, { 7, 1 }, true, { false, false }, IvoryRGB);
	_animSets[deathN].image.setTransparency(255);
	_animSets[deathN].anim = AnimationManager::makeAnimation(playerSize, { 7, 1 }, 6.5, 0, 0, false, false);
	_animSets[deathN].frameSpeed = 0.04f;
	_animSets[deathN].frameLength = 0;
	_animSets[deathN].soundFile = { "", SOUND_TYPE::END};	

	_rect = move(generateRect(playerSP, playerSize));

	setInvincibility(false);

	setState(PLAYER_STATE::IDLE);
	setStandingPositionState(PLAYER_STANDING_POSITION_STATE::FRONT);
	_currentAnimSet = make_unique<ANIMATION_SET*>();
	setCurrentAnimation(PLAYER_STATE::IDLE);

	setHP(7);
	setDefense(7);
	setDamage(5);
	setSpeed(10);
	setDodgeSpeed(15);
	_ui.init(scale, move(uiSP), getStartPos(), _pistol.getCurrentUseWeaponName(), _pistol.getMaxBulletCount(), _hp);

	_currPos = {};
	_currIdx = {};
	_curreAngle = {};	

	startCurrentAnimation();
}

void Player::update()
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
			}

			++time;
		}

		(*(*_currentAnimSet)).anim.updateKeyFrame((*(*_currentAnimSet)).frameSpeed, (*(*_currentAnimSet)).frameLength);
		(*(*_currentUseWeaponPtr)).update();
		_ui.update();
	}
}

void Player::render(const HDC& hdc) const
{
	if (isDead() == false)
	{
		switch (!isDamaging())
		{
		case true:
			(*(*_currentAnimSet)).image.renderAnimation(hdc, {}, (*(*_currentAnimSet)).anim.getCurrFramePos());
			break;
		case false:
			(*(*_currentAnimSet)).image.renderTransparentAnimation(hdc, {}, (*(*_currentAnimSet)).anim.getCurrFramePos());
			break;
		}

		if (!isDodgeing())
		{
			_hand.render(hdc, {});
		}

		(*(*_currentUseWeaponPtr)).render(hdc);
		_ui.render(hdc);
	}
}

void Player::release()
{
	_pistol.release();
	_currentUseWeaponPtr.reset();
	_ui.release();
}

void Player::setCurrentAnimSetMotionData(const uint16 frameLength, const uint16 playingIdx, const uint16 stopSectionFrame)
{
	(*(*_currentAnimSet)).frameLength = frameLength;
	(*(*_currentAnimSet)).anim.start(playingIdx, stopSectionFrame);
}

void Player::setCurrentHandPosition(bool isReverseX)
{
	const POINT handSP = { (*(*_currentAnimSet)).image.getStartPosition().x + 70, (*(*_currentAnimSet)).image.getStartPosition().y + (70 / 2) + 10 };
	_hand.setReverseData({ isReverseX, false });
	switch (isReverseX)
	{
	case true:
		_hand.setStartPosition({ handSP.x - getPlayerSize().x, handSP.y });
		(*(*_currentUseWeaponPtr)).setRelativeSP({ _hand.getStartPosition().x - 20, _hand.getStartPosition().y });
		break;
	case false:
		_hand.setStartPosition(handSP);
		(*(*_currentUseWeaponPtr)).setRelativeSP({ _hand.getStartPosition().x + 20, _hand.getStartPosition().y });
		break;
	}
}

bool Player::isDead() const
{
	if (getState() == PLAYER_STATE::DEATH && !(*(*_currentAnimSet)).anim.isPlaying())
	{
		static uint16 time = 0;
		if (70 < time)
			return true;

		++time;
	}

	return false;
}

void Player::shotWithWeapon(const int16 angleToFire)
{
	if (getState() != PLAYER_STATE::DODGE && _ui.isReloading() == false)
	{
		if ((*(*_currentUseWeaponPtr)).use(angleToFire))
			_ui.useBulletImage();
	}
}

void Player::reloadWeapon()
{
	if (getState() != PLAYER_STATE::DODGE &&
		_ui.isReloading() == false &&
		(*(*_currentUseWeaponPtr)).getCurrentFiredBulletCount() != (*(*_currentUseWeaponPtr)).getMaxBulletCount())
	{
		(*(*_currentUseWeaponPtr)).reload();
		_ui.setReloading(true);
	}
}

void Player::stopCurrentAnimationSound(const PLAYER_STATE& selectedPlayerState) const
{
	static auto& soundCtr = SoundController::getInstance();
	if (getState() == PLAYER_STATE::WALKING && selectedPlayerState != PLAYER_STATE::WALKING)
	{
		soundCtr.stop((*(*_currentAnimSet)).soundFile);
	}
	else if (getState() == PLAYER_STATE::DODGE)
	{
		/*if (selectedPlayerState == PLAYER_STATE::WALKING)
			soundCtr.stop(_animSets[toInt(PLAYER_STATE::WALKING)].soundFile);*/

		soundCtr.stop((*(*_currentAnimSet)).soundFile);
	}
}

bool Player::injured(const uint16 damage)
{
	if (isInvincibility())
		return false;

	(*(*_currentAnimSet)).image.setTransparency(50);
	setDamagingValue(true);
	setHP(getHP() - damage);
	_ui.setHearts();
	if (getHP() < 0)
	{
		return true;
	}

	return false;
}







