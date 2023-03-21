#include "Category.h"
#include "ButtonState.h"
#include "Inventory.h"
#include "MapTool.h"
#include "Debugging.h"

vector<ICON_INFORMATION> TileInformationHandler::generateIconDataVecByCategoryType(const CATEGORY_TYPE& type, const POINT& tileSize)
{
	vector<ICON_INFORMATION> iconInfosVec;
	switch (type)
	{
	case CATEGORY_TYPE::TILES:
	{
		for (int i = 0; i < 74; i++)
		{
			ICON_INFORMATION iconInfo;
			iconInfo.keyFrame = { i, 0 };
			iconInfo.sizeCount = {1,1};
			iconInfo.size = tileSize;
			iconInfo.material = MATERIAL::TERRAIN;
			iconInfo.objectName = OBJECT_NAME::NONE;
			iconInfosVec.emplace_back(move(iconInfo));
		}
	}
	break;
	case CATEGORY_TYPE::DECORATIONS:
	{
		for (int i = 0; i < 13; i++)
		{
			ICON_INFORMATION iconInfo;
			iconInfo.keyFrame = { i, 1 };
			iconInfo.sizeCount = { 1, 1 };
			iconInfo.size = tileSize;
			iconInfo.material = MATERIAL::DECORATIONS;
			iconInfo.objectName = OBJECT_NAME::NONE;
			iconInfosVec.emplace_back(move(iconInfo));
		}
	}
	break;
	case CATEGORY_TYPE::ENEMYS:
	{
		for (int i = 0; i < 6; i++)
		{
			ICON_INFORMATION iconInfo;
			iconInfo.keyFrame = { i * 2, 2 };
			iconInfo.sizeCount = { 2, 2 };
			iconInfo.size = { tileSize.x * 2, tileSize.y * 2 };
			iconInfo.material = MATERIAL::ENEMY;
			iconInfo.objectName = OBJECT_NAME::NONE;
			iconInfosVec.emplace_back(move(iconInfo));
		}
	}
	break;
	case CATEGORY_TYPE::BOSSES:
	{
		ICON_INFORMATION iconInfo1;
		iconInfo1.keyFrame = { 0, 4 };
		iconInfo1.sizeCount = { 2, 2 };
		iconInfo1.size = { tileSize.x * 2, tileSize.y * 2 };
		iconInfo1.material = MATERIAL::BOSS;
		iconInfo1.objectName = OBJECT_NAME::NONE;
		iconInfosVec.emplace_back(move(iconInfo1));

		ICON_INFORMATION iconInfo2;
		iconInfo2.keyFrame = { 2, 4 };
		iconInfo2.sizeCount = { 2, 2 };
		iconInfo2.size = { tileSize.x * 2, tileSize.y * 2 };
		iconInfo2.material = MATERIAL::BOSS;
		iconInfo2.objectName = OBJECT_NAME::NONE;
		iconInfosVec.emplace_back(move(iconInfo2));
	}
	break;
	case CATEGORY_TYPE::OBJECTS:
	{
		for (int i = 0; i < 3; i++)
		{
			ICON_INFORMATION iconInfo;
			iconInfo.keyFrame = {i * 2, 6};
			iconInfo.sizeCount = { 2, 1 };
			iconInfo.size = { tileSize.x * 2, tileSize.y};
			iconInfo.material = MATERIAL::OBJECT;
			switch (i)
			{
			case 0:
				iconInfo.objectName = OBJECT_NAME::DOOR;
			default:
				iconInfo.objectName = OBJECT_NAME::TABLE;
			}
			iconInfosVec.emplace_back(move(iconInfo));
		}

		ICON_INFORMATION iconInfo;
		iconInfo.keyFrame = { 6, 6 };
		iconInfo.sizeCount = { 2, 1 };
		iconInfo.size = { tileSize.x * 2, tileSize.y };
		iconInfo.material = MATERIAL::OBJECT;
		iconInfo.objectName = OBJECT_NAME::GATE;
		iconInfosVec.emplace_back(move(iconInfo));

		for (int i = 0; i < 6; i++)
		{
			ICON_INFORMATION iconInfo1;
			iconInfo1.material = MATERIAL::OBJECT;
			switch (i)
			{
			case 0:
			case 1:
				iconInfo1.keyFrame = { i * 2, 7 };
				iconInfo1.sizeCount = { 2, 2 };
				iconInfo1.size = { tileSize.x * 2, tileSize.y * 2};
				iconInfo1.material = MATERIAL::DECORATIONS;
				iconInfo.objectName = OBJECT_NAME::TREASURE_BOX;
				break;
			case 2:
				iconInfo1.keyFrame = { i * 2, 7 };
				iconInfo1.sizeCount = { 2, 2 };
				iconInfo1.size = { tileSize.x * 2, tileSize.y * 2 };
				iconInfo1.material = MATERIAL::TERRAIN;
				iconInfo.objectName = OBJECT_NAME::SMALL_TILESET;
				break;
			case 3:
				iconInfo1.keyFrame = { 6, 7 };
				iconInfo1.sizeCount = { 1, 2 };
				iconInfo1.size = { tileSize.x, tileSize.y * 2 };
				iconInfo.objectName = OBJECT_NAME::TABLE;
				break;
			case 4:
				iconInfo1.keyFrame = { 7, 7 };
				iconInfo1.sizeCount = { 1, 2 };
				iconInfo1.size = { tileSize.x, tileSize.y * 2 };
				iconInfo.objectName = OBJECT_NAME::WALL;
				break;
			case 5:
				iconInfo1.keyFrame = { 8, 7 };
				iconInfo1.sizeCount = { 1, 2 };
				iconInfo1.size = { tileSize.x, tileSize.y * 2 };
				iconInfo.objectName = OBJECT_NAME::POTION_MACHINE;
				break;
			}
			iconInfosVec.emplace_back(move(iconInfo1));
		}

		for (int i = 0; i < 4; i++)
		{
			ICON_INFORMATION iconInfo2;
			iconInfo2.keyFrame = { i * 3, 9 };
			iconInfo2.sizeCount = { 3, 3 };
			iconInfo2.size = { tileSize.x * 3, tileSize.y * 3 };
			iconInfo2.material = (i != 0) ? MATERIAL::TERRAIN : MATERIAL::OBJECT;
			iconInfo2.objectName = OBJECT_NAME::TILESET;
			iconInfosVec.emplace_back(move(iconInfo2));
		}

		for (int i = 0; i < 2; i++)
		{
			ICON_INFORMATION iconInfo3;
			iconInfo3.keyFrame = { i * 3, 12 };
			iconInfo3.sizeCount = { 3, 5 };
			iconInfo3.size = { tileSize.x * 3, tileSize.y * 5 };
			iconInfo3.material = MATERIAL::OBJECT;
			iconInfo3.objectName = i == 0 ? OBJECT_NAME::RAMPART : OBJECT_NAME::LIBRARY_WALL;
			iconInfosVec.emplace_back(move(iconInfo3));
		}

		ICON_INFORMATION iconInfo4;
		iconInfo4.keyFrame = { 0, 17 };
		iconInfo4.sizeCount = { 1, 4 };
		iconInfo4.size = { tileSize.x, tileSize.y * 4 };
		iconInfo4.material = MATERIAL::OBJECT;
		iconInfo4.objectName = OBJECT_NAME::GATE;
		iconInfosVec.emplace_back(move(iconInfo4));

		for (int i = 0; i < 2; i++)
		{
			ICON_INFORMATION iconInfo5;
			iconInfo5.keyFrame = { i * 4, 21 };
			iconInfo5.sizeCount = { 4, 8 };
			iconInfo5.size = { tileSize.x * 4, tileSize.y * 8 };
			iconInfo5.material = MATERIAL::OBJECT;
			switch (i)
			{
			case 0:	
				iconInfo5.objectName = OBJECT_NAME::EMPTY_SCULPTURE;
				break;
			case 1:
				iconInfo5.objectName = OBJECT_NAME::ELEVATOR;
				break;
			}
			iconInfosVec.emplace_back(move(iconInfo5));
		}

		ICON_INFORMATION iconInfo6;
		iconInfo6.keyFrame = { 0, 29 };
		iconInfo6.sizeCount = { 19, 10 };
		iconInfo6.size = { tileSize.x * 19, tileSize.y * 10 };
		iconInfo6.material = MATERIAL::TERRAIN;
		iconInfo6.objectName = OBJECT_NAME::STORE;
		iconInfosVec.emplace_back(move(iconInfo6));

		ICON_INFORMATION iconInfo7;
		iconInfo7.keyFrame = { 0, 39 };
		iconInfo7.sizeCount = { 19, 18 };
		iconInfo7.size = { tileSize.x * 19, tileSize.y * 18 };
		iconInfo7.material = MATERIAL::TERRAIN;
		iconInfo7.objectName = OBJECT_NAME::WAITING_ROOM;
		iconInfosVec.emplace_back(move(iconInfo7));

		ICON_INFORMATION iconInfo8;
		iconInfo8.keyFrame = { 0, 57 };
		iconInfo8.sizeCount = { 19, 24 };
		iconInfo8.size = { tileSize.x * 19, tileSize.y * 24 };
		iconInfo8.material = MATERIAL::TERRAIN;
		iconInfo8.objectName = OBJECT_NAME::BOOS_MAP;
		iconInfosVec.emplace_back(move(iconInfo8));
	}
	break;
	case CATEGORY_TYPE::UTILITIES:
		break;
	default:
		assert(false);
		break;
	}

	return iconInfosVec;
}

