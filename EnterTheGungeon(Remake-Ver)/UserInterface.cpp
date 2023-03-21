#include "UserInterface.h"
#include "ScreenManager.h"

void UserInterface::init(const FPOINT& scale, const POINT& startLocation, const POINT& playerSP, const WEAPON_NAME& selectedWeaponName, const uint16 bulletN, const int16 playerHP)
{
	const auto& screenManager = ScreenManager::getInstance();
	const auto& subResolution = screenManager.getSubResolution();

	const POINT& heartsSP{ startLocation.x + 5, startLocation.y + 15 };
	for (int i = 0; i < playerHP / 2; i++)
	{
		POINT heartSize{ 50, 150 };
		POINT heartSP{ (heartsSP.x + (heartSize.x * i) + (i * 10)) * scale.x, heartsSP.y * scale.y };
		_hearts[i] = ImageManager::makeImage("Images/Player/UI/Heart.bmp", move(heartSP), scale, move(heartSize), { 1, 3 }, true, {}, MagentaRGB);
	}

	const POINT& empsSP{ heartsSP.x, heartsSP.y + _hearts[0].getImagePrintSize().y + 10 };
	for (int i = 0; i < 2; i++)
	{
		POINT empSize{ 50, 50 };
		POINT empSP{ (empsSP.x + (empSize.x * i) + (i * 10)) * scale.x, empsSP.y * scale.y };
		_emps[i] = ImageManager::makeImage("Images/Player/UI/Emp.bmp", move(empSP), scale, move(empSize), true, {}, MagentaRGB);
	}

	const POINT& gunBorderSize{ 200, 130 };
	const POINT& gunBorderSP{ (startLocation.x + subResolution.x) - gunBorderSize.x - 30, (subResolution.y - gunBorderSize.y) - 30 };
	_weaponUIBorder = ImageManager::makeImage("Images/Player/UI/GunBorder.bmp", gunBorderSP, scale, gunBorderSize, true, {}, MagentaRGB);

	initSelectedWeaponImage(scale, selectedWeaponName);

	const POINT loadBorderSP{ playerSP.x - 30, playerSP.y - 30 };
	_loadBorder = ImageManager::makeImage("Images/Player/UI/LoadBorder.bmp", loadBorderSP, scale, { 130, 20 }, true, {}, MagentaRGB);
	_loadBar = ImageManager::makeImage("Images/Player/UI/LoadBar.bmp", { loadBorderSP.x, loadBorderSP.y }, scale, { 5, _loadBorder.getImagePrintSize().y }, true, {}, MagentaRGB);

	const POINT& magazineBorderSize{ 15, 15 };
	const POINT& magazineBorderUpperSP{ gunBorderSP.x + gunBorderSize.x + 5, gunBorderSP.y - 30 };
	_magazineUpperBorder = ImageManager::makeImage("Images/Player/UI/MagazineBorder.bmp", magazineBorderUpperSP, scale, magazineBorderSize, true, {}, MagentaRGB);
	POINT magazineBorderLowerSP{ gunBorderSP.x + gunBorderSize.x + 5, magazineBorderUpperSP.y + 145 };
	_magazineLowerBorder = ImageManager::makeImage("Images/Player/UI/MagazineBorder.bmp", move(magazineBorderLowerSP), scale, magazineBorderSize, true, { false, true }, MagentaRGB);

	POINT emptyAmmunitionSize{ 60, 30 };
	POINT emptyAmmunitionSP{ magazineBorderUpperSP.x - emptyAmmunitionSize.x - 10, magazineBorderUpperSP.y - 3 };
	_emptyAmmunition = ImageManager::makeImage("Images/Player/UI/EmptyBullet.bmp", move(emptyAmmunitionSP), scale, move(emptyAmmunitionSize), true, {}, MagentaRGB);

	const POINT& magazineSize{ 15, 20 };
	const POINT& magazineSP{ magazineBorderUpperSP.x, magazineBorderUpperSP.y + magazineSize.y };
	_bulletImagesVec.resize(bulletN);
	for (int i = 0; i < _bulletImagesVec.size(); i++)
	{
		POINT bulletSP{ magazineSP.x, magazineSP.y + 1 + (magazineSize.y * i) + (i * 1) };
		_bulletImagesVec[i] = ImageManager::makeImage("Images/Player/UI/Ammunition.bmp", move(bulletSP), scale, magazineSize, { 1, 2 }, true, {}, MagentaRGB);
	}

	setReloading(false);

	_usedBulletsCount = 0;
}

void UserInterface::update()
{
	if (isReloading())
	{
		moveLoadBar();
	}
}

void UserInterface::render(const HDC& hdc) const
{
	for (const auto& heart : _hearts)
		heart.renderFrame(hdc, {});

	for (const auto& emp : _emps)
		emp.render(hdc, {});

	_weaponUIBorder.render(hdc, {});
	_selectedWeaponUI.renderFrame(hdc, {});

	_emptyAmmunition.render(hdc, {});

	_magazineUpperBorder.render(hdc, {});

	for (const auto& bulletImage : _bulletImagesVec)
		bulletImage.renderFrame(hdc, {});

	_magazineLowerBorder.render(hdc, {});

	if (isReloading())
	{
		_loadBorder.render(hdc, {});
		_loadBar.render(hdc, {});
	}
}

void UserInterface::release()
{}

void UserInterface::setHearts()
{
	static uint16 currIdx = 2;
	static uint16 damage = 1;
	if (currIdx == 0 && damage == 3)
		return;

	if (damage < 3)
	{
		_hearts[currIdx].setCurrentKeyFrameY(damage);
		++damage;
	}
	else
	{
		damage = 1;
		--currIdx;
	}
}

void UserInterface::useBulletImage()
{
	if (_bulletImagesVec.size() <= _usedBulletsCount)
		assert(false);

	_bulletImagesVec[_usedBulletsCount].setCurrentKeyFrameY(1);
	++_usedBulletsCount;
}

void UserInterface::initSelectedWeaponImage(FPOINT scale, WEAPON_NAME weaponName)
{
	const char* source{};
	switch (weaponName)
	{
	case WEAPON_NAME::PISTOL:
		source = { "Images/Player/UI/Pistols.bmp" };
		break;
	}

	POINT pistolSize{ 120, 180 };
	POINT pistolSP{ _weaponUIBorder.getStartPosition().x + (_weaponUIBorder.getImagePrintSize().x / 2) - (pistolSize.x / 2), (_weaponUIBorder.getStartPosition().y + (_weaponUIBorder.getImagePrintSize().y / 2)) - ((pistolSize.y / 3) / 2) };
	_selectedWeaponUI = ImageManager::makeImage(source, move(pistolSP), move(scale), move(pistolSize), { 1, 3 }, true, {}, MagentaRGB);
}

void UserInterface::moveLoadBar()
{
	static const int destX{ _loadBorder.getStartPosition().x + _loadBorder.getImagePrintSize().x };
	static const int loadBarSizeX{ _loadBar.getImagePrintSize().x };

	const int nextPositionX{ _loadBar.getStartPosition().x + loadBarSizeX };
	switch (nextPositionX < destX)
	{
	case true:
		_loadBar.setStartPosition({ nextPositionX, _loadBar.getStartPosition().y });
		break;
	case false:
		_loadBar.setStartPosition(_loadBorder.getStartPosition());
		setReloading(false);
		fillMagazine();
		break;
	}
}

void UserInterface::fillMagazine()
{
	for (int i = 0; i < _usedBulletsCount; i++)
	{
		_bulletImagesVec[i].setCurrentKeyFrameY(0);
	}

	_usedBulletsCount = 0;
}