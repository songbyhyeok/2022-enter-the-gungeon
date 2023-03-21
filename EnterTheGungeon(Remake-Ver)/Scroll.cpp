#include "Scroll.h"
#include "ButtonState.h"
#include "MouseHandler.h"
#include "Category.h"
#include "MapTool.h"
#include "Debugging.h"

void Scroll::init(POINT scrollSP, FPOINT scale, uint16 invisibleIconCount)
{
	_clickedPositionY = -1;

	if (scale.x > 1)
		scale.x = 1;

	_scroll = ImageManager::makeImage("Images/Scene/Maptool/Inventory/Scroll.bmp", scrollSP, scale, { 30, 750 });

	_scrollBar.initImage("Images/Scene/Maptool/Inventory/ScrollBar.bmp", move(scrollSP), move(scale), _scroll.getSize());
	_scrollBar.setImagePrintSize(move(POINT{ static_cast<int>(_scrollBar.getImageSize().x * scale.x), static_cast<int>((_scrollBar.getImageSize().y / (invisibleIconCount + 1)) * scale.y) }));	
	_scrollBar.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::setButtonTransparency, { {&_scrollBar, false}, {new BYTE{255}, true} });
	_scrollBar.createAndInitToButtonStateBy(make_shared<Active>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::setButtonTransparency, { {&_scrollBar, false}, {new BYTE{200}, true} });
	_scrollBar.createAndInitToButtonStateBy(make_shared<Pressed>(true), { "", SOUND_TYPE::UI }, &FunctionsStorage::pullScrollBar, { {this, false} });
	_scrollBar.createAndInitToButtonStateBy(make_shared<PressUp>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::putScrollBar, { {this, false} });
}

void Scroll::update()
{
	_scrollBar.update();
}

void Scroll::render(const HDC& hdc) const
{
	_scroll.render(hdc, { 0, 0 });
	_scrollBar.renderTransparently(hdc, false, true, false);
	
	Debugging::renderPoint(hdc, "µ¸¿ò", 40, { ScreenManager::getInstance().getSubResolution().x - 500, 200 }, WhiteRGB, true);
}

void Scroll::release()
{
	_scrollBar.release();
}

void Scroll::reset(POINT scrollSP, FPOINT scale, uint16 invisibleIconCount)
{
	_clickedPositionY = -1;

	if (scale.x > 1)
		scale.x = 1;

	ImageManager::resetImage(_scroll, scrollSP, scale);
	_scrollBar.resetImage(move(scrollSP), move(POINT{ static_cast<int>(_scrollBar.getImageSize().x * scale.x), static_cast<int>((_scrollBar.getImageSize().y / (invisibleIconCount + 1)) * scale.y) }));
}

void Scroll::pullBar()
{
	(*_palettePtr)->selectTool(PALETTE_STATE::PULL);

	if (_clickedPositionY == -1)
	{
		_clickedPositionY = MouseHandler::Mouse.y;
		return;
	}

	const POINT& scrollSP = _scroll.getStartPosition();
	const POINT& scrollBarSP = _scrollBar.getImageSP();
	const POINT& scrollBarPrintSize = _scrollBar.getImagePrintSize();

	const uint16 startBoundaryY = scrollSP.y;
	const uint16 endBoundaryY = scrollSP.y + _scroll.getImagePrintSize().y - scrollBarPrintSize.y;

	const int16 distanceY = _clickedPositionY - MouseHandler::Mouse.y;
	int16 calculatedY = scrollBarSP.y - distanceY;
	if (calculatedY < startBoundaryY)
		calculatedY = startBoundaryY;
	else if (calculatedY > endBoundaryY)
		calculatedY = endBoundaryY;

	if (scrollBarSP.y == calculatedY)
		return;

	const uint16 currentSectionY = calculatedY - _scroll.getStartPosition().y;
	const uint16 sectionIdx = currentSectionY / scrollBarPrintSize.y;
	(*_categoryPtr)->moveLines(sectionIdx);

	_scrollBar.setImageStartPosition({ scrollSP.x, calculatedY });
	_scrollBar.setRectStartPosition(scrollBarSP, scrollBarPrintSize);

	if (_clickedPositionY != MouseHandler::Mouse.y)
		_clickedPositionY = MouseHandler::Mouse.y;
}

void Scroll::putBar()
{
	(*_palettePtr)->selectTool(PALETTE_STATE::SELECTED);

	_clickedPositionY = -1;
}