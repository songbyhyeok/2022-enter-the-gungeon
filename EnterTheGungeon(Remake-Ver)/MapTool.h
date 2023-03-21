#pragma once

#include "BaseInterface.h"
#include "TrigonometricController.h"
#include "Inventory.h"

enum class PALETTE_STATE
{
	SELECTED,
	MOVEMENT,
	DRAGGING,
	ERASER,
	PULL,
	MAGNIFIER,
	MATERIAL,
	ZONE,
	END
};

enum class SELECTED_STATE
{
	CLICKABLE,
	INSTALLABLE,
	UNINSTALLABLE,
};

enum class INSTALLATION_COLOR
{
	GREEN,
	YELLOW,
	RED,
	BLUE,
	CORAL,
	GRAY,
};

enum class FILE_MODE
{
	NONE,
	SAVE,
	LOAD
};

constexpr uint16														PaletteImagesCount = 8;
constexpr uint16														InstallationColorCount = 6;

constexpr POINT															TileSize{ 70, 70 };
constexpr POINT															MaxTileEA{ 600, 600 };
constexpr POINT															CanvasKeyFrame{ 100, 100 };
constexpr POINT															WhiteTile{ 99, 99 };
constexpr POINT															BlackTile{ 19, 0 };
constexpr POINT															EmptyTile{ MaxTileEA.x - 1, MaxTileEA.y - 1 };

class MapTool;
class Palette : BaseInterface
{
public:
	void																init(POINT startSP, FPOINT scale, const POINT& tileSize);
	void																update() override;
	void																render(const HDC& hdc) const override;
	void																release() override {}

	void																setMapToolPtr(MapTool* mapToolPtr) { _mapToolPtr = make_unique<MapTool*>(mapToolPtr); }
	void																setInventoryPtr(Inventory* inventoryPtr) { _inventoryPtr = make_unique<Inventory*>(inventoryPtr); }

	void																setInstallationColor(uint16 keyFrameY) { _installationColor.setCurrentKeyFrameY(keyFrameY); }
	const INSTALLATION_COLOR&											getInstallationColor() const { return static_cast<INSTALLATION_COLOR>(_installationColor.getCurrentKeyFrame().y); }
	void																setSelectedStateKeyFrameY(uint16 keyFrameY) { _toolImages[toInt(PALETTE_STATE::SELECTED)].setCurrentKeyFrameY(keyFrameY); }
	const SELECTED_STATE&												getSelectedState() const { return static_cast<SELECTED_STATE>(_toolImages[toInt(PALETTE_STATE::SELECTED)].getCurrentKeyFrame().y); }
	const PALETTE_STATE&												getCurrentState() const { return _currentState; }
	void																setIconInformation(ICON_INFORMATION iconInformation, Inventory& inventory);
	void																setIdleState();
	void																setMovementState();
	void																setDraggingState();
	void																setEnable(bool value) { _isActivated = value; }
	bool																getActivatedState() const { return _isActivated; }
	const ICON_INFORMATION&												getIconInfo() const { return _iconInfo; }
	void																setInstallationTilesCount(POINT count) { _draggedTilesCount = move(count); }

	void																reset(POINT startSP, FPOINT scale, POINT tileSize);
	void																initSelectedIconInfo() { _iconInfo = {}; }
	void																modifyToFitCurrentState(const POINT& canvasSP, const POINT& canvasEP, const POINT& tileStartPointIdx, POINT currentIdx, const POINT& currentTileCoordinates);
	void																handleTiles(array<array<TILES_INFORMATION, MaxTileEA.y>, MaxTileEA.x>& tileInfos, const POINT& currentIdx);
	void																clearInstallationPointVec() { _installationPointVec.clear(); }	
	void																selectTool(PALETTE_STATE paletteST);
	void																moveMovementStateToDifferentState();

