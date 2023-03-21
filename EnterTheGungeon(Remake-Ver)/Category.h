#pragma once

#include "Scroll.h"
#include "Monster.h"

enum class MATERIAL
{
	NONE,
	TERRAIN,
	OBJECT,
	DECORATIONS,
	ENEMY,
	BOSS,
	UTILITY,
	END
};

enum class OBJECT_NAME
{
	NONE,
	DOOR,
	GATE,
	TABLE,
	TREASURE_BOX,
	POTION_MACHINE,
	RAMPART,
	EMPTY_SCULPTURE,
	ELEVATOR,
	STORE,
	WAITING_ROOM,
	WALL,
	LIBRARY_WALL,
	SMALL_TILESET,
	TILESET,
	BOOS_MAP,
};

struct ARROW_BUTTONS
{
	Button													up;
	Button													down;
};

struct ICON_INFORMATION
{
	POINT													location;
	POINT													keyFrame;
	POINT													sizeCount;
	POINT													size;
	POINT													printSize;
	OBJECT_NAME												objectName;
	MATERIAL												material;
	BYTE													alpha;	
};

constexpr uint16 CreatedMonsterCount = 99;
struct MONSTER_INFORMATION
{
	array<ENEMY_NAME, CreatedMonsterCount>					createdEnemyNames;
	array<BOSS_NAME, CreatedMonsterCount>					createdBossNames;
	POINT													locationIdx;	
	uint16													createdEnemyCount;
	uint16													createdBossCount;
	bool													isActivated;
};

struct TILES_INFORMATION
{
	array<ICON_INFORMATION, 5>								iconInfos;	
	MONSTER_INFORMATION										monsterInfo;
	RECT													rect;
	int16													zoneN;
	bool													isVisitedZone;
};

enum class CATEGORY_TYPE
{
	NONE,
	TILES,
	DECORATIONS,
	ENEMYS,
	BOSSES,
	OBJECTS,
	UTILITIES,
	END
};

class TileInformationHandler
{
public:
	static int16						toInt(MATERIAL material) { return static_cast<uint16>(material) - 1; }
	static vector<ICON_INFORMATION>		generateIconDataVecByCategoryType(const CATEGORY_TYPE& type, const POINT& tileSize);
};

#include "BaseInterface.h"

class MapTool;
class Inventory;
class Palette;
class Category : BaseInterface
{
public:
	void								init(uint16 bottomStartPointY, POINT inventorySP, POINT categorySP, POINT inventorySize, FPOINT scale, CATEGORY_TYPE categoryType);
	void								update() override;
	void								render(const HDC& hdc) const override;
	void								release() override;

	void								setMapToolPtr(const shared_ptr<MapTool*>& mapToolPtr) { _mapToolPtr = mapToolPtr; }
	void								setInventoryPtr(Inventory* inventoryPtr) { _inventoryPtr = make_unique<Inventory*>(inventoryPtr); }
	void								setPalettePtr(const shared_ptr<Palette*>& palettePtr) { _palettePtr = palettePtr; }

	void								setIconMatrix(POINT matrix);
	void								setIconInfoTransparency(uint16 idx, BYTE alpha) { _iconInfosVec[idx].alpha = alpha; }
	void								setEnabled(bool value) { _isActivated = value; }
	bool								getActivatedState() const { return _isActivated; }

	void								reset(uint16 bottomStartPointY, POINT inventorySP, POINT categorySP, POINT inventorySize, FPOINT scale);
	void								moveLines(uint16 sectionY);

	static uint16						toInt(CATEGORY_TYPE categoryType) { return static_cast<uint16>(move(categoryType)) - 1; }
	static CATEGORY_TYPE				toCategory(uint16 idx) { return static_cast<CATEGORY_TYPE>(idx); }
	static const char*					toCategoryName(CATEGORY_TYPE category);
	static POINT						toMatrix(CATEGORY_TYPE category);

private:
	void								resetIconButtons(FPOINT scale);
	void								convertMatrixText(const POINT& matrix);
	int16								generateInvisibleIconCount(const int gridStartLocationY);

private:
	vector<Button>						_iconBtnsVec;
	Scroll								_scroll;
	ARROW_BUTTONS						_leftArrowButtons;
	ARROW_BUTTONS						_rightArrowButtons;
	Button								_categoryBtn;
	TextHandler							_categoryTitle;
	TextHandler							_matrixTextHandler;
	vector<ICON_INFORMATION>			_iconInfosVec;
	shared_ptr<MapTool*>				_mapToolPtr;
	shared_ptr<Palette*>				_palettePtr;
	unique_ptr<Inventory*>				_inventoryPtr;
	POINT								_matrix;
	POINT								_inventorySP;
	POINT								_inventorySize;
	POINT								_iconSpacing;
	CATEGORY_TYPE						_type;
	uint16								_startIdx;
	uint16								_endBorderLineY;
	char								_matrixText[8];
	bool								_isActivated;
};
