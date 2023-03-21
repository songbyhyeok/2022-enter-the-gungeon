#pragma once

#include "BaseInterface.h"
#include "Button.h"

class Palette;
class Category;
class Scroll : BaseInterface
{
public:
	void								init(POINT scrollSP, FPOINT scale, uint16 invisibleIconCount);
	void								update() override;
	void								render(const HDC& hdc) const override;
	void								release() override;

	void								setPalettePtr(const shared_ptr<Palette*>& palettePtr) { _palettePtr = palettePtr; }
	void								setCategoryPtr(Category* categoryPtr) { _categoryPtr = make_unique<Category*>(categoryPtr); }
	void								setTransparency(BYTE alpha) { _scrollBar.setTransparency(alpha); }

	void								reset(POINT scrollSP, FPOINT scale, uint16 invisibleIconCount);
	void								pullBar();
	void								putBar();

private:
	Button								_scrollBar;
	Image								_scroll;
	shared_ptr<Palette*>				_palettePtr;
	unique_ptr<Category*>				_categoryPtr;
	int									_clickedPositionY;
};

