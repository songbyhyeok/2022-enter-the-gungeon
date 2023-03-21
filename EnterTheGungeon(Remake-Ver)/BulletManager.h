#pragma once

#include "ObjectPoolingManager.h"
#include "Bullet.h"

class MapTool;
class BulletManager : BaseInterface
{
public:
	void									init(FPOINT scale, POINT sp, const uint16 bulletN, const uint16 speed);
	void									update() override;
	void									render(const HDC& hdc) const override;
	void									release() override;

	void									setMapToolPtr(shared_ptr<MapTool*> mapToolPtr) { _mapToolPtr = mapToolPtr; }

	void									setFiredBulletPtrsPos(POINT pos);
	void									setFiredBulletPtrsPosAndIdx(const POINT& movedCount, const POINT& distance);
	int16									getCurrentFiredBulletCount() const { return _bulletMagazine.size(); }

	bool									isBulletEmpty() const { return _bulletMagazine.empty() ? true : false; }
	bool									fire(POINT locationSP, const int16 currentAngle);

	void									fillMagazine(const POINT& sp);
	void									patternHeart();
	
private:
	void									recycleBullet(list<Bullet*>::iterator& usingIter);

private:
	ObjectPoolingManager<Bullet>			_objectPoolingMgr;
	vector<Bullet*>							_bulletMagazine;
	list<Bullet*>							_firedBulletPtrsList;
	shared_ptr<MapTool*>					_mapToolPtr;
	uint16									_maxBulletN;
	uint16									_bulletSpeed;
	bool									_isStop;
};