void Category::init(uint16 bottomStartPointY, POINT inventorySP, POINT categorySP, POINT inventorySize, FPOINT scale, CATEGORY_TYPE categoryType)
{
	setEnabled(false);

	_inventorySP = move(inventorySP);
	_inventorySize = move(inventorySize);

	_type = move(categoryType);
	_matrix = Category::toMatrix(_type);

	POINT titleSP{ _inventorySP.x + (_inventorySize.x / 2) - strlen(Category::toCategoryName(_type)) * (15 * scale.x), _inventorySP.y + (scale.y * 50) };
	_categoryTitle.init("µ¸¿ò", Category::toCategoryName(_type), move(titleSP), TextManager::generateFontSizeByResolution(), WhiteRGB, true);

	_categoryBtn.initImage("Images/Scene/Maptool/Inventory/Category.bmp", categorySP, scale, { 110, 50 }, true, { false, false }, MagentaRGB);
	_categoryBtn.initText("µ¸¿ò", Category::toCategoryName(_type), categorySP, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_categoryBtn.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_categoryBtn.createAndInitToButtonStateBy(make_shared<Active>(), { "", SOUND_TYPE::UI }, nullptr);
	_categoryBtn.createAndInitToButtonStateBy(make_shared<PressUp>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::activateSelectedCategory, { {(*_inventoryPtr.get()), false}, { &_type, false} });
	
	if (_type != CATEGORY_TYPE::UTILITIES)
	{
		_iconInfosVec = TileInformationHandler::generateIconDataVecByCategoryType(_type, (*_mapToolPtr)->getTileSize());
		_iconBtnsVec.resize(_iconInfosVec.size());

		convertMatrixText(_matrix);
		POINT matrixSP{ _inventorySP.x + (_inventorySize.x / 2) - strlen(_matrixText) * (15 * scale.x), _inventorySP.y + (_inventorySize.y * 0.10f) };
		_matrixTextHandler.init("µ¸¿ò", _matrixText, move(matrixSP), TextManager::generateFontSizeByResolution(), WhiteRGB, true);

		const POINT iconBorderSize{ _inventorySize.x - _inventorySize.x * 0.28f, _inventorySize.y - _inventorySize.y * 0.2f };
		const POINT iconSize{ iconBorderSize.x / (_matrix.x + 1),  iconBorderSize.y / (_matrix.y + 1) };
		const POINT spacing{ (iconBorderSize.x / (_matrix.x + 1) / (_matrix.x + 1)), scale.y * 25 };
		const POINT gridStartLocation{ _inventorySP.x + (100 * scale.x) + spacing.x, _inventorySP.y + (140 * scale.y) + spacing.y };

		_iconSpacing = { spacing.x + iconSize.x, iconSize.y + spacing.y };
		_startIdx = 0;
		_endBorderLineY = bottomStartPointY;

		const POINT leftArrowSP{ matrixSP.x - (50 * scale.x), matrixSP.y };
		_leftArrowButtons.up.initImage("Images/Scene/Maptool/Inventory/Arrow.bmp", { leftArrowSP.x, static_cast<int>(leftArrowSP.y + (5 * scale.y)) }, scale, { 30, 40 }, { 1,2 }, true, { false, false }, MagentaRGB);
		_leftArrowButtons.up.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, &FunctionsStorage::changeButtonShape, { {&_leftArrowButtons.up, false}, {new int16(0), true} });
		_leftArrowButtons.up.createAndInitToButtonStateBy(make_shared<Active>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::changeButtonShape, { {&_leftArrowButtons.up, false}, {new int16(1), true} });
		_leftArrowButtons.up.createAndInitToButtonStateBy(make_shared<Pressed>(), {"", SOUND_TYPE::UI}, &FunctionsStorage::setIconMatrix, { {this, false}, {new POINT{-1, 0}, true } });
		_leftArrowButtons.up.createAndInitToButtonStateBy(make_shared<PressUp>(), {"", SOUND_TYPE::UI}, &FunctionsStorage::setIconMatrix, { {this, false}, {new POINT{-1, 0}, true } });

		_leftArrowButtons.down.initImage("Images/Scene/Maptool/Inventory/Arrow.bmp", { leftArrowSP.x, static_cast<int>(leftArrowSP.y + (25 * scale.y)) }, scale, { 30, 40 }, { 1,2 }, true, { false, true }, MagentaRGB);
		_leftArrowButtons.down.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, &FunctionsStorage::changeButtonShape, { {&_leftArrowButtons.down, false}, {new int16(0), true} });
		_leftArrowButtons.down.createAndInitToButtonStateBy(make_shared<Active>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::changeButtonShape, { {&_leftArrowButtons.down, false}, {new int16(1), true} });
		_leftArrowButtons.down.createAndInitToButtonStateBy(make_shared<Pressed>(), {"", SOUND_TYPE::UI}, &FunctionsStorage::setIconMatrix, { {this, false}, {new POINT{1, 0}, true } });
		_leftArrowButtons.down.createAndInitToButtonStateBy(make_shared<PressUp>(), {"", SOUND_TYPE::UI}, &FunctionsStorage::setIconMatrix, { {this, false}, {new POINT{1, 0}, true } });

		const uint16 x = scale.x <= 1 ? 210 * scale.x : 210 * 1.0f;
		const POINT rightArrowSP{ matrixSP.x + x, matrixSP.y };
		_rightArrowButtons.up.initImage("Images/Scene/Maptool/Inventory/Arrow.bmp", { rightArrowSP.x, static_cast<int>(rightArrowSP.y + (5 * scale.y)) }, scale, { 30, 40 }, { 1,2 }, true, { false, false }, MagentaRGB);
		_rightArrowButtons.up.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, &FunctionsStorage::changeButtonShape, { {&_rightArrowButtons.up, false}, {new int16(0), true} });
		_rightArrowButtons.up.createAndInitToButtonStateBy(make_shared<Active>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::changeButtonShape, { {&_rightArrowButtons.up, false}, {new int16(1), true} });
		_rightArrowButtons.up.createAndInitToButtonStateBy(make_shared<Pressed>(), {"", SOUND_TYPE::UI}, &FunctionsStorage::setIconMatrix, { {this, false}, {new POINT{0, -1}, true } });
		_rightArrowButtons.up.createAndInitToButtonStateBy(make_shared<PressUp>(), {"", SOUND_TYPE::UI}, &FunctionsStorage::setIconMatrix, { {this, false}, {new POINT{0, -1}, true } });

		_rightArrowButtons.down.initImage("Images/Scene/Maptool/Inventory/Arrow.bmp", { rightArrowSP.x, static_cast<int>(rightArrowSP.y + (25 * scale.y)) }, scale, { 30, 40 }, { 1,2 }, true, { false, true }, MagentaRGB);
		_rightArrowButtons.down.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, &FunctionsStorage::changeButtonShape, { {&_rightArrowButtons.down, false}, {new int16(0), true} });
		_rightArrowButtons.down.createAndInitToButtonStateBy(make_shared<Active>(),  { "", SOUND_TYPE::UI }, &FunctionsStorage::changeButtonShape, { {&_rightArrowButtons.down, false}, {new int16(1), true} });
		_rightArrowButtons.down.createAndInitToButtonStateBy(make_shared<Pressed>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::setIconMatrix, { {this, false}, {new POINT{0, 1}, true } });
		_rightArrowButtons.down.createAndInitToButtonStateBy(make_shared<PressUp>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::setIconMatrix, { {this, false}, {new POINT{0, 1}, true } });

		_scroll.setPalettePtr(_palettePtr);
		_scroll.setCategoryPtr(this);
		_scroll.init({ static_cast<int>(_inventorySP.x + (_inventorySize.x * 0.85f)), gridStartLocation.y - static_cast<int>((50 * scale.y)) }, scale, generateInvisibleIconCount(gridStartLocation.y));

		for (int i = 0; i < _matrix.y; i++)
		{
			for (int j = 0; j < _matrix.x; j++)
			{
				const POINT iconSP{ (gridStartLocation.x + (_iconSpacing.x) * j), gridStartLocation.y + (_iconSpacing.y * i) };
				const uint16 iconIdx = i * _matrix.x + j;
				if (_iconInfosVec.size() <= iconIdx)
					return;

				auto& iconBtn = _iconBtnsVec[iconIdx];
				iconBtn.initImage("Images/Scene/Maptool/Inventory/Icon.bmp", iconSP, { 1.0f, 1.0f }, iconSize, true);
				iconBtn.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::setIconInfoTransparency, { {this, false}, {new uint16(iconIdx), true}, {new BYTE{255}, true} });
				iconBtn.createAndInitToButtonStateBy(make_shared<Active>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::setIconInfoTransparency, { {this, false}, {new uint16(iconIdx), true}, {new BYTE{150}, true} });
				auto& iconInfo = _iconInfosVec[iconIdx];
				iconBtn.createAndInitToButtonStateBy(make_shared<PressUp>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::setIconInformation, { {&(*(*_palettePtr)), false}, {&(*(*_inventoryPtr)), false}, {&iconInfo, false} });

				iconInfo.printSize = { static_cast<int>(iconBtn.getImagePrintSize().x * 0.8f), static_cast<int>(iconBtn.getImagePrintSize().y * 0.8f) };
				iconInfo.location = { iconSP.x + (iconBtn.getImagePrintSize().x / 2) - (iconInfo.printSize.x / 2), iconSP.y + (iconBtn.getImagePrintSize().y / 2) - (iconInfo.printSize.y / 2) };
				iconInfo.alpha = 255;
			}
		}

		return;
	}

	_iconInfosVec.resize(0);
	_iconBtnsVec.resize(_matrix.y);
	for (int i = 0; i < _matrix.y; i++)
	{
		const POINT iconSP{ ((_inventorySP.x + (_inventorySize.x / 2)) - (400 / 2) * scale.x) , _inventorySP.y + ((150 * scale.y) * (i + 1)) };

		auto& iconBtn = _iconBtnsVec[i];
		iconBtn.initImage("Images/Scene/Maptool/Inventory/GoldBorder.bmp", iconSP, scale, {400, 150}, true, { false ,false }, MagentaRGB);
		const char* iconBtnName {};
		switch (i)
		{
		case 0:
			iconBtnName = { "Save" };
			iconBtn.createAndInitToButtonStateBy(make_shared<PressUp>(), {"", SOUND_TYPE::UI}, &FunctionsStorage::handleFile, {{&(*(*_mapToolPtr)), false}, {new FILE_MODE(FILE_MODE::SAVE), true}});
			break;
		case 1:				
			iconBtnName = { "Load" };
			iconBtn.createAndInitToButtonStateBy(make_shared<PressUp>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::handleFile, { {&(*(*_mapToolPtr)), false}, {new FILE_MODE(FILE_MODE::LOAD), true} });
			break;
		case 2:
			iconBtnName = { "Reset" };
			iconBtn.createAndInitToButtonStateBy(make_shared<PressUp>(), { "", SOUND_TYPE::UI}, &FunctionsStorage::clearCanvas, {{&(*(*_mapToolPtr)), false}});
			break;
		}

		const uint16 fontSize{ static_cast<uint16>(TextManager::generateFontSizeByResolution() * 2) };
		POINT textSP{ iconSP.x + ((iconBtn.getImagePrintSize().x / 2) - fontSize), iconSP.y + ((iconBtn.getImagePrintSize().y / 2) - (fontSize / 2)) };
		iconBtn.initText("µ¸¿ò", move(iconBtnName), move(textSP), fontSize, WhiteRGB, true);
		iconBtn.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::UI }, nullptr);
		iconBtn.createAndInitToButtonStateBy(make_shared<Active>(), { "", SOUND_TYPE::UI }, nullptr);
	}
}

