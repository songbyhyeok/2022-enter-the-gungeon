#pragma once

#include "Weapon.h"

class UserInterface : public BaseInterface
{
public:
	void							init(const FPOINT& scale, const POINT& startLocation, const POINT& playerSP, const WEAPON_NAME& selectedWeaponName, const uint16 bulletN, const int16 playerHP);
	void							update() override;
	void							render(const HDC& hdc) const override;
	void							release() override;

	void							setReloading(bool isReloading) { _isReloading = isReloading; }

	bool							isReloading() const { return _isReloading; }

	void							setHearts();
	void							useBulletImage();	

private:
	void							initSelectedWeaponImage(FPOINT scale, WEAPON_NAME weaponName);
	void							moveLoadBar();
	void							fillMagazine();

private:
	vector<Image>					_bulletImagesVec;
	array<Image, 3>					_hearts;
	array<Image, 2>					_emps;
	Image							_weaponUIBorder;
	Image							_selectedWeaponUI;
	Image							_loadBorder;
	Image							_loadBar;
	Image							_magazineUpperBorder;
	Image							_magazineLowerBorder;
	Image							_emptyAmmunition;
	uint16							_usedBulletsCount;
	bool							_isReloading;
};

