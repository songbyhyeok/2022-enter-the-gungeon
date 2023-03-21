#pragma once

#include "BaseInterface.h"
#include "AnimationManager.h"
#include "BulletManager.h"

enum class WEAPON_NAME
{
	PISTOL,
	END
};

class MapTool;
class Weapon : public BaseInterface
{
public:
	void					init(const char* wRotSource, const char* wSource, SOUND_FILE fireSoundFile, SOUND_FILE reloadingSoundFile, const FPOINT& scale, const POINT& sp, const float range, const uint16 bulletN, const uint16 speed, const uint16 damage, WEAPON_NAME name);
	void					update() override;
	void					render(const HDC& hdc) const override;
	void					release() override;

	void					setMapToolPtr(shared_ptr<MapTool*> mapToolPtr);
	
	void					setFiredBulletPtrsPos(POINT pos) { _bulletMgr.setFiredBulletPtrsPos(move(pos)); }
	void					setFiredBulletPtrsPosAndIdx(const POINT& movedCount, const POINT& distance) { _bulletMgr.setFiredBulletPtrsPosAndIdx(movedCount, distance); }
	void					setWeaponHandlePosition(POINT handPosition) { _weapon.setStartPosition(move(handPosition)); }
	void					setCurrentUseWeaponHidable(bool isHidingWeapon) { _isHidingWeapon = isHidingWeapon; }
	const WEAPON_NAME&		getCurrentUseWeaponName() const { return _name; }
	void					setRelativeSP(POINT relativeSP) { _relativeSP = move(relativeSP); }
	const POINT&			getRelativeSP() const { return _relativeSP; }
	uint16					getMaxBulletCount() const { return _maxBulletN; }
	uint16					getCurrentFiredBulletCount() const { return _bulletMgr.getCurrentFiredBulletCount(); }

	bool					isHidingWeapon() const { return _isHidingWeapon; }
	bool					use(const int16 angleToFire);

	void					reload();

private:
	bool					canRotationUse() const { return _rotationUse; }

private:
	ANIMATION_SET			_reloadAni;
	ANIMATION_SET			_shotEffectAni;
	BulletManager			_bulletMgr;
	Image					_weapon;
	SOUND_FILE				_fireSoundFile;
	shared_ptr<MapTool*>	_mapToolPtr;
	POINT					_relativeSP;
	float					_range;
	uint16					_speed;
	uint16					_damage;
	uint16					_maxBulletN;
	int16					_currentAngle;
	WEAPON_NAME				_name;
	bool					_isHidingWeapon;
	bool					_rotationUse;
};

