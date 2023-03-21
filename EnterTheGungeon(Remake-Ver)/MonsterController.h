#pragma once

#include "BaseInterface.h"

class Monster;
class MapTool;
class PlayerController;
class AStar;

class MonsterController : public BaseInterface
{
public:
	void							init();
	void							update() override;
	void							render(const HDC& hdc) const override;
	void							release() override;

	void							setMapToolPtr(MapTool* mapToolPtr) { _mapToolPtr = make_unique<MapTool*>(mapToolPtr); }
	void							setPlayerControllerPtr(PlayerController* playerControllerPtr) { _playerCtrPtr = make_unique<PlayerController*>(playerControllerPtr); }
	void							setAStarPtr(AStar* aStarPtr) { _aStarPtr = make_unique<AStar*>(aStarPtr); }
	
	void							setMonstersPosAndIdx() const;
	void							setFiredBulletPtrsPosAndIdx(const POINT& movedCount, const POINT& distance) const;
	const vector<Monster*>&			getMonsterPtrsVec() const { return _monsterPtrsVec; }

	bool							isMonstersDied(const int16 useZone);

	void							takeMonsterDamage(Monster* selectedMonsterPtr, const int16 damage, const int16 useZoneN);
	void							addCreatedMonstersVecMap(Monster* monsterPtr, const uint16 zone) { _createdMonsterPtrsVecMap[zone].emplace_back(monsterPtr); }
	void							moveCreatedMonsterList(const int16 useZone) { _monsterPtrsVec = move(_createdMonsterPtrsVecMap[useZone]); }
	void							clearMonsterPtrsVec() { _monsterPtrsVec.clear(); }
	
private:

private:
	map<int16, vector<Monster*>>	_createdMonsterPtrsVecMap;
	map<int16, uint16>				_deadMonstersCountMap;	
	vector<Monster*>				_monsterPtrsVec;
	unique_ptr<MapTool*>			_mapToolPtr;
	unique_ptr<PlayerController*>	_playerCtrPtr;
	unique_ptr<AStar*>				_aStarPtr;
};

