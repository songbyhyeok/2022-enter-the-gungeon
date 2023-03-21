#pragma once

#include "BaseInterface.h"
#include "ImageManager.h"
#include "Category.h"

constexpr uint16 CategoryCount = 6;

class MapTool;
class Palette;
class Inventory : BaseInterface
{
public:
	void								init(POINT startPos, POINT endPos, POINT subResolution, POINT halfSubResolution, FPOINT scale);
	void								update() override;
	void								render(const HDC& hdc) const override;;
	void								release() override;;

	void								setMapToolPtr(MapTool* mapToolPtr) { _mapToolPtr = make_shared<MapTool*>(mapToolPtr); }
	void								setPalettePtr(Palette* palettePtr) { _palettePtr = make_shared<Palette*>(palettePtr); }

	void								setEnabled(bool value) { _isActivated = value; }
	bool								getActivatedState() const { return _isActivated; }
	void								setIconSelectMode(bool selectMode) { _isIconSelectMode = selectMode; }
	const Category&						getSelectedCategory() { return _categorys[Category::toInt(_currentType)]; }

	bool								isIconSelectMode() const { return _isIconSelectMode; }

	void								reset(POINT startPos, POINT endPos, POINT subResolution, POINT halfSubResolution, FPOINT scale);
	void								activateSelectedCategory(CATEGORY_TYPE selectedType);

private:
	void								controlInventory();
	void								cancelIconSelectMode();

private:
	array<Category, CategoryCount>		_categorys;
	Image								_background;
	Image								_bottomBG;
	Image								_iconSelectOutline;
	shared_ptr<MapTool*>				_mapToolPtr;
	shared_ptr<Palette*>				_palettePtr;
	POINT								_selectedIconSP;
	POINT								_selectedIconPrintSize;
	CATEGORY_TYPE						_currentType;
	bool								_isIconSelectMode;
	bool								_isActivated;
};
