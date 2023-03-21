#include "MapTool.h"
#include "ImageManager.h"
#include "MouseHandler.h"
#include "ButtonState.h"
#include "Inventory.h"
#include "CollisionHandler.h"
#include "MonsterAbstractFactory.h"

void Palette::init(POINT startSP, FPOINT scale, const POINT& tileSize)
{
	setEnable(true);

	_scale = move(scale);

	_iconInfo = { {0, 0}, {0,0}, {0,0}, {0,0}, {0,0}, OBJECT_NAME::NONE, MATERIAL::NONE, 255 };
	_symbol = { 1,1 };

	const POINT imageSize{ 75, 75 };
	startSP.x = startSP.x - (((imageSize.x * _scale.x) * PaletteImagesCount) / 2);
	const char* imageName{ "" };
	for (int i = 0; i < PaletteImagesCount; i++)
	{
		switch (i)
		{
		case 0:
			imageName = "Images/Scene/Maptool/Palette/Selected.bmp";
			break;
		case 1:
			imageName = "Images/Scene/Maptool/Palette/Movement.bmp";
			break;
		case 2:
			imageName = "Images/Scene/Maptool/Palette/Dragging.bmp";
			break;
		case 3:
			imageName = "Images/Scene/Maptool/Palette/Eraser.bmp";
			break;
		case 4:
			imageName = "Images/Scene/Maptool/Palette/Pull.bmp";
			break;
		case 5:
			imageName = "Images/Scene/Maptool/Palette/Magnifier.bmp";
			break;
		case 6:
			imageName = "Images/Scene/Maptool/Palette/Material.bmp";
			break;
		case 7:
			imageName = "Images/Scene/Maptool/Palette/Zone.bmp";
			break;
		}

		auto& image = _toolImages[i];
		POINT imageSP{ startSP.x + (imageSize.x * i) * _scale.x, startSP.y };
		switch (i != 0)
		{
		case true:
			image = ImageManager::makeImage(imageName, move(imageSP), _scale, imageSize, false);
			break;
		case false:
			image = ImageManager::makeImage(imageName, move(imageSP), _scale, { imageSize.x, imageSize.y * 3 }, { 1, 3 }, false);
			break;
		}

		image.setTransparency(255);
	}

	_installationColor = ImageManager::makeImage("Images/Scene/Maptool/Palette/InstallationColor.bmp", { 0, 0 }, { 1.0f, 1.0f }, { tileSize.x, tileSize.y * InstallationColorCount }, { 1, InstallationColorCount }, false);
	_installationColor.setTransparency(255);
	const auto& maxTilePrintCount = (*_mapToolPtr)->getMaxPrintCount();
	_installationPointVec.reserve(maxTilePrintCount.x * maxTilePrintCount.y);
	_installationCount = { };
	_draggedSPIdx = {};
	_draggedPointVec.reserve(1000);
	_draggedTilesCount = {};

	selectTool(PALETTE_STATE::SELECTED);

	_currentZoneN = 0;
}

void Palette::update()
{
	if (getActivatedState() == true)
	{
		if (KeyController::isOnceKeyUp(VK_Ekey))
		{
			selectTool(PALETTE_STATE::ERASER);
		}

		if (KeyController::isOnceKeyUp(VK_Qkey))
		{
			selectTool(PALETTE_STATE::MATERIAL);
		}

		if (KeyController::isOnceKeyUp(VK_Zkey))
		{
			selectTool(PALETTE_STATE::ZONE);
		}

		if (_currentState == PALETTE_STATE::ZONE)
		{
			if (KeyController::isOnceKeyUp(VK_Xkey))
			{
				--_currentZoneN;
			}
			if (KeyController::isOnceKeyUp(VK_Ckey))
			{
				++_currentZoneN;
			}
		}
	}
}

void Palette::render(const HDC& hdc) const
{
	if (getActivatedState() == true)
	{
		for (const auto& toolImage : _toolImages)
			toolImage.renderTransparent(hdc, {0, 0});

		renderInstallationColor(hdc);

		if (_currentState == PALETTE_STATE::ZONE)
			TextHandler::renderImmediately(hdc, "돋움", _currentZoneN, 40, {500, 500}, WhiteRGB, true);
	}
}

void Palette::setIconInformation(ICON_INFORMATION iconInformation, Inventory& inventory)
{
	_iconInfo = move(iconInformation);
	
	selectTool(PALETTE_STATE::SELECTED);
	setSelectedStateKeyFrameY(Palette::toInt(SELECTED_STATE::INSTALLABLE));
	setInstallationColor(Palette::toInt(INSTALLATION_COLOR::GREEN));
	_installationColor.setTransparency(155);

	inventory.setEnabled(false);
	inventory.setIconSelectMode(true);
}

void Palette::setIdleState()
{
	selectTool(PALETTE_STATE::SELECTED);
	initSelectedIconInfo();
	setInstallationTilesCount({ 0, 0 });
	clearInstallationPointVec();
}

void Palette::setMovementState()
{
	switch (getCurrentState())
	{
	case PALETTE_STATE::SELECTED:
	case PALETTE_STATE::ERASER:
		setInstallationTilesCount({ 0, 0 });
		setInstallationColor(Palette::toInt(INSTALLATION_COLOR::GREEN));
		setSelectedStateKeyFrameY(Palette::toInt(SELECTED_STATE::CLICKABLE));
		selectTool(PALETTE_STATE::MOVEMENT);
		break;
	case PALETTE_STATE::DRAGGING:
		break;
	}
}

void Palette::setDraggingState()
{
	INSTALLATION_COLOR installationColor{};
	switch (getCurrentState())
	{
	case PALETTE_STATE::SELECTED:
		installationColor = INSTALLATION_COLOR::GREEN;
		break;
	case PALETTE_STATE::ERASER:
		installationColor = INSTALLATION_COLOR::YELLOW;
		break;
	case PALETTE_STATE::MATERIAL:
		switch (getInstallationColor())
		{
		case INSTALLATION_COLOR::BLUE:
			installationColor = INSTALLATION_COLOR::BLUE;
			break;
		case INSTALLATION_COLOR::CORAL:
			installationColor = INSTALLATION_COLOR::CORAL;
			break;
		}
		break;
	case PALETTE_STATE::ZONE:
		installationColor = INSTALLATION_COLOR::GRAY;
		break;
	}

	if (getCurrentState() != PALETTE_STATE::DRAGGING)
	{
		selectTool(PALETTE_STATE::DRAGGING);
		setInstallationColor(Palette::toInt(move(installationColor)));
	}
}

