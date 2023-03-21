#pragma once

#include "pch.h"

class Bullet;
class MapTool;
class PlayerController;
class MonsterController;
class CollisionHandler
{
public:
	static void			setMapToolPtr(MapTool* mapToolPtr) { MapToolPtr = make_unique<MapTool*>(mapToolPtr); }
	static void			setPlayerCtrPtr(PlayerController* playerCtrPtr) { PlayerCtrPtr = make_unique<PlayerController*>(playerCtrPtr); }
	static void			setMonsterCtrPtr(MonsterController* monsterCtrPtr) { MonsterCtrPtr = make_unique<MonsterController*>(monsterCtrPtr); }

	static bool			checkWallCollision(const POINT& currPos, const POINT& userSize, const POINT& tileSize, POINT& moveCount);
	static bool			checkWallCollisionForBullet(Bullet* firedBullet = nullptr);
	static bool			checkBulletCollisionToMonsters(Bullet* firedBullet = nullptr);
	static bool			checkBulletCollisionToPlayer(Bullet* firedBullet = nullptr);
	 
	static void			release();

private:
	static bool			isPointInCircle(const POINT& circleCenterPos, const POINT& circleRadius, const FPOINT& squarePos);

private:
	static				unique_ptr<MapTool*>					MapToolPtr;
	static				unique_ptr<PlayerController*>			PlayerCtrPtr;
	static				unique_ptr<MonsterController*>			MonsterCtrPtr;
};

