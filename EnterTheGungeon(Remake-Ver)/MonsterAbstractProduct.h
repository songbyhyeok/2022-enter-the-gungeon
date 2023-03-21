#pragma once

#include "EnemyBullet.h"

class MonsterAbstractProduct
{
public:
	virtual ~MonsterAbstractProduct() {};

};

class EnemyProduct : public MonsterAbstractProduct
{
public:
	EnemyBullet* makeBullet() const { return new EnemyBullet(); }

private:
	
};

class BossProduct : public MonsterAbstractProduct
{
public:

};