void Palette::reset(POINT startSP, FPOINT scale, POINT tileSize)
{
	const POINT& imageSize{ 75, 75 };
	startSP.x = startSP.x - (((imageSize.x * scale.x) * PaletteImagesCount) / 2);

	for (int i = 0; i < PaletteImagesCount; i++)
	{
		POINT imageSP{ startSP.x + (imageSize.x * i) * scale.x, startSP.y };
		ImageManager::resetImage(_toolImages[i], move(imageSP), scale);
	}

	ImageManager::resetImage(_installationColor, {}, move(tileSize));
	const auto& maxTilePrintCount = (*_mapToolPtr)->getMaxPrintCount();
	_installationPointVec.reserve(maxTilePrintCount.x * maxTilePrintCount.y);
	_installationCount = { };
	_draggedSPIdx = {};
	_draggedTilesCount = {};
	_draggedPointVec.reserve(1000);
}

void Palette::modifyToFitCurrentState(const POINT& canvasSP, const POINT& canvasEP, const POINT& tileStartPointIdx, POINT currentIdx, const POINT& currentTileCoordinates)
{
	if (getInstallationColor() == INSTALLATION_COLOR::RED)
	{
		setSelectedStateKeyFrameY(toInt(SELECTED_STATE::INSTALLABLE));
		setInstallationColor(toInt(INSTALLATION_COLOR::GREEN));
	}
	const POINT& tileSize{ _installationColor.getImagePrintSize() };
	_symbol = { 1, 1 };

	static POINT previousTileSPIdx{};
	static POINT clickedInstallationSP{};
	switch (getCurrentState())
	{
	case PALETTE_STATE::SELECTED:
	case PALETTE_STATE::ERASER:
	case PALETTE_STATE::MATERIAL:
	case PALETTE_STATE::ZONE:
		previousTileSPIdx = tileStartPointIdx;
		clickedInstallationSP = currentTileCoordinates;
		_draggedSPIdx = currentIdx;
		break;
	case PALETTE_STATE::DRAGGING:
		const POINT &twoSPIdxDistance{ tileStartPointIdx.x - previousTileSPIdx.x, tileStartPointIdx.y - previousTileSPIdx.y};
		previousTileSPIdx = tileStartPointIdx;
		switch (twoSPIdxDistance.x <= 0)
		{
			// left
		case true:
			clickedInstallationSP.x = clickedInstallationSP.x + ((-twoSPIdxDistance.x) * tileSize.x);
			if (clickedInstallationSP.x >= canvasEP.x)
				clickedInstallationSP.x = canvasEP.x - tileSize.x;
			break;
			// right
		case false:
			clickedInstallationSP.x -= twoSPIdxDistance.x * tileSize.x;
			if (clickedInstallationSP.x < canvasSP.x)
				clickedInstallationSP.x = canvasSP.x;
			break;
		}
		switch (twoSPIdxDistance.y <= 0)
		{
			// left
		case true:
			clickedInstallationSP.y = clickedInstallationSP.y + ((-twoSPIdxDistance.y) * tileSize.y);
			if (clickedInstallationSP.y >= canvasEP.y)
				clickedInstallationSP.y = canvasEP.y - tileSize.y;
			break;
			// right
		case false:
			clickedInstallationSP.y -= twoSPIdxDistance.y * tileSize.y;
			if (clickedInstallationSP.y <= canvasSP.y)
				clickedInstallationSP.y = canvasSP.y;
			break;
		}

		const POINT& clickedInstallationTileSPIdx = { (clickedInstallationSP.x - canvasSP.x) / tileSize.x, (clickedInstallationSP.y - canvasSP.y) / tileSize.y };
		const POINT& clickedInstallationTileEPIdx = { currentIdx.x - tileStartPointIdx.x, currentIdx.y - tileStartPointIdx.y };
		_installationCount = { clickedInstallationTileEPIdx.x - clickedInstallationTileSPIdx.x, clickedInstallationTileEPIdx.y - clickedInstallationTileSPIdx.y };
		switch (_installationCount.x >= 0)
		{
			// 클릭된 위치가 시작 위치와 같거나 더 큰 경우 -> 드래그 설치 방향이 우측인 경우
		case true:
			_installationCount.x = _installationCount.x + 1;
			break;
			// 클릭된 위치가 시작 위치보다 작은 경우 -> 드래그 설치 방향이 좌측인 경우
		case false:
			_installationCount.x = -(_installationCount.x - 1);
			break;
		}
		switch (_installationCount.y >= 0)
		{
			// 클릭된 위치가 시작 위치와 같거나 더 큰 경우 -> 드래그 설치 방향이 하인 경우
		case true:
			_installationCount.y = _installationCount.y + 1;
			break;
			// 클릭된 위치가 시작 위치보다 작은 경우 -> 드래그 설치 방향이 상인 경우
		case false:
			_installationCount.y = -(_installationCount.y - 1);
			break;
		}

		const POINT& clickedDraggedTileSPIdx { _draggedSPIdx.x - tileStartPointIdx.x,  _draggedSPIdx.y - tileStartPointIdx.y };
		const POINT& clickedDraggedTileEPIdx { clickedInstallationTileEPIdx };
		_draggedTilesCount = { clickedDraggedTileEPIdx.x - clickedDraggedTileSPIdx.x, clickedDraggedTileEPIdx.y - clickedDraggedTileSPIdx.y };
		switch (_draggedTilesCount.x >= 0)
		{
		case true:
			_draggedTilesCount.x = _draggedTilesCount.x + 1;
			break;
		case false:
			_draggedTilesCount.x = -(_draggedTilesCount.x - 1);
			_symbol.x = -1;
			break;
		}
		switch (_draggedTilesCount.y >= 0)
		{
		case true:
			_draggedTilesCount.y = _draggedTilesCount.y + 1;
			break;
		case false:
			_draggedTilesCount.y = -(_draggedTilesCount.y - 1);
			_symbol.y = -1;
			break;
		}

		currentIdx = { 0, 0};
		break;
	}

	clearInstallationPointVec();
	for (int i = 0; i < _installationCount.y; i++)
	{
		for (int j = 0; j < _installationCount.x; j++)
		{
			_installationPointVec.emplace_back(move(POINT{ clickedInstallationSP.x + tileSize.x * (j * _symbol.x), clickedInstallationSP.y + tileSize.y * (i * _symbol.y) }));
		}
	}

	const POINT &currentInstallationArea { currentIdx.x + (_installationCount.x - 1), currentIdx.y + (_installationCount.y - 1)};
	if (currentInstallationArea.x >= MaxTileEA.x || currentInstallationArea.y >= MaxTileEA.y)
	{
		setUnInstallableCondition();
	}
}

void Palette::handleTiles(array<array<TILES_INFORMATION, MaxTileEA.y>, MaxTileEA.x>& tileInfos, const POINT& currentIdx)
{
	switch (getCurrentState())
	{
	case PALETTE_STATE::SELECTED:
		drawTiles(currentIdx);
		break;
	case PALETTE_STATE::DRAGGING:
		dropIconInformationOnTiles(tileInfos);
		break;
	case PALETTE_STATE::ERASER:
		eraseTiles(tileInfos[currentIdx.y][currentIdx.x]);
		setInstallableCondition();
		break;
	case PALETTE_STATE::MATERIAL:
		setTileToMaterialOrTerrain(tileInfos[currentIdx.y][currentIdx.x]);
		setInstallableCondition();
		break;
	case PALETTE_STATE::ZONE:
		setTileToZone(tileInfos[currentIdx.y][currentIdx.x], _currentZoneN);
		setInstallableCondition();
	}
}