	static const uint16													toInt(PALETTE_STATE paletteST) { return static_cast<uint16>(paletteST); }
	static const uint16													toInt(SELECTED_STATE selectedST) { return static_cast<uint16>(selectedST); }
	static const uint16													toInt(INSTALLATION_COLOR installationColor) { return static_cast<uint16>(installationColor); }

private:
	void																setInstallableCondition();
	void																setUnInstallableCondition();
	void																setCurrentState(PALETTE_STATE paletteState) { _currentState = move(paletteState); }
	void																setTileToMaterialOrTerrain(TILES_INFORMATION& selectedTileInfo);
	void																setTileToZone(TILES_INFORMATION& selectedTileInfo, int16 zoneN) { selectedTileInfo.zoneN = zoneN; }

	void																renderInstallationColor(const HDC& hdc) const;
	void																drawTiles(const POINT& currentIdx);
	void																dropIconInformationOnTiles(array<array<TILES_INFORMATION, MaxTileEA.y>, MaxTileEA.x>& tileInfos);
	void																eraseTiles(TILES_INFORMATION& selectedTileInfo);
	
private:
	array<Image, PaletteImagesCount>									_toolImages;
	Image																_installationColor;
	ICON_INFORMATION													_iconInfo;
	vector<POINT>														_installationPointVec;
	vector<POINT>														_draggedPointVec;
	unique_ptr<MapTool*>												_mapToolPtr;
	unique_ptr<Inventory*>												_inventoryPtr;
	FPOINT																_scale;
	POINT																_installationCount;
	POINT																_draggedSPIdx;
	POINT																_draggedTilesCount;
	POINT																_symbol;
	PALETTE_STATE														_currentState;
	int16																_currentZoneN;
	bool																_isActivated;
};

constexpr uint16														GammaValue = 11;

class PlayerController;
class MonsterController;
class MapTool : BaseInterface
{
public:
	void																init(const FPOINT& scale, POINT startPosition);
	void																update() override;
	void																render(const HDC& hdc) const override;
	void																release() override;

	void																setCanvasGamma(const BYTE gamma) { _canvas.setTransparency(gamma); }
	void																setCurrentAreaGamma(const uint16 gammaCount) { _currentAreaGamma.setTransparency(255 - GammaValue * gammaCount); }
	void																setPlayerControllerPtr(PlayerController* playerCtrPtr) { _playerCtrPtr = make_unique<PlayerController*>(playerCtrPtr); }
	void																setMonsterControllerPtr(MonsterController* monsterCtrPtr) { _monsterCtrPtr = make_unique<MonsterController*>(monsterCtrPtr); }
	uint16																getMapAngleByMouse();
	uint16																getMapPreciseAngleByMouse();
	POINT																getMaxMapDistance() { return { _tileInfos[MaxTileEA.y - 1][MaxTileEA.x - 1].rect.left, _tileInfos[MaxTileEA.y - 1][MaxTileEA.x - 1].rect.top }; }
	const POINT&														getTileSize() const { return _tileSize; }
	const POINT&														getStartLocation() const { return _startLocation; }
	const POINT&														getDestLocation() const { return _destLocation; }
	const POINT&														getTileDistance() const { return _distance; }
	POINT																getMaxPrintCount() const { return { _maxPrintCount.x + 1, _maxPrintCount.y + 1 }; }
	RECT																getTileRect(const POINT& idx) const { return _tileInfos[idx.y][idx.x].rect; }
	int16																getCurrentUseZoneN() const { return _currentUseZoneN; }
	const POINT&														getStartTile() const { return _tileStartIdx; }
	void																setGammaMode(bool gammaMode) { _gammaMode = gammaMode; }

	bool																isCurrentPositionMaterialObject(const POINT& selectedIdx) const;
	bool																isGateObject(const POINT& selectedIdx);
	bool																isGammaMode() const { return _gammaMode; }
	bool																canGo(const POINT& selectedIdx) const;