void Category::update()
{
	_categoryBtn.update();

	if (getActivatedState())
	{
		if (_type != CATEGORY_TYPE::UTILITIES)
		{
			_leftArrowButtons.up.update();
			_leftArrowButtons.down.update();

			_rightArrowButtons.up.update();
			_rightArrowButtons.down.update();

			_scroll.update();
			for (auto& iconButton : _iconBtnsVec)
			{
				if (_endBorderLineY <= iconButton.getImageSP().y)
					return;

				iconButton.update();
			}

			return;
		}

		for (auto& iconButton : _iconBtnsVec)
			iconButton.update();
	}
}

void Category::render(const HDC& hdc) const
{
	_categoryBtn.render(hdc, false, true, true);

	if (getActivatedState())
	{
		_categoryTitle.render(hdc);

		if (_type != CATEGORY_TYPE::UTILITIES)
		{
			_leftArrowButtons.up.renderFrame(hdc, false, true, false);
			_leftArrowButtons.down.renderFrame(hdc, false, true, false);

			_rightArrowButtons.up.renderFrame(hdc, false, true, false);
			_rightArrowButtons.down.renderFrame(hdc, false, true, false);

			_scroll.render(hdc);

			_matrixTextHandler.render(hdc);

			for (int i = _startIdx; i < _iconBtnsVec.size(); i++)
			{
				if (_endBorderLineY <= _iconBtnsVec[i].getImageSP().y)
					return;

				_iconBtnsVec[i].render(hdc, false, true, false);

				const auto& iconInfo = _iconInfosVec[i];
				(*_mapToolPtr)->renderToSpecifiedLocation(hdc, iconInfo.location, iconInfo.keyFrame, iconInfo.size, iconInfo.printSize, iconInfo.alpha);
			}

			return;
		}

		for (const auto& iconButton : _iconBtnsVec)
			iconButton.render(hdc, false, true, true);
	}
}