void Palette::selectTool(PALETTE_STATE paletteST)
{
	assert(paletteST != PALETTE_STATE::END);

	switch (paletteST)
	{
	case PALETTE_STATE::SELECTED:
		setSelectedStateKeyFrameY(toInt(SELECTED_STATE::CLICKABLE));
		setInstallationColor(toInt(INSTALLATION_COLOR::GREEN));
		_installationCount = _iconInfo.sizeCount;
		_installationColor.setTransparency(255);
		break;
	case PALETTE_STATE::DRAGGING:
		setInstallationColor(toInt(INSTALLATION_COLOR::GREEN));
		_installationColor.setTransparency(155);
		break;
	case PALETTE_STATE::ERASER:
		setInstallationColor(toInt(INSTALLATION_COLOR::YELLOW));
		_installationCount = { 1, 1 };
		_installationColor.setTransparency(155);
		break;
	case PALETTE_STATE::MATERIAL:
		switch (getInstallationColor())
		{
		case INSTALLATION_COLOR::BLUE:
			setInstallationColor(toInt(INSTALLATION_COLOR::CORAL));
			break;
		case INSTALLATION_COLOR::CORAL:
			setInstallationColor(toInt(INSTALLATION_COLOR::BLUE));
			break;
		default:
			setInstallationColor(toInt(INSTALLATION_COLOR::BLUE));
			break;
		}
		
		_installationCount = { 1, 1 };
		_installationColor.setTransparency(155);
		break;
	case PALETTE_STATE::ZONE:
		setInstallationColor(toInt(INSTALLATION_COLOR::GRAY));

		_installationCount = { 1, 1 };
		_installationColor.setTransparency(155);
		break;
	}

	_toolImages[toInt(getCurrentState())].setTransparency(255);
	_toolImages[toInt(paletteST)].setTransparency(155);

	setCurrentState(move(paletteST));
}

void Palette::moveMovementStateToDifferentState()
{
	switch (getCurrentState())
	{
	case PALETTE_STATE::MOVEMENT:
		setInstallableCondition();
		break;
	case PALETTE_STATE::DRAGGING:
		break;
	}
}

void Palette::setInstallableCondition()
{
	if ((*_inventoryPtr)->isIconSelectMode())
	{
		selectTool(PALETTE_STATE::SELECTED);
		setSelectedStateKeyFrameY(Palette::toInt(SELECTED_STATE::INSTALLABLE));
		setInstallationColor(toInt(INSTALLATION_COLOR::GREEN));
		_installationColor.setTransparency(155);
	}
}

void Palette::setUnInstallableCondition()
{
	selectTool(PALETTE_STATE::SELECTED);
	setSelectedStateKeyFrameY(toInt(SELECTED_STATE::UNINSTALLABLE));
	setInstallationColor(toInt(INSTALLATION_COLOR::RED));
	_installationColor.setTransparency(155);
}

void Palette::setTileToMaterialOrTerrain(TILES_INFORMATION& selectedTileInfo)
{
	switch (getInstallationColor())
	{
	case INSTALLATION_COLOR::BLUE:
		selectedTileInfo.iconInfos[TileInformationHandler::toInt(MATERIAL::OBJECT)].material = MATERIAL::OBJECT;
		break;
	case INSTALLATION_COLOR::CORAL:
		selectedTileInfo.iconInfos[TileInformationHandler::toInt(MATERIAL::TERRAIN)].material = MATERIAL::TERRAIN;
		break;
	}

	/*for (int i = selectedTileInfo.iconInfos.size() - 1; i >= 0; i--)
	{
		auto& tileIconInfo = selectedTileInfo.iconInfos[i];
		if (tileIconInfo.material != MATERIAL::NONE)
		{
			tileIconInfo.material = selectedMat;
			break;
		}
	}*/
}

void Palette::renderInstallationColor(const HDC& hdc) const
{
	const auto& destLocation = (*_mapToolPtr)->getDestLocation();
	for (const auto& installationPoint : _installationPointVec)
	{
		if (destLocation.x <= installationPoint.x || destLocation.y <= installationPoint.y)
			continue;

		_installationColor.renderTransparentFrame(hdc, installationPoint, {});
	}
}

void Palette::drawTiles(const POINT& currentIdx)
{	
	if (getInstallationColor() == INSTALLATION_COLOR::GREEN)
	{
		const POINT & tileSize = _installationColor.getImagePrintSize();
		const POINT & keyFrameCount = _iconInfo.sizeCount;
		const POINT & endIdx{ currentIdx.x + keyFrameCount.x, currentIdx.y + keyFrameCount.y };
		const int16 matIdx{ TileInformationHandler::toInt(_iconInfo.material) };
		(*_mapToolPtr.get())->drawTiles(_iconInfo, currentIdx, endIdx, matIdx);
	}
}

