#include "BulletManager.h"
#include "TrigonometricController.h"
#include "KeyController.h"
#include "CollisionHandler.h"
#include "MapTool.h"

void BulletManager::init(FPOINT scale, POINT sp, const uint16 bulletN, const uint16 speed)
{
	_maxBulletN = bulletN;
	_bulletSpeed = speed;

	Bullet tempBullet{ move(scale), sp, _bulletSpeed };
	_objectPoolingMgr.init(tempBullet, 30);

	_bulletMagazine.reserve(bulletN);
	fillMagazine(sp);

	_firedBulletPtrsList.clear();

	_isStop = false;
}

void BulletManager::update()
{
	if (KeyController::isOnceKeyUp(VK_Qkey))
	{
		_isStop = _isStop == true ? false : true;
	}

	if (_isStop == false)
	{
		for (list<Bullet*>::iterator iter = _firedBulletPtrsList.begin(); iter != _firedBulletPtrsList.end();)
		{
			(*iter)->update();
			if (CollisionHandler::checkWallCollisionForBullet((*iter)))
			{
				recycleBullet(iter);
				continue;
			}

			if (CollisionHandler::checkBulletCollisionToMonsters((*iter)))
			{
				recycleBullet(iter);
				continue;
			}

			if (CollisionHandler::checkBulletCollisionToPlayer((*iter)))
			{
				recycleBullet(iter);
				continue;
			}

			++iter;
		}
	}
}

void BulletManager::render(const HDC& hdc) const
{
	for (const auto& usingBullet : _firedBulletPtrsList)
	{
		usingBullet->render(hdc);
	}
}

void BulletManager::release()
{
	_objectPoolingMgr.release();
	_firedBulletPtrsList.clear();
}

void BulletManager::setFiredBulletPtrsPos(POINT pos)
{
	for (auto& firedBullet : _firedBulletPtrsList)
	{
		firedBullet->setRelativePos(move(pos));
		int a = 5;
	}
}

void BulletManager::setFiredBulletPtrsPosAndIdx(const POINT& movedCount, const POINT& distance)
{
	for (auto& firedBullet : _firedBulletPtrsList)
	{
		firedBullet->setRelativePos(move(POINT{ firedBullet->getRelativePos().x - movedCount.x, firedBullet->getRelativePos().y - movedCount.y }));
		firedBullet->setCurrPos(move((*_mapToolPtr)->convertCurrPos(firedBullet->getRelativePos(), distance)));
		firedBullet->setCurrIdx(move((*_mapToolPtr)->convertPosToIndex(firedBullet->getCurrPos(), firedBullet->getRadius())));
	}
}

bool BulletManager::fire(POINT locationSP, const int16 currentAngle)
{
	if (isBulletEmpty())
		return false;

	Bullet* selectedBulletPtr = _bulletMagazine[_bulletMagazine.size() - 1];
	_bulletMagazine.pop_back();
	Bullet& selectedBullet = *selectedBulletPtr;
	if (locationSP.x != 0 && locationSP.y != 0)
		selectedBullet.setRelativePos(move(locationSP));

	if (selectedBullet.isFire())
	{
		assert(false);
	}

	selectedBullet.setCurrPos(move((*_mapToolPtr)->convertCurrPos(selectedBullet.getRelativePos(), (*_mapToolPtr)->getTileDistance())));
	selectedBullet.setCurrIdx((*_mapToolPtr)->convertPosToIndex(selectedBullet.getCurrPos(), selectedBullet.getRadius()));
	selectedBullet.setFire(true);
	selectedBullet.setAngle(TrigonometricController::getDegreeToRadian(currentAngle));

	_firedBulletPtrsList.emplace_back(selectedBulletPtr);

	return true;
}

void BulletManager::fillMagazine(const POINT& sp)
{
	for (int i = _bulletMagazine.size(); i < _maxBulletN; i++)
	{
		_bulletMagazine.emplace_back(_objectPoolingMgr.getObjectFromPools());
		_bulletMagazine[i]->setRelativePos(sp);
	}
}

void BulletManager::recycleBullet(list<Bullet*>::iterator& usingIter)
{
	(*usingIter)->reset();
	_objectPoolingMgr.returnObject((*usingIter));
	usingIter = move(_firedBulletPtrsList.erase(usingIter));
}