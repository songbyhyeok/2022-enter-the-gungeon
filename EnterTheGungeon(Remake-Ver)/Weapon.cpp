#include "Weapon.h"
#include "KeyController.h"

void Weapon::init(const char* wRotSource, const char* wSource, SOUND_FILE fireSoundFile, SOUND_FILE reloadingSoundFile, const FPOINT& scale, const POINT& sp, const float range, const uint16 bulletN, const uint16 speed, const uint16 damage, WEAPON_NAME name)
{
	if (wRotSource != "")
	{
		_rotationUse = true;

		_reloadAni.image = ImageManager::makeImage(wRotSource, sp, scale, { 65, 65 * 2 }, { 1, 2 }, true, { false, false }, MagentaRGB);
		_reloadAni.anim = AnimationManager::makeAnimation({ 65, 65 }, { 1, 3 }, 6.5, 0, 0, false, false);
		_reloadAni.frameSpeed = 0.15f;
		_reloadAni.frameLength = 0;		
	}
	_reloadAni.soundFile = move(reloadingSoundFile);

	_weapon = ImageManager::makeImage(wSource, sp, scale, { 65, 105 }, { 1, 3 }, true, { false, false }, MagentaRGB);
	_fireSoundFile = move(fireSoundFile);
	_name = move(name);
	_range = range;
	_speed = speed;
	_damage = damage;
	setCurrentUseWeaponHidable(false);

	_maxBulletN = bulletN;
	_bulletMgr.init(scale, _weapon.getStartPosition(), _maxBulletN, _speed);

	_relativeSP = {};
}

void Weapon::update()
{
	if (canRotationUse())
		_reloadAni.anim.updateKeyFrame(_reloadAni.frameSpeed, _reloadAni.frameLength);

	_bulletMgr.update();
}

void Weapon::render(const HDC& hdc) const
{	
	if (canRotationUse())
		_reloadAni.image.renderAnimation(hdc, {}, _reloadAni.anim.getCurrFramePos());

	if (isHidingWeapon() == false)
	{ }
	//_weapon.renderTransparentRotatingFrame(hdc, {}, { }, _currentAngle);

	_bulletMgr.render(hdc);
}

void Weapon::release()
{
	_bulletMgr.release();
}

void Weapon::setMapToolPtr(shared_ptr<MapTool*> mapToolPtr)
{
	_mapToolPtr = mapToolPtr;
	_bulletMgr.setMapToolPtr(_mapToolPtr);
}

bool Weapon::use(const int16 angleToFire)
{
	if (isHidingWeapon() == false)
	{
		if (_bulletMgr.fire(getRelativeSP(), angleToFire))
		{
			auto& soundCtr = SoundController::getInstance();
			soundCtr.stop(_fireSoundFile);
			soundCtr.play(_fireSoundFile);

			return true;
		}
	}

	return false;
}

void Weapon::reload()
{
	_bulletMgr.fillMagazine(_weapon.getStartPosition());

	if (canRotationUse())
		_reloadAni.anim.start();

	auto& soundCtr = SoundController::getInstance();
	soundCtr.stop(_reloadAni.soundFile);
	soundCtr.play(_reloadAni.soundFile);
}