void Palette::dropIconInformationOnTiles(array<array<TILES_INFORMATION, MaxTileEA.y>, MaxTileEA.x>& tileInfos)
{
	if (_symbol.y == -1)
		_draggedSPIdx.y = (_draggedSPIdx.y - _draggedTilesCount.y) + 1;

	if (_symbol.x == -1)
		_draggedSPIdx.x = (_draggedSPIdx.x - _draggedTilesCount.x) + 1;

	const POINT installationEPIdx{ (_draggedSPIdx.x + _draggedTilesCount.x) - 1, (_draggedSPIdx.y + _draggedTilesCount.y) - 1};
	const int16 matIdx{ TileInformationHandler::toInt(_iconInfo.material) };
	switch (getInstallationColor())
	{
		case INSTALLATION_COLOR::GREEN:
		{			
			if (_iconInfo.material == MATERIAL::TERRAIN && _iconInfo.objectName == OBJECT_NAME::NONE)
			{
				for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
				{
					for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
					{
						tileInfos[i][j].iconInfos[matIdx] = _iconInfo;
					}
				}
			}
			else if (_iconInfo.material == MATERIAL::DECORATIONS && _iconInfo.objectName == OBJECT_NAME::NONE)
			{
				for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
				{
					for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
					{
						tileInfos[i][j].iconInfos[matIdx] = _iconInfo;
					}
				}
			}
			else if (_iconInfo.objectName == OBJECT_NAME::SMALL_TILESET)
			{
				for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
				{
					for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
					{
						POINT calcKeyFrame{ (((j - _draggedSPIdx.x) % 2) == 0) ? _iconInfo.keyFrame.x : _iconInfo.keyFrame.x + 1, (((i - _draggedSPIdx.y) % 2) == 0) ? _iconInfo.keyFrame.y : _iconInfo.keyFrame.y + 1 };
						tileInfos[i][j].iconInfos[matIdx] = _iconInfo;
						tileInfos[i][j].iconInfos[matIdx].keyFrame = move(calcKeyFrame);
					}
				}
			}
			else if (_iconInfo.objectName == OBJECT_NAME::WALL)
			{
				for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
				{
					for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
					{
						POINT calcKeyFrame{ _iconInfo.keyFrame.x, (((i - _draggedSPIdx.y) % 2) == 0) ? _iconInfo.keyFrame.y : _iconInfo.keyFrame.y + 1 };
						tileInfos[i][j].iconInfos[matIdx] = _iconInfo;
						tileInfos[i][j].iconInfos[matIdx].keyFrame = move(calcKeyFrame);
					}
				}
			}
			else if (_iconInfo.objectName == OBJECT_NAME::TILESET)
			{
				for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
				{
					for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
					{
						POINT calcKeyFrame{};
						if (i == _draggedSPIdx.y)
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = _iconInfo.keyFrame;
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 2, _iconInfo.keyFrame.y };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y };
						}
						else if (i == installationEPIdx.y)
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x, _iconInfo.keyFrame.y + 2 };
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 2, _iconInfo.keyFrame.y + 2 };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y + 2 };
						}
						else
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x, _iconInfo.keyFrame.y + 1 };
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 2, _iconInfo.keyFrame.y + 1 };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y + 1 };
						}

						tileInfos[i][j].iconInfos[matIdx] = _iconInfo;
						tileInfos[i][j].iconInfos[matIdx].keyFrame = move(calcKeyFrame);
					}
				}
			}
			else if (_iconInfo.objectName == OBJECT_NAME::RAMPART || _iconInfo.objectName == OBJECT_NAME::LIBRARY_WALL)
			{
				for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
				{
					for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
					{
						POINT calcKeyFrame{};
						if (i == _draggedSPIdx.y)
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = _iconInfo.keyFrame;
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 2, _iconInfo.keyFrame.y };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y };
						}
						else if (i == installationEPIdx.y - 2)
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x, _iconInfo.keyFrame.y + 2 };
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 2, _iconInfo.keyFrame.y + 2 };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y + 2 };
						}
						else if (i == installationEPIdx.y - 1)
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x, _iconInfo.keyFrame.y + 3 };
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 2, _iconInfo.keyFrame.y + 3 };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y + 3 };
						}
						else if (i == installationEPIdx.y)
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x, _iconInfo.keyFrame.y + 4 };
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 2, _iconInfo.keyFrame.y + 4 };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y + 4 };
						}
						else
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x, _iconInfo.keyFrame.y + 1 };
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 2, _iconInfo.keyFrame.y + 1 };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y + 1 };
						}

						tileInfos[i][j].iconInfos[matIdx] = _iconInfo;
						tileInfos[i][j].iconInfos[matIdx].keyFrame = move(calcKeyFrame);
					}
				}
			}
			else if (_iconInfo.objectName == OBJECT_NAME::STORE)
			{
				for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
				{
					for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
					{
						POINT calcKeyFrame{};
						if (i <= _draggedSPIdx.y + 2)
						{
							if (j <= _draggedSPIdx.x + 1)
								calcKeyFrame = {j - _draggedSPIdx.x, (i - _draggedSPIdx.y) + _iconInfo.keyFrame.y};
							else if (_draggedSPIdx.x + 8 <= j && j <= _draggedSPIdx.x + 9)
								calcKeyFrame = { j - _draggedSPIdx.x, (i - _draggedSPIdx.y) + _iconInfo.keyFrame.y };
							else if (j >= installationEPIdx.x - 2 && j <= installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + (_iconInfo.sizeCount.x - 1) + (j - installationEPIdx.x - 2), (i - _draggedSPIdx.y) + _iconInfo.keyFrame.y };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 2, _iconInfo.keyFrame.y};
						}
						else if (_draggedSPIdx.y + 3 <= i && i <= _draggedSPIdx.y + 4)
						{
							if (_draggedSPIdx.x + 8 <= j && j <= _draggedSPIdx.x + 9)
								calcKeyFrame = { j - _draggedSPIdx.x, (i - _draggedSPIdx.y) + _iconInfo.keyFrame.y };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + _iconInfo.sizeCount.x - 1, _iconInfo.keyFrame.y + _iconInfo.sizeCount.y - 1 };
						}
						else if (installationEPIdx.y - 4 <= i && i <= installationEPIdx.y - 2)
						{
							if (j <= _draggedSPIdx.x + 9)
								calcKeyFrame = { j - _draggedSPIdx.x, (_iconInfo.keyFrame.y + _iconInfo.sizeCount.y - 1) - (installationEPIdx.y - i)};
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + _iconInfo.sizeCount.x - 1, (_iconInfo.keyFrame.y + _iconInfo.sizeCount.y - 1) - (installationEPIdx.y - i) };
						}
						else
						{
							calcKeyFrame = { _iconInfo.keyFrame.x + _iconInfo.sizeCount.x - 1, _iconInfo.keyFrame.y + _iconInfo.sizeCount.y - 1 };
						}
					}
				}
			}
			/*else if (_iconInfo.name == "WaitingRoom")
			{
				for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
				{
					for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
					{
						auto& tileInfo = tileInfos[i][j];
						tileInfo.matKeyFrames[matIdx].first = _iconInfo.material;
						if (i == _draggedSPIdx.y)
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = _iconInfo.keyFrame;
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 21, _iconInfo.keyFrame.y };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y };
						}
						else if (i == installationEPIdx.y)
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x, _iconInfo.keyFrame.y + 25 };
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 21, _iconInfo.keyFrame.y + 25 };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y + 25 };
						}
						else
						{
							if (j == _draggedSPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x, _iconInfo.keyFrame.y + 1 };
							else if (j == installationEPIdx.x)
								calcKeyFrame = { _iconInfo.keyFrame.x + 21, _iconInfo.keyFrame.y + 1 };
							else
								calcKeyFrame = { _iconInfo.keyFrame.x + 1, _iconInfo.keyFrame.y + 1 };
						}
					}
				}
			}*/
			/*else if (_iconInfo.name == "BossMap")
			{
				for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
				{
					for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
					{
						auto& tileInfo = tileInfos[i][j];
						tileInfo.matKeyFrames[matIdx].first = _iconInfo.material;
						if (_draggedSPIdx.y + 6 <= i && i <= _draggedSPIdx.y + 21)
						{
							if (_draggedSPIdx.x + 3 <= j && j <= installationEPIdx.x - 2)
								calcKeyFrame = { j - _draggedSPIdx.x, (i - _draggedSPIdx.y) + _iconInfo.keyFrame.y};
							else
								calcKeyFrame = { _iconInfo.keyFrame };
						}
						else
						{
							calcKeyFrame = { _iconInfo.keyFrame };
						}
					}
				}
			}*/
		}
		break;
		case INSTALLATION_COLOR::YELLOW:
			for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
			{
				for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
				{
						eraseTiles(tileInfos[i][j]);
				}
			}
			break;
		case INSTALLATION_COLOR::BLUE:
		case INSTALLATION_COLOR::CORAL:
			for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
			{
				for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
				{
					setTileToMaterialOrTerrain(tileInfos[i][j]);
				}
			}
			break;
		case INSTALLATION_COLOR::GRAY:
			for (int i = _draggedSPIdx.y; i <= installationEPIdx.y; i++)
			{
				for (int j = _draggedSPIdx.x; j <= installationEPIdx.x; j++)
				{
					setTileToZone(tileInfos[i][j], _currentZoneN);
				}
			}
			break;
	}

	selectTool(PALETTE_STATE::SELECTED);
	setInstallableCondition();
}

