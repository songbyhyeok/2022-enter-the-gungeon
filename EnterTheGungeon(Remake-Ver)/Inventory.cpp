#include "Inventory.h"
#include "MapTool.h"
#include "KeyController.h"

void Inventory::init(POINT startPos, POINT endPos, POINT subResolution, POINT halfSubResolution, FPOINT scale)
{
	if (endPos.x > 2560)
		endPos.x = 2560;

	setEnabled(false);
	_currentType = CATEGORY_TYPE::NONE;

	const POINT inventorySize{ halfSubResolution.x - (300 * scale.x), subResolution.y };
	const POINT inventorySP{ endPos.x - (inventorySize.x - (30 * scale.x)), startPos.y };
	_background = ImageManager::makeImage("Images/Scene/Maptool/Inventory/Inventory.bmp", inventorySP, { 1.0f, 1.0f }, inventorySize, true, { false, false }, MagentaRGB);

	POINT bottomInventorySize{ inventorySize.x, inventorySize.y - (0.8f * subResolution.y) };
	POINT bottomInventorSP{ inventorySP.x, inventorySP.y + inventorySize.y - bottomInventorySize.y };
	_bottomBG = ImageManager::makeImage("Images/Scene/Maptool/Inventory/BottomInventory.bmp", move(bottomInventorSP), { 1.0f, 1.0f }, move(bottomInventorySize), true, { false, false }, MagentaRGB);

	for (int i = 0; i < CategoryCount; i++)
	{
		auto& category = _categorys[i];
		category.setMapToolPtr(_mapToolPtr);
		category.setInventoryPtr(this);
		category.setPalettePtr(_palettePtr);
		POINT categorySP{ inventorySP.x - (40 * scale.x), inventorySP.y + ((180 + (i * 100)) * scale.y) };
		category.init(_bottomBG.getStartPosition().y, inventorySP, move(categorySP), inventorySize, scale, move(Category::toCategory(i + 1)));
	}

	_iconSelectOutline = ImageManager::makeImage("Images/Scene/Maptool/Inventory/Icon.bmp", { static_cast<int>(endPos.x - (250 * scale.x)), static_cast<int>((450 * scale.y)) }, scale, {175, 175}, true, { false, false }, MagentaRGB);
	_selectedIconPrintSize = { static_cast<int>(_iconSelectOutline.getImagePrintSize().x * 0.8f), static_cast<int>(_iconSelectOutline.getImagePrintSize().y * 0.8f) };
	_selectedIconSP = { _iconSelectOutline.getStartPosition().x + (_iconSelectOutline.getImagePrintSize().x / 2) - (_selectedIconPrintSize.x / 2), _iconSelectOutline.getStartPosition().y + (_iconSelectOutline.getImagePrintSize().y / 2) - (_selectedIconPrintSize.y / 2) };
}

void Inventory::update()
{
	cancelIconSelectMode();

	controlInventory();

	if (getActivatedState() == true)
	{
		for (auto& category : _categorys)
			category.update();
	}
}

void Inventory::render(const HDC& hdc) const
{
	switch (getActivatedState() == true)
	{
	case true:
		_background.render(hdc, { 0, 0 });

		for (const auto& category : _categorys)
			category.render(hdc);

		_bottomBG.render(hdc, { 0,0 });
		break;
	case false:
		if (isIconSelectMode() == true)
		{
			_iconSelectOutline.render(hdc, {});

			const auto& iconInfo = (*_palettePtr)->getIconInfo();
			(*_mapToolPtr)->renderToSpecifiedLocation(hdc, _selectedIconSP, iconInfo.keyFrame, iconInfo.size, _selectedIconPrintSize, 200);
		}
		break;
	}
}

void Inventory::release()
{
	for (auto& category : _categorys)
		category.release();
}

void Inventory::reset(POINT startPos, POINT endPos, POINT subResolution, POINT halfSubResolution, FPOINT scale)
{
	if (endPos.x > 2560)
		endPos.x = 2560;

	const POINT inventorySize{ halfSubResolution.x - (300 * scale.x), subResolution.y };
	const POINT inventorySP{ endPos.x - (inventorySize.x - (30 * scale.x)), startPos.y };
	ImageManager::resetImage(_background, inventorySP, inventorySize);

	POINT bottomInventorySize{ inventorySize.x, inventorySize.y - (0.8f * subResolution.y) };
	POINT bottomInventorSP{ inventorySP.x, inventorySP.y + inventorySize.y - bottomInventorySize.y };
	ImageManager::resetImage(_bottomBG, move(bottomInventorSP), move(bottomInventorySize));

	for (int i = 0; i < CategoryCount; i++)
	{
		POINT categorySP{ inventorySP.x - (40 * scale.x), inventorySP.y + ((180 + (i * 100)) * scale.y) };
		_categorys[i].reset(_bottomBG.getStartPosition().y, inventorySP, move(categorySP), inventorySize, scale);
	}

	ImageManager::resetImage(_iconSelectOutline, { static_cast<int>(endPos.x - (250 * scale.x)), static_cast<int>((450 * scale.y)) }, scale);
	_selectedIconPrintSize = { static_cast<int>(_iconSelectOutline.getImagePrintSize().x * 0.8f), static_cast<int>(_iconSelectOutline.getImagePrintSize().y * 0.8f) };
	_selectedIconSP = { _iconSelectOutline.getStartPosition().x + (_iconSelectOutline.getImagePrintSize().x / 2) - (_selectedIconPrintSize.x / 2), _iconSelectOutline.getStartPosition().y + (_iconSelectOutline.getImagePrintSize().y / 2) - (_selectedIconPrintSize.y / 2) };
}

void Inventory::activateSelectedCategory(CATEGORY_TYPE selectedType)
{
	if (_currentType != CATEGORY_TYPE::NONE)
		_categorys[Category::toInt(_currentType)].setEnabled(false);
	
	switch (_currentType != selectedType)
	{
	case true:
		_currentType = move(selectedType);
		_categorys[Category::toInt(_currentType)].setEnabled(true);
		break;
	case false:
		_currentType = CATEGORY_TYPE::NONE;
		break;
	}
}

void Inventory::controlInventory()
{
	if (KeyController::isOnceKeyUp(VK_Ikey))
	{
		setEnabled(!getActivatedState());
		setIconSelectMode(false);
		if (getActivatedState())
		{
			(*_palettePtr)->setIdleState();
		}
	}
}

void Inventory::cancelIconSelectMode()
{
	if (KeyController::isOnceKeyUp(VK_Xkey) && isIconSelectMode())
	{
		setEnabled(true);
		setIconSelectMode(false);

		(*_palettePtr)->setIdleState();
	}
}