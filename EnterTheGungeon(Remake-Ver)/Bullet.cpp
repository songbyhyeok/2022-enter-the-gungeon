#include "Bullet.h"
#include "Debugging.h"

void Bullet::init(FPOINT scale, POINT launchLocation, const uint16 bulletSpeed)
{
	_bullet = ImageManager::makeImage("Images/Weapons/Bullet.bmp", move(launchLocation), move(scale), { 21, 21 }, true, {}, MagentaRGB);
	_currPos = {};
	_currIdx = {};
	_angle = 0.0f;
	_radius = { getSize().x / 2, getSize().y / 2 };
	_speed = bulletSpeed;
	setFire(false);
}

void Bullet::update()
{
	if (isFire())
	{
		POINT updatePos{ getRelativePos().x + _speed * cosf(_angle), getRelativePos().y - _speed * sinf(_angle) };
		setRelativePos(move(updatePos));
		//Debugging::setPrintValue(_currentBulletTileIdx);
	}
}

void Bullet::render(const HDC& hdc) const
{
	if (isFire())
	{
		_bullet.render(hdc, {});
		//ImageManager::renderRectBy(hdc, _rect);
		//Debugging::renderPoint(hdc, "µ¸¿ò", 40, { 2300, 400 }, WhiteRGB, true);
	}
}

void Bullet::release()
{
}

void Bullet::reset()
{
	_bullet.setStartPosition({});
	_angle = 0.0f;
	setFire(false);
}