void Palette::eraseTiles(TILES_INFORMATION & selectedTileInfo)
{
	if (getInstallationColor() == INSTALLATION_COLOR::YELLOW)
	{
		for (auto& iconInfo : selectedTileInfo.iconInfos)
		{
			if (iconInfo.material != MATERIAL::NONE)
			{
				iconInfo = {};
			}
		}

		selectedTileInfo.monsterInfo = {};
		selectedTileInfo.zoneN = -1;
		selectedTileInfo.isVisitedZone = false;		
	}
}

#include "CollisionHandler.h"
#include "PlayerController.h"
#include "MonsterController.h"

void MapTool::init(const FPOINT& scale, POINT startPosition)
{
	const auto& screenMgr = ScreenManager::getInstance();
	const auto& subResolution = move(screenMgr.getSubResolution());
	const auto& halfSubResolution = move(screenMgr.generateHalfOfResolution(subResolution));

	initMap(scale, move(startPosition), subResolution, halfSubResolution, false);
	
	POINT paletteSP{ _startLocation.x + halfSubResolution.x, _startLocation.y + 20 * scale.y };
	initPalette(_inventory, move(paletteSP), scale);
	initInventory(_palette, subResolution, halfSubResolution, scale);

	setGammaMode(false);
}

void MapTool::update()
{
	_palette.update();
	_inventory.update();

	if (_inventory.getActivatedState() == false || _inventory.isIconSelectMode() == true)
	{
		updateMapForDrawing();
	}
}

void MapTool::render(const HDC& hdc) const
{	
	renderMap(hdc);

	//_triCtr.drawCircle(hdc);
	_palette.render(hdc);
	_inventory.render(hdc);
}

void MapTool::release()
{
	_inventory.release();
	_palette.release();
}

uint16 MapTool::getMapAngleByMouse()
{
	if (MouseHandler::Mouse.x < _startLocation.x || MouseHandler::Mouse.x > _destLocation.x)
		return -1;

	if (MouseHandler::Mouse.y < _startLocation.y || MouseHandler::Mouse.y > _destLocation.y)
		return -1;

	return _triCtr.getCurrentAngle();
}

uint16 MapTool::getMapPreciseAngleByMouse()
{
	if (MouseHandler::Mouse.x < _startLocation.x || MouseHandler::Mouse.x > _destLocation.x)
		return -1;

	if (MouseHandler::Mouse.y < _startLocation.y || MouseHandler::Mouse.y > _destLocation.y)
		return -1;

	return _triCtr.getPreciseCurrentAngle();
}

bool MapTool::isCurrentPositionMaterialObject(const POINT& selectedIdx) const
{
	for (const auto& iconInfo : _tileInfos[selectedIdx.y][selectedIdx.x].iconInfos)
	{
		if (iconInfo.material == MATERIAL::OBJECT)
		{
			return true;
		}
	}

	return false;
}

bool MapTool::isGateObject(const POINT& selectedIdx)
{
	for (auto& iconInfo : _tileInfos[selectedIdx.y][selectedIdx.x].iconInfos)
	{
		if (iconInfo.objectName == OBJECT_NAME::GATE)
		{
			(*_gateObjectPtr.get()) = &iconInfo;
			return true;
		}
	}

	return false;
}

bool MapTool::canGo(const POINT& selectedIdx) const
{
	if (selectedIdx.x < 0 || MaxTileEA.x < selectedIdx.x || selectedIdx.y < 0 || MaxTileEA.y < selectedIdx.y)
		return false;

	if (_tileInfos[selectedIdx.y][selectedIdx.x].iconInfos[TileInformationHandler::toInt(MATERIAL::OBJECT)].material == MATERIAL::OBJECT)
		return false;

	return true;
}

void MapTool::initMap(FPOINT scale, POINT startPosition, POINT subResolution, POINT halfSubResolution, bool isMapLoadingSupported)
{
	_scale = move(scale);

	_tileSize = { static_cast<int>(_scale.x * TileSize.x), static_cast<int>(_scale.y * TileSize.y) };
	_maxPrintCount = { subResolution.x / (_tileSize.x), subResolution.y / (_tileSize.y) };
	_startLocation = move(startPosition);
	_destLocation = { _startLocation.x + (_maxPrintCount.x * _tileSize.x) + _tileSize.x, _startLocation.y + (_maxPrintCount.y * _tileSize.y) + _tileSize.y };
	_tileStartIdx = { 0, 0 };
	_tileEndIdx = _maxPrintCount;
	_distance = { (_tileStartIdx.x * _tileSize.x), (_tileStartIdx.y * _tileSize.y) };

	_gateObjectPtr = make_unique<ICON_INFORMATION*>(nullptr);

	initImage({ CanvasKeyFrame.x * _tileSize.x, CanvasKeyFrame.y * _tileSize.y });
	initTileInfos(isMapLoadingSupported);
	_triCtr.init(_startLocation, move(subResolution), move(halfSubResolution));

	setGammaMode(true);
}

void MapTool::reset(POINT startPosition, FPOINT scale)
{
	const auto& screenMgr = ScreenManager::getInstance();
	const auto& subResolution = move(screenMgr.getSubResolution());
	const auto& halfSubResolution = move(screenMgr.generateHalfOfResolution(subResolution));

	resetMap(move(startPosition), subResolution, halfSubResolution, scale);

	POINT paletteSP{ _startLocation.x + halfSubResolution.x, _startLocation.y + 20 * scale.y };
	_palette.reset(move(paletteSP), scale, _tileSize);
	_inventory.reset(_startLocation, _destLocation, subResolution, halfSubResolution, scale);
}

void MapTool::resetMap(POINT startPosition, POINT subResolution, POINT halfSubResolution, FPOINT scale)
{
	_tileSize = { static_cast<int>(scale.x * TileSize.x), static_cast<int>(scale.y * TileSize.y) };
	_maxPrintCount = { subResolution.x / (_tileSize.x), subResolution.y / (_tileSize.y) };
	_startLocation = move(startPosition);
	_destLocation = { _startLocation.x + (_maxPrintCount.x * _tileSize.x) + _tileSize.x, _startLocation.y + (_maxPrintCount.y * _tileSize.y) + _tileSize.y };
	_tileEndIdx = _maxPrintCount;
	_distance = { (_tileStartIdx.x * _tileSize.x), (_tileStartIdx.y * _tileSize.y) };

	resetImage(_tileSize);
	loadTileInfos(false);
	_triCtr.init(_startLocation, move(subResolution), move(halfSubResolution));
}

void MapTool::updateMapForDrawing()
{
	moveCanvasPosition();

	for (int i = _tileStartIdx.y; i <= _tileEndIdx.y; i++)
	{
		for (int j = _tileStartIdx.x; j <= _tileEndIdx.x; j++)
		{
			const RECT& calcRect = move(generateRect({ _tileInfos[i][j].rect.left - _distance.x, _tileInfos[i][j].rect.top - _distance.y }, _tileSize));
			if (PtInRect(&calcRect, MouseHandler::Mouse))
			{
				_palette.modifyToFitCurrentState(_startLocation, _destLocation, _tileStartIdx, { j, i }, { calcRect.left, calcRect.top });

				static uint16 pressedCount = 0;
				if (MouseHandler::isStayKeyDownByLeftClick())
				{
					switch (pressedCount <= 6)
					{
					case true:
						++pressedCount;
						break;
					case false:
						_palette.setDraggingState();
						break;
					}
				}

				if (MouseHandler::isLeftClick())
				{
					pressedCount = 0;
					_palette.handleTiles(_tileInfos, { j, i });
				}
			}
		}
	}
}

void MapTool::updateMapForPlaying(POINT movedCount)
{	
	if (CollisionHandler::checkWallCollision({ (*_playerCtrPtr)->getCurrPos()}, (*_playerCtrPtr)->getSize(), _tileSize, movedCount))
	{
		return;
	}

	_distance = { _distance.x + movedCount.x, _distance.y + movedCount.y };

	//Debugging::setPrintValue(calcTileCount);
	const POINT calcTileCount = convertPosToIndex(_distance, _tileSize);
	setStartTileForWorld(calcTileCount);
	setEndTileForWorld(calcTileCount);
	
	const POINT& pIdx = (*_playerCtrPtr)->getCurrIdx();
	const auto& preUseZoneN = _tileInfos[pIdx.y][pIdx.x].zoneN;
	if (getCurrentUseZoneN() != preUseZoneN)
	{
		switch (_isVisitedZoneMap[preUseZoneN])
		{
		case true:
			if ( (*_monsterCtrPtr)->isMonstersDied(getCurrentUseZoneN()))
				(*_monsterCtrPtr)->clearMonsterPtrsVec();
			break;
		case false:
			_isVisitedZoneMap[preUseZoneN] = true;

			(*_monsterCtrPtr)->moveCreatedMonsterList(preUseZoneN);

			for (const auto& location : _zonesLocationMap[preUseZoneN])
			{
				_tileInfos[location.y][location.x].isVisitedZone = true;
			}
			break;
		}

		setCurrentUseZoneN(preUseZoneN);
	}
	
	/*for (int i = _tileStartIdx.y; i <= _tileEndIdx.y; i++)
	{
		for (int j = _tileStartIdx.x; j <= _tileEndIdx.x; j++)
		{
			const auto& tileInfo = _tileInfos[i][j];	
			const RECT& calcRect = move(generateRect({ (tileInfo.rect.left - _distance.x), tileInfo.rect.top - _distance.y }, _tileSize));		
		}
	}*/

	(*_monsterCtrPtr)->setMonstersPosAndIdx();
	(*_monsterCtrPtr)->setFiredBulletPtrsPosAndIdx(movedCount, _distance);

	(*_playerCtrPtr)->setCurrPosAndIdx(_distance);
	(*_playerCtrPtr)->setFiredBulletPtrsPosAndIdx(movedCount, _distance);
}

void MapTool::renderMap(const HDC& hdc) const
{
	for (int i = _tileStartIdx.y; i <= _tileEndIdx.y; i++)
	{
		for (int j = _tileStartIdx.x; j <= _tileEndIdx.x; j++)
		{
			const auto& tileInfo = _tileInfos[i][j];
			const RECT& calcRect = move(generateRect({ tileInfo.rect.left - _distance.x, tileInfo.rect.top - _distance.y }, _tileSize));
			
			bool isPrint = false;
			for (const auto& iconInfo : tileInfo.iconInfos)
			{
				if (iconInfo.material == MATERIAL::NONE)
				{
					continue;
				}

				isPrint = true;
				_canvas.renderFrame(hdc, { calcRect.left, calcRect.top }, iconInfo.keyFrame);		
			}

			if (isGammaMode() && isPrint == true)
			{
				switch (tileInfo.isVisitedZone == false)
				{
				case true:
					_canvas.renderTransparentFrame(hdc, { calcRect.left, calcRect.top }, BlackTile);
					break;
				case false:
					_currentAreaGamma.renderTransparent(hdc, { calcRect.left, calcRect.top });
					break;
				}
			}

			TextHandler::renderImmediately(hdc, "돋움", i, j, 13, { calcRect.left, calcRect.top }, WhiteRGB, true);
		}
	}

	//_triCtr.drawCircle(hdc);
	//Debugging::renderPoint(hdc, "돋움", 40, { ScreenManager::getInstance().getSubResolution().x - 1000, ScreenManager::getInstance().getSubResolution().y - 500 }, WhiteRGB, true);
}

void MapTool::renderToSpecifiedLocation(const HDC& hdc, const POINT& dest, const POINT& keyFrame, const POINT& size, const POINT& printSize, const BYTE alpha)
{
	switch (alpha)
	{
	case 255:
		_canvas.renderFrame(hdc, dest, keyFrame, size, printSize);
		break;
	default:
		_canvas.renderTransparentFrame(hdc, dest, keyFrame, size, printSize, alpha);
		break;
	}
}

void MapTool::releaseMap()
{}

void MapTool::clearCanvas()
{
	for (auto& tileInfoY : _tileInfos)
	{
		for (auto& tileInfoX : tileInfoY)
		{
			for (auto& iconInfo : tileInfoX.iconInfos)
			{
				iconInfo = {};
			}
		}
	}
}

void MapTool::drawTiles(ICON_INFORMATION& iconInfo, const POINT& currentIdx, const POINT& endIdx, const int16 matIdx)
{
	ICON_INFORMATION gettableIconInfo = move(iconInfo);
	for (int i = currentIdx.y; i < endIdx.y; i++)
	{
		for (int j = currentIdx.x; j < endIdx.x; j++)
		{
			auto& tileInfo = _tileInfos[i][j];
			switch (gettableIconInfo.material)
			{
			case MATERIAL::ENEMY:
			case MATERIAL::BOSS:
				if (i == currentIdx.y && j == currentIdx.x)
				{
					switch (gettableIconInfo.keyFrame.y)
					{
					case 2:
						tileInfo.monsterInfo.createdEnemyNames[tileInfo.monsterInfo.createdEnemyCount] = move(Monster::toEnemyName(gettableIconInfo.keyFrame.x));
						++tileInfo.monsterInfo.createdEnemyCount;
						break;
					case 4:
						tileInfo.monsterInfo.createdBossNames[tileInfo.monsterInfo.createdBossCount] = move(Monster::toBossName(gettableIconInfo.keyFrame.x));
						++tileInfo.monsterInfo.createdBossCount;
						break;
					}
					tileInfo.monsterInfo.locationIdx = currentIdx;
					tileInfo.monsterInfo.isActivated = true;
				}

				break;
			}

			tileInfo.iconInfos[matIdx] = gettableIconInfo;
			tileInfo.iconInfos[matIdx].location = currentIdx;
			tileInfo.iconInfos[matIdx].keyFrame = { tileInfo.iconInfos[matIdx].keyFrame.x + (j - currentIdx.x), tileInfo.iconInfos[matIdx].keyFrame.y + (i - currentIdx.y) };
		}
	}
}