void Category::release()
{
	_categoryBtn.release();

	if (_type != CATEGORY_TYPE::UTILITIES)
	{
		_leftArrowButtons.up.release();
		_leftArrowButtons.down.release();

		_rightArrowButtons.up.release();
		_rightArrowButtons.down.release();

		_scroll.release();
	}

	for (auto& iconBtn : _iconBtnsVec)
		iconBtn.release();
}

void Category::setIconMatrix(POINT matrix)
{
	POINT virtualMatrix{ _matrix.x + matrix.x, _matrix.y + matrix.y };
	if (virtualMatrix.x < 1)
		virtualMatrix.x = 1;
	else if (virtualMatrix.x > 10)
		virtualMatrix.x = 10;
	if (virtualMatrix.y < 1)
		virtualMatrix.y = 1;
	else if (virtualMatrix.y > 15)
		virtualMatrix.y = 15;

	matrix.x != 0 ? virtualMatrix.y = (_iconInfosVec.size() / virtualMatrix.x) + 1 : virtualMatrix.x = (_iconInfosVec.size() / virtualMatrix.y) + 1;

	_matrix = move(virtualMatrix);
	convertMatrixText(_matrix);
	resetIconButtons(ScreenManager::getInstance().getCalculatedScale());
}

void Category::reset(uint16 bottomStartPointY, POINT inventorySP, POINT categorySP, POINT inventorySize, FPOINT scale)
{
	_inventorySP = move(inventorySP);
	_inventorySize = move(inventorySize);

	if (_type != CATEGORY_TYPE::UTILITIES)
	{		
		POINT matrixSP{ _inventorySP.x + (_inventorySize.x / 2) - strlen(_matrixText) * (15 * scale.x), _inventorySP.y + (_inventorySize.y * 0.10f) };
		const POINT leftArrowSP{ matrixSP.x - (50 * scale.x), matrixSP.y };
		_leftArrowButtons.up.resetImage({ leftArrowSP.x, static_cast<int>(leftArrowSP.y + (5 * scale.y)) }, scale);
		_leftArrowButtons.down.resetImage({ leftArrowSP.x, static_cast<int>(leftArrowSP.y + (25 * scale.y)) }, scale);

		const uint16 x = scale.x <= 1 ? 210 * scale.x : 210 * 1.0f;
		const POINT rightArrowSP{ matrixSP.x + x, matrixSP.y };
		_rightArrowButtons.up.resetImage({ rightArrowSP.x, static_cast<int>(rightArrowSP.y + (5 * scale.y)) }, scale);
		_rightArrowButtons.down.resetImage({ rightArrowSP.x, static_cast<int>(rightArrowSP.y + (25 * scale.y)) }, scale);

		_matrixTextHandler.setStartPosition(move(matrixSP));
		_matrixTextHandler.setFontSize(move(TextManager::generateFontSizeByResolution()));

		_endBorderLineY = bottomStartPointY;
	}

	_categoryBtn.resetImage(categorySP, scale);
	_categoryBtn.resetText(TextManager::generateFontSizeByResolution(), categorySP);

	POINT titleSP{ _inventorySP.x + (_inventorySize.x / 2) - strlen(Category::toCategoryName(_type)) * (15 * scale.x), _inventorySP.y + (scale.y * 50) };
	_categoryTitle.setStartPosition(move(titleSP));
	_categoryTitle.setFontSize(TextManager::generateFontSizeByResolution());

	resetIconButtons(scale);
}

