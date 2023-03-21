#include "MonsterController.h"
#include "Monster.h"
#include "MapTool.h"
#include "PlayerController.h"
#include "AStar.h"
#include "KeyController.h"

void MonsterController::init()
{
	_createdMonsterPtrsVecMap.clear();
	_monsterPtrsVec.clear();
	_deadMonstersCountMap.clear();
}

void MonsterController::update()
{
	if ((*_playerCtrPtr)->getPlayerCurrState() != PLAYER_STATE::DEATH)
	{
		for (auto& monster : _monsterPtrsVec)
		{
			if (monster->getState() != MONSTER_STATE::DEATH)
			{
				if (!monster->isPlayerFound((*_playerCtrPtr)->getCurrIdx()))
				{
					if (monster->getState() == MONSTER_STATE::IDLE)
						monster->setPathVec(move((*_aStarPtr)->findPath(monster->getCurrIdx(), (*_playerCtrPtr)->getCurrIdx())));
				}
				else
				{
					monster->setPathVec({});
					monster->setStateAndAnimation(MONSTER_STATE::IDLE);
					monster->lookAt((*_playerCtrPtr)->getCurrIdx());
				}
			}

			monster->moveToPath();
			monster->update();
		}
	}
}

void MonsterController::render(const HDC& hdc) const
{
	for (const auto& monster : _monsterPtrsVec)
	{
		monster->render(hdc);
		if (!monster->isDead())
			(*_aStarPtr)->renderPath(hdc, monster->getPathVec());
	}
}

void MonsterController::release()
{	
	for (auto& monstersPtrVecMap : _createdMonsterPtrsVecMap)
	{
		for (auto& monster : monstersPtrVecMap.second)
		{
			monster->release();
			delete(monster);
		}
	}

	_createdMonsterPtrsVecMap.clear();
	_monsterPtrsVec.clear();
}

void MonsterController::setMonstersPosAndIdx() const
{	
	for (const auto& monster : _monsterPtrsVec)
	{
		monster->setCurrPos({ monster->getRect().left, monster->getRect().top });
		monster->setCurrIdx(move((*_mapToolPtr)->convertPosToIndex(monster->getStartPos(), monster->getSize())));
		monster->setRelativePos(move((*_mapToolPtr)->convertCurrPosToRelativePos(monster->getCurrPos())));
		monster->setRelativeHandPosition(monster->getCurrReverseData().x);
	}
}

void MonsterController::setFiredBulletPtrsPosAndIdx(const POINT& movedCount, const POINT& distance) const
{
	for (auto& monster : _monsterPtrsVec)
	{
		monster->setFiredBulletPtrsPosAndIdx(movedCount, distance);
	}
}

bool MonsterController::isMonstersDied(const int16 useZone)
{
	if (_monsterPtrsVec.size() == _deadMonstersCountMap[useZone] ||
		_monsterPtrsVec.size() == 0)
		return true;

	return false;
}

void MonsterController::takeMonsterDamage(Monster* selectedMonsterPtr, const int16 damage, const int16 useZoneN)
{
	if (selectedMonsterPtr->injured(damage))
	{
		addCreatedMonstersVecMap(selectedMonsterPtr, useZoneN);
		++_deadMonstersCountMap[useZoneN];
	}
}