void MapTool::handleFile(const FILE_MODE& fileMode)
{
	const int16 ofnSize = sizeof(OPENFILENAME);
	char lpstrFile[MAX_PATH] = "";

	OPENFILENAMEA ofn;
	memset(&ofn, 0, ofnSize);
	ofn.lStructSize = ofnSize;
	ofn.hwndOwner = G_hWnd;
	ofn.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = "E:\Repository\EnterTheGungeon(Remake-Ver)\EnterTheGungeon(Remake-Ver)\Datas";
	if (::GetSaveFileNameA(&ofn) != NULL)
	{
		switch (fileMode)
		{
		case FILE_MODE::SAVE:
			saveFile(ofn.lpstrFile);
			break;
		case FILE_MODE::LOAD:
			loadFile(ofn.lpstrFile);
			resetTileInfos(false);
			break;
		default:
			assert(false);
			break;
		}
	}
}

void MapTool::openGate()
{
	const POINT installedIdx{ (*(*_gateObjectPtr.get())).location };
	const POINT& gateLength{ installedIdx.x + (*(*_gateObjectPtr.get())).sizeCount.x, installedIdx.y + (*(*_gateObjectPtr.get())).sizeCount.y};
	const int16 gateMaterialIdx{ TileInformationHandler::toInt((*(*_gateObjectPtr.get())).material) };
	for (int y = installedIdx.y; y < gateLength.y; y++)
	{
		for (int x = installedIdx.x; x < gateLength.x; x++)
		{
			_tileInfos[y][x].iconInfos[gateMaterialIdx] = {};
		}
	}	
}

void MapTool::setCurrentZoneEnvironment()
{
	setCurrentUseZoneN(0);
	_isVisitedZoneMap[getCurrentUseZoneN()] = true;
	for (const auto& location : _zonesLocationMap[getCurrentUseZoneN()])
	{
		_tileInfos[location.y][location.x].isVisitedZone = true;
	}
}

void MapTool::setStartTileForWorld(POINT startTile)
{
	if (startTile.x < 0)
		startTile.x = 0;
	if (startTile.y < 0)
		startTile.y = 0;

	_tileStartIdx = move(startTile);
}

void MapTool::setStartTileForMapTool(const POINT& moveCount)
{
	POINT calculatedStartTile{ _tileStartIdx.x + moveCount.x, _tileStartIdx.y + moveCount.y };
	if (calculatedStartTile.x < 0)
	{
		calculatedStartTile.x = 0;
	}
	else if (calculatedStartTile.x > MaxTileEA.x - _maxPrintCount.x)
	{
		calculatedStartTile.x = MaxTileEA.x - _maxPrintCount.x;
	}

	if (calculatedStartTile.y < 0)
	{
		calculatedStartTile.y = 0;
	}
	else if (calculatedStartTile.y > MaxTileEA.y - _maxPrintCount.y)
	{
		calculatedStartTile.y = MaxTileEA.y - _maxPrintCount.y;
	}

	_tileStartIdx = move(calculatedStartTile);
}

void MapTool::setEndTileForWorld(const POINT& moveCount)
{
	POINT calculatedEndTile{ _maxPrintCount.x + moveCount.x, _maxPrintCount.y + moveCount.y };
	if (MaxTileEA.x <= calculatedEndTile.x)
		calculatedEndTile.x = MaxTileEA.x - 1;
	if (MaxTileEA.y <= calculatedEndTile.y)
		calculatedEndTile.y = MaxTileEA.y - 1;

	_tileEndIdx = move(calculatedEndTile);
}

void MapTool::setEndTileForMapTool(const POINT& moveCount)
{
	POINT calculatedEndTile{ _tileEndIdx.x + moveCount.x, _tileEndIdx.y + moveCount.y };
	if (calculatedEndTile.x < _maxPrintCount.x)
	{
		calculatedEndTile.x = _maxPrintCount.x;
	}
	else if (calculatedEndTile.x >= MaxTileEA.x)
	{
		calculatedEndTile.x = MaxTileEA.x - 1;
	}

	if (calculatedEndTile.y < _maxPrintCount.y)
	{
		calculatedEndTile.y = _maxPrintCount.y;
	}
	else if (calculatedEndTile.y >= MaxTileEA.y)
	{
		calculatedEndTile.y = MaxTileEA.y - 1;
	}

	_tileEndIdx = move(calculatedEndTile);
}

void MapTool::initImage(POINT canvasSize)
{
	_canvas = ImageManager::makeImage("Images/Scene/Maptool/Canvas.bmp", {}, { 1.0f, 1.0f }, move(canvasSize), CanvasKeyFrame, true, { false, false }, MagentaRGB);
	_canvas.setTransparency(255);

	_currentAreaGamma = ImageManager::makeImage("Images/Scene/Maptool/Black.bmp", {}, { 1.0f, 1.0f }, _tileSize, true, { false, false }, MagentaRGB);
	_currentAreaGamma.setTransparency(GammaValue * 20);
}

void MapTool::resetImage(POINT printSize)
{
	ImageManager::resetImage(_canvas, {}, move(printSize));	
	ImageManager::resetImage(_currentAreaGamma, {}, _tileSize);
}

void MapTool::initTileInfos(bool isMapLoadingSupported)
{
	for (int i = 0; i < MaxTileEA.y; i++)
	{
		for (int j = 0; j < MaxTileEA.x; j++)
		{
			auto& tileInfo = _tileInfos[i][j];

			for (auto& iconInfo : tileInfo.iconInfos)
			{
				iconInfo = {};
				iconInfo.keyFrame = { MaxTileEA.x - 1, MaxTileEA.y - 1 };
			}

			for (int i = 0; i < CreatedMonsterCount; i++)
			{
				tileInfo.monsterInfo.createdEnemyNames[i] = ENEMY_NAME::END;
				tileInfo.monsterInfo.createdBossNames[i] = BOSS_NAME::END;
			}
			tileInfo.monsterInfo.locationIdx = {};
			tileInfo.monsterInfo.createdEnemyCount = 0;
			tileInfo.monsterInfo.createdBossCount = 0;
			tileInfo.monsterInfo.isActivated = false;

			tileInfo.rect = move(generateRect({ _startLocation.x + (j * _tileSize.x), _startLocation.y + (i * _tileSize.y) }, _tileSize));

			tileInfo.zoneN = -1;
		}
	}

	setExecutedMap(false);
	if (isMapLoadingSupported)
	{
		loadTileInfos();
	}	
}