void Category::moveLines(uint16 sectionY)
{
	const uint16 sectionIconXEA = sectionY * _matrix.x;
	int16 intervalIconXEA = sectionIconXEA - _startIdx;
	int16 spacingY = 0;
	if (intervalIconXEA == 0)
		return;
	else if (intervalIconXEA < 0)
	{
		intervalIconXEA = -intervalIconXEA;
		_startIdx = _startIdx - intervalIconXEA;
		spacingY = _iconSpacing.y * (intervalIconXEA / _matrix.x);
	}
	else if (intervalIconXEA > 0)
	{
		_startIdx = _startIdx + intervalIconXEA;
		spacingY = -(_iconSpacing.y * (intervalIconXEA / _matrix.x));
	}

	for (int i = 0; i < _iconBtnsVec.size(); i++)
	{
		auto& iconBtn = _iconBtnsVec[i];
		const POINT& iconSP{ iconBtn.getImageSP().x, iconBtn.getImageSP().y + spacingY };
		iconBtn.setImageStartPosition(iconSP);
		iconBtn.setRectStartPosition(iconSP, iconBtn.getImagePrintSize());

		auto& iconInfo = _iconInfosVec[i];
		iconInfo.location = { iconSP.x + (iconBtn.getImagePrintSize().x / 2) - (iconInfo.printSize.x / 2), iconSP.y + (iconBtn.getImagePrintSize().y / 2) - (iconInfo.printSize.y / 2) };
	}
}

