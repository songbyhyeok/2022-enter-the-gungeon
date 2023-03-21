#include "MonsterAbstractFactory.h"

EnemyBullet* EnemyFactory::createBullet(MapTool* mapToolPtr, FPOINT scale, POINT locationSP, POINT currPos, POINT currIdx) const
{
	unique_ptr<MonsterAbstractProduct*> monsterAbsProductPtr = createMonsterAbstractProduct();
	EnemyProduct* enemyProductPtr = dynamic_cast<EnemyProduct*>((*monsterAbsProductPtr.get()));
	EnemyBullet* enemyBulletPtr = enemyProductPtr->makeBullet();

	delete (*monsterAbsProductPtr.get());
	monsterAbsProductPtr.reset();

	enemyBulletPtr->setMapToolPtr(make_shared<MapTool*>(mapToolPtr));
	enemyBulletPtr->init(move(scale), move(locationSP), {50, 70}, move(currPos), move(currIdx));
	return enemyBulletPtr;
}
