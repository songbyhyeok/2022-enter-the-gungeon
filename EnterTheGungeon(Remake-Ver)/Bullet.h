#pragma once

#include "BaseInterface.h"
#include "ImageManager.h"

class Bullet : BaseInterface
{
public:
	Bullet() = default;
	Bullet(FPOINT scale, POINT launchLocation, const uint16 bulletSpeed)
	{
		init(move(scale), move(launchLocation), bulletSpeed);
	}

	void					init(FPOINT scale, POINT launchLocation, const uint16 bulletSpeed);
	void					update() override;
	void					render(const HDC& hdc) const override;
	void					release() override;

	void					setRelativePos(POINT startPos) { _bullet.setStartPosition(move(startPos)); }
	const POINT&			getRelativePos() const { return _bullet.getStartPosition(); }
	void					setCurrPos(POINT currPos) { _currPos = move(currPos); }
	const POINT&			getCurrPos() const { return _currPos; }
	POINT					getCenterPosition() const { return { _currPos.x + _radius.x, _currPos.y + _radius.y }; }
	const POINT&			getRadius() const { return _radius; }
	void					setCurrIdx(POINT currIdx) { _currIdx = move(currIdx); }
	const POINT&			getCurrIdx() const { return _currIdx; }
	void					setFire(bool isFire) { _isFire = isFire;}
	void					setAngle(double angle) { _angle = angle; }	
	const POINT&			getSize() const { return _bullet.getImagePrintSize(); }

	bool					isFire() const { return _isFire; }

	void					reset();

private:
	
private:
	Image					_bullet;
	POINT					_currPos;
	POINT					_currIdx;
	POINT					_radius;
	double					_angle;
	int16					_speed;
	bool					_isFire;
};