const char* Category::toCategoryName(CATEGORY_TYPE category)
{
	const char* name{};
	switch (category)
	{
	case CATEGORY_TYPE::TILES:
		name = "Tiles";
		break;
	case CATEGORY_TYPE::OBJECTS:
		name = "Objects";
		break;
	case CATEGORY_TYPE::DECORATIONS:
		name = "Decorations";
		break;
	case CATEGORY_TYPE::ENEMYS:
		name = "Enemys";
		break;
	case CATEGORY_TYPE::BOSSES:
		name = "Bosses";
		break;
	case CATEGORY_TYPE::UTILITIES:
		name = "Utilities";
		break;
	default:
		assert(0);
		break;
	}

	return name;
}

POINT Category::toMatrix(CATEGORY_TYPE category)
{
	POINT matrix{ 0,0 };
	switch (category)
	{
	case CATEGORY_TYPE::TILES:
		matrix = { 5, 15 };
		break;
	case CATEGORY_TYPE::DECORATIONS:
		matrix = { 5, 4 };
		break;
	case CATEGORY_TYPE::ENEMYS:
		matrix = { 2, 4 };
		break;
	case CATEGORY_TYPE::BOSSES:
		matrix = { 2, 1 };
		break;
	case CATEGORY_TYPE::OBJECTS:
		matrix = { 5, 6 };
		break;
	case CATEGORY_TYPE::UTILITIES:
		matrix = { 1, 3 };
		break;
	default:
		assert(0);
		break;
	}

	return matrix;
}