void MapTool::resetTileInfos(bool isCreateMonsters)
{
	for (int i = 0; i < MaxTileEA.y; i++)
	{
		for (int j = 0; j < MaxTileEA.x; j++)
		{
			auto& tileInfo = _tileInfos[i][j];
			if (tileInfo.zoneN == getCurrentUseZoneN())
			{
				tileInfo.isVisitedZone = true;
			}
			_zonesLocationMap[tileInfo.zoneN].emplace_back(POINT{j, i});

			if (isCreateMonsters)
			{
				switch (tileInfo.iconInfos[TileInformationHandler::toInt(MATERIAL::ENEMY)].material)
				{
				case MATERIAL::ENEMY:
				case MATERIAL::BOSS:
					tileInfo.iconInfos[TileInformationHandler::toInt(MATERIAL::ENEMY)].material = MATERIAL::NONE;
					break;
				}

				if (tileInfo.monsterInfo.isActivated == true)
				{
					for (int i = 0; i < tileInfo.monsterInfo.createdBossCount; i++)
					{
						tileInfo.monsterInfo.createdBossNames[i];
					}

					for (int i = 0; i < tileInfo.monsterInfo.createdEnemyCount; i++)
					{
						switch (tileInfo.monsterInfo.createdEnemyNames[i])
						{
						case ENEMY_NAME::BULLET:
							EnemyFactory enemyFactory;
							EnemyBullet* enemyBulletPtr = enemyFactory.createBullet(this, _scale, _startLocation, move(convertIndexToPos(tileInfo.monsterInfo.locationIdx)), tileInfo.monsterInfo.locationIdx);
							enemyBulletPtr->setRect(move(generateRect(move(convertIndexToPos(tileInfo.monsterInfo.locationIdx)), enemyBulletPtr->getSize())));	
							enemyBulletPtr->initWeapon("Images/Weapons/Gun1.bmp", { "Sounds/Acoustic/Weapons/gun1Firing.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::GUN1_FIRING) }, { "Sounds/Acoustic/Weapons/gun1Reloading.mp3", SOUND_TYPE::ACOUSTIC, SoundController::acousticListToInt(ACOUSTIC_LIST::GUN1_RELOADING) }, 6, 15);
							(*_monsterCtrPtr.get())->addCreatedMonstersVecMap(enemyBulletPtr, tileInfo.zoneN);
							break;
						}
					}

					tileInfo.monsterInfo = {};
				}
			}

			tileInfo.rect = move(generateRect({ _startLocation.x + (j * _tileSize.x), _startLocation.y + (i * _tileSize.y) }, _tileSize));
		}
	}

	setCurrentZoneEnvironment();

	if (isCreateMonsters)
		(*_monsterCtrPtr)->moveCreatedMonsterList(getCurrentUseZoneN());
}

void MapTool::initPalette(Inventory& inventory, POINT paletteSP, FPOINT scale)
{
	_palette.setMapToolPtr(this);
	_palette.setInventoryPtr(&inventory);
	_palette.init(move(paletteSP), move(scale), _tileSize);
}

void MapTool::initInventory(Palette& palette, POINT subResolution, POINT halfSubResolution, FPOINT scale)
{
	_inventory.setMapToolPtr(this);
	_inventory.setPalettePtr(&palette);
	_inventory.init(_startLocation, _destLocation, move(subResolution), move(halfSubResolution), move(scale));
}

void MapTool::saveFile(const char* source)
{
	HANDLE hFile = CreateFileA(move(source), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		assert(hFile);
	}

	DWORD dwBytesWritten{ sizeof(_tileInfos) };
	DWORD writtenByte = 0;
	const bool bErrorFlag = ::WriteFile(hFile, &_tileInfos, dwBytesWritten, &writtenByte, NULL);
	switch (!bErrorFlag)
	{
	case true:
		assert(bErrorFlag);
		break;
	case false:
		if (writtenByte != dwBytesWritten)
		{
			assert(writtenByte == dwBytesWritten);
		}
		break;
	}

	::CloseHandle(hFile);
}

void MapTool::loadFile(const char* source)
{
	HANDLE hFile = ::CreateFileA(move(source), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		assert(hFile);
	}

	DWORD dwBytesWritten{ sizeof(_tileInfos) };
	DWORD readByte = 0;
	const bool bErrorFlag = ::ReadFile(hFile, &_tileInfos, dwBytesWritten, &readByte, NULL);
	if (!bErrorFlag)
	{
		assert(bErrorFlag);
	}

	::CloseHandle(hFile);

	setExecutedMap(true);
}

void MapTool::loadTileInfos(bool willLoadMap)
{
	if (willLoadMap)
		loadFile("E:\\Repository\\EnterTheGungeon(Remake-Ver)\\EnterTheGungeon(Remake-Ver)\\Datas\\1");

	if (isExecuteMap())
		resetTileInfos();
}

void MapTool::moveCanvasPosition()
{
	if (MouseHandler::Mouse.x < _startLocation.x || MouseHandler::Mouse.x > _destLocation.x)
		return;

	if (MouseHandler::Mouse.y < _startLocation.y || MouseHandler::Mouse.y > _destLocation.y)
		return;

	if (MouseHandler::isStayKeyDownByRightClick())
	{
		const int16 currentAngle = _triCtr.getCurrentAngle();
		if (currentAngle != -1)
		{
			uint16 speed = 0;
			POINT virtualPoint{ 0,0 };

			switch (currentAngle)
			{
			case 0:
				speed = 1;
				virtualPoint = { 1 + speed, 0 };
				break;
			case 45:
				speed = 2;
				virtualPoint = { 1 + speed, -1 + -speed };
				break;
			case 90:
				speed = 1;
				virtualPoint = { 0, -1 + -speed };
				break;
			case 135:
				speed = 2;
				virtualPoint = { -1 + -speed, -1 + -speed };
				break;
			case 180:
				speed = 1;
				virtualPoint = { -1 + -speed, 0 };
				break;
			case 225:
				speed = 2;
				virtualPoint = { -1 + -speed, 1 + +speed };
				break;
			case 270:
				speed = 1;
				virtualPoint = { 0, +1 + +speed };
				break;
			case 315:
				speed = 2;
				virtualPoint = { 1 + speed, +1 + +speed };
				break;
			}

			setStartTileForMapTool(virtualPoint);
			setEndTileForMapTool(virtualPoint);

			_distance = { (_tileStartIdx.x * _tileSize.x), (_tileStartIdx.y * _tileSize.y) };

			_palette.setMovementState();
		}
	}

	if (MouseHandler::isRightClick())
	{
		_palette.moveMovementStateToDifferentState();
	}
}




