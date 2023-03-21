#pragma once

#include "MonsterAbstractProduct.h"

class MonsterAbstractFactory
{
protected:
	virtual unique_ptr<MonsterAbstractProduct*> createMonsterAbstractProduct() const = 0;
	~MonsterAbstractFactory() {};
};

class MapTool;
class EnemyBullet;
class EnemyShotgun;
class EnemyShotgun2;
class EnemyFactory : public MonsterAbstractFactory
{
public:
	EnemyBullet* createBullet(MapTool* mapToolPtr, FPOINT scale, POINT locationSP, POINT currPos, POINT currIdx) const;

private:
	unique_ptr<MonsterAbstractProduct*> createMonsterAbstractProduct() const override
	{
		return move(make_unique<MonsterAbstractProduct*>(new EnemyProduct()));
	}
};

class BossFactory : public MonsterAbstractFactory
{
public:

private:
	unique_ptr<MonsterAbstractProduct*> createMonsterAbstractProduct() const override
	{
		return move(make_unique<MonsterAbstractProduct*>(new BossProduct()));
	}

};

