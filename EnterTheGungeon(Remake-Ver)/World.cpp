#include "World.h"
#include "CollisionHandler.h"
#include "EffectManager.h"

void World::init(const FPOINT& scale, const POINT& startPosition)
{
	const auto& screenMgr = ScreenManager::getInstance();
	const auto& subResolution = move(screenMgr.getSubResolution());
	const auto& halfSubResolution = move(screenMgr.generateHalfOfResolution(subResolution));

	_monsterCtr.init();
	_monsterCtr.setMapToolPtr(&_mapTool);
	_monsterCtr.setPlayerControllerPtr(&_playerCtr);
	_monsterCtr.setAStarPtr(&_aStar);
	
	_mapTool.setMonsterControllerPtr(&_monsterCtr);
	_mapTool.initMap(scale, startPosition, subResolution, halfSubResolution, true);
	_mapTool.setPlayerControllerPtr(&_playerCtr);
	_monsterCtr.setMonstersPosAndIdx();

	_playerCtr.setMapToolPtr(&_mapTool);
	_playerCtr.init(scale, startPosition);

	_aStar.setMapToolPtr(&_mapTool);
	_aStar.init();

	CollisionHandler::setMapToolPtr(&_mapTool);
	CollisionHandler::setPlayerCtrPtr(&_playerCtr);
	CollisionHandler::setMonsterCtrPtr(&_monsterCtr);

	EffectManager::getInstance().init({ "Sounds/Acoustic/Weapons/BulletSplashing.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::BULLET_SPLASHING) }, scale);	
}

void World::update()
{
	_playerCtr.update();
	_monsterCtr.update();
	EffectManager::getInstance().update();
}

void World::render(const HDC& hdc) const
{
	_mapTool.renderMap(hdc);
	_playerCtr.render(hdc);
	_monsterCtr.render(hdc);

	EffectManager::getInstance().render(hdc);
}

void World::release()
{
	_mapTool.releaseMap();
	_playerCtr.release();
	_monsterCtr.release();

	CollisionHandler::release();
	EffectManager::getInstance().release();
}
