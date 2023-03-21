#pragma once

#include "BaseInterface.h"
#include "MapTool.h"
#include "PlayerController.h"
#include "MonsterController.h"
#include "AStar.h"

class World : BaseInterface
{
public:
	void									init(const FPOINT& scale, const POINT& startPosition);
	void									update() override;
	void									render(const HDC& hdc) const override;
	void									release() override;

	void									setCanvasGamma(const BYTE gammaCount) { _mapTool.setCanvasGamma(gammaCount); }
	void									setCurrentAreaGamma(const int16 gammaCount) { _mapTool.setCurrentAreaGamma(gammaCount); }

private:
	MapTool									_mapTool;
	PlayerController						_playerCtr;
	MonsterController						_monsterCtr;
	AStar									_aStar;
};

