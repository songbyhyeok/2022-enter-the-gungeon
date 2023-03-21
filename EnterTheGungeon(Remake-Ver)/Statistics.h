#pragma once

#include "pch.h"

class Statistics
{
public:
	virtual void			setCurrPos(POINT currPos) = 0;
	virtual const POINT&	getCurrPos() const = 0;
	virtual void			setCurrIdx(POINT currIdx) = 0;
	virtual const POINT&	getCurrIdx() const = 0;
	virtual void			setCurrAngle(int16 angle) = 0;
	virtual const int16&	getCurrAngle() const = 0;
	virtual void			setHP(int16 hp) = 0;
	virtual const int16&	getHP() const = 0;
	virtual void			setSpeed(int16 speed) = 0;
	virtual const int16&	getSpeed() const = 0;
	virtual void			setDamage(int16 damage) = 0;
	virtual const int16&	getDamage() const = 0;
	virtual void			setDefense(int16 defense) = 0;
	virtual const int16&	getDefense() const = 0;

	virtual					~Statistics() {};

protected:
	POINT					_currPos;
	POINT					_currIdx;
	int16					_curreAngle;
	int16					_hp;
	int16					_speed;
	int16					_damage;
	int16					_defense;
};