void Category::resetIconButtons(FPOINT scale)
{
	if (_type != CATEGORY_TYPE::UTILITIES)
	{
		const POINT iconBorderSize{ _inventorySize.x - _inventorySize.x * 0.28f, _inventorySize.y - _inventorySize.y * 0.2f };
		const POINT iconSize = { iconBorderSize.x / (_matrix.x + 1),  iconBorderSize.y / (_matrix.y + 1) };
		const POINT spacing = { (iconBorderSize.x / (_matrix.x + 1) / (_matrix.x + 1)), scale.y * 25 };
		const POINT gridStartLocation{ _inventorySP.x + (100 * scale.x) + spacing.x, _inventorySP.y + (140 * scale.y) + spacing.y };

		_iconSpacing = { spacing.x + iconSize.x, iconSize.y + spacing.y };

		_scroll.reset({ static_cast<int>(_inventorySP.x + (_inventorySize.x * 0.85f)), gridStartLocation.y - static_cast<int>((50 * scale.y)) }, scale, generateInvisibleIconCount(gridStartLocation.y));

		_startIdx = 0;
		for (int i = 0; i < _matrix.y; i++)
		{
			for (int j = 0; j < _matrix.x; j++)
			{
				const uint16 iconIdx = i * _matrix.x + j;
				if (_iconInfosVec.size() <= iconIdx)
					return;

				const POINT iconSP{ (_inventorySP.x + (70 * scale.x) + spacing.x + (_iconSpacing.x * j)), _inventorySP.y + (140 * scale.y) + spacing.y + (_iconSpacing.y * i) };
				auto& iconBtn = _iconBtnsVec[iconIdx];
				iconBtn.resetImage(iconSP, iconSize);

				auto& iconInfo = _iconInfosVec[iconIdx];
				iconInfo.printSize = { static_cast<int>(iconBtn.getImagePrintSize().x * 0.8f), static_cast<int>(iconBtn.getImagePrintSize().y * 0.8f) };
				iconInfo.location = { iconSP.x + (iconBtn.getImagePrintSize().x / 2) - (iconInfo.printSize.x / 2), iconSP.y + (iconBtn.getImagePrintSize().y / 2) - (iconInfo.printSize.y / 2) };
			}
		}

		return;
	}

	for (int i = 0; i < _matrix.y; i++)
	{
		const POINT iconSP{ ((_inventorySP.x + (_inventorySize.x / 2)) - (400 / 2) * scale.x) , _inventorySP.y + ((150 * scale.y) * (i + 1)) };

		auto& iconBtn = _iconBtnsVec[i];
		iconBtn.resetImage(iconSP, scale);

		const uint16 fontSize{ static_cast<uint16>(TextManager::generateFontSizeByResolution() * 2) };
		POINT textSP{ iconSP.x + ((iconBtn.getImagePrintSize().x / 2) - fontSize), iconSP.y + ((iconBtn.getImagePrintSize().y / 2) - (fontSize / 2)) };
		iconBtn.resetText(fontSize, move(textSP));
	}
}

void Category::convertMatrixText(const POINT& matrix)
{
	_matrixText[0] = matrix.x / 10 + '0';
	_matrixText[1] = matrix.x % 10 + '0';
	_matrixText[2] = ' ';
	_matrixText[3] = 'X';
	_matrixText[4] = ' ';
	_matrixText[5] = matrix.y / 10 + '0';
	_matrixText[6] = matrix.y % 10 + '0';
}

int16 Category::generateInvisibleIconCount(const int gridStartLocationY)
{
	const uint16 showedLengthY = _endBorderLineY - gridStartLocationY;
	const uint16 displayedIconCount = (showedLengthY / _iconSpacing.y);
	int16 invisibleIconCount = _matrix.y - displayedIconCount;
	if (invisibleIconCount < 0)
		invisibleIconCount = 0;
	
	return invisibleIconCount;
}