	void																initMap(FPOINT scale, POINT startPosition, POINT subResolution, POINT halfSubResolution, bool isMapLoadingSupported);
	void																reset(POINT startPosition, FPOINT scale);
	void																resetMap(POINT startPosition, POINT subResolution, POINT halfSubResolution, FPOINT scale);
	void																updateMapForDrawing();
	void																updateMapForPlaying(POINT movedCount);
	void																renderMap(const HDC& hdc) const;
	void																renderToSpecifiedLocation(const HDC& hdc, const POINT& dest, const POINT& keyFrame, const POINT& size, const POINT& printSize, const BYTE alpha);
	void																releaseMap();
	void																clearCanvas();
	void																drawTiles(ICON_INFORMATION& iconInfo, const POINT& currentIdx, const POINT& endIdx, const int16 matIdx);
	void																handleFile(const FILE_MODE& fileMode);
	POINT																convertPosToIndex(const POINT& pos, const POINT& size) const { return { (pos.x + size.x / 2) / _tileSize.x, (pos.y + size.y / 2) / _tileSize.y }; }
	POINT																convertIndexToPos(const POINT& idx) const { return { (idx.x * _tileSize.x), (idx.y * _tileSize.y)}; }
	POINT																convertIndexToCalcPos(const POINT& idx) const { return { (idx.x * _tileSize.x) - _distance.x, (idx.y * _tileSize.y) - _distance.y }; }
	POINT																convertCurrPos(const POINT& pos, const POINT& distance) const { return { pos.x + distance.x, pos.y + distance.y }; }
	POINT																convertCurrPosToRelativePos(const POINT& currPos) const { return { currPos.x - _distance.x, currPos.y - _distance.y }; }
	void																openGate();		
	
private:
	void																setCurrentUseZoneN(const int16 selectedZoneN) { _currentUseZoneN = selectedZoneN; }
	void																setExecutedMap(bool executedMap) { _executedMap = executedMap; }
	void																setCurrentZoneEnvironment();
	void																setStartTileForWorld(POINT startTile);
	void																setStartTileForMapTool(const POINT& moveCount);
	void																setEndTileForWorld(const POINT& moveCount);
	void																setEndTileForMapTool(const POINT& moveCount);
	const POINT&														getEndTile() const { return _tileEndIdx; }

	bool																isExecuteMap() const { return _executedMap; }

	void																initImage(POINT canvasSize);
	void																resetImage(POINT printSize);
	void																initTileInfos(bool isMapLoadingSupported);
	void																resetTileInfos(bool isCreateMonsters = true);
	void																initPalette(Inventory& inventory, POINT paletteSP, FPOINT scale);
	void																initInventory(Palette& palette, POINT subResolution, POINT halfSubResolution, FPOINT scale);
	void																saveFile(const char* source);
	void																loadFile(const char* source);
	void																loadTileInfos(bool willLoadMap = true);
	void																moveCanvasPosition();

private:
	Inventory															_inventory;
	Palette																_palette;
	Image																_canvas;
	Image																_currentAreaGamma;
	array<array<TILES_INFORMATION, MaxTileEA.y>, MaxTileEA.x>			_tileInfos;
	TrigonometricController												_triCtr;	
	map<int16, vector<POINT>>											_zonesLocationMap;
	map<int16, bool>													_isVisitedZoneMap;
	unique_ptr<PlayerController*>										_playerCtrPtr;
	unique_ptr<MonsterController*>										_monsterCtrPtr;
	unique_ptr<ICON_INFORMATION*>										_gateObjectPtr;
	FPOINT																_scale;
	POINT																_tileSize;
	POINT																_tileStartIdx;
	POINT																_tileEndIdx;
	POINT																_distance;
	POINT																_maxPrintCount;
	POINT																_startLocation;
	POINT																_destLocation;
	int16																_currentUseZoneN;
	bool																_executedMap;
	bool																_gammaMode;
};

