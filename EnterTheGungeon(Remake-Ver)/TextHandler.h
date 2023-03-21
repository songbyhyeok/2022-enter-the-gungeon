#pragma once

#include "ScreenManager.h"

class TextManager
{
public:
	static uint16 generateFontSizeByResolution();
};

class TextAlignment
{
public:
	static						uint16 alignStartX(uint16 startPositionX, uint16 imgSizeX, float scaleX);
	static						uint16 alignStartY(uint16 startPositionY, uint16 imgSizeY, float scaleY);
	static						uint16 alignStartY(uint16 imgSizeY);
	static						uint16 alignMiddleCenterX(uint16 imgSizeX);
	static						uint16 alignMiddleCenterX(const char* textName);
};

class TextHandler
{
public:
	void						init(const char* fontName, const char* textName, POINT sp, uint16 fontSize, COLORREF fontColor, bool isTrans);
	void						render(const HDC& hdc, double value) const;
	void						render(const HDC& hdc, uint64 value) const;
	void						render(const HDC& hdc, POINT value) const;
	void						render(const HDC& hdc) const;
	void						render(const HDC& hdc, char value) const;

	static void					renderImmediately(const HDC& hdc, const char* fontName, POINT value, uint16 fontSize, const POINT& sp, COLORREF fontColor, bool isTrans);
	static void					renderImmediately(const HDC& hdc, const char* fontName, double value, uint16 fontSize, const POINT& sp, COLORREF fontColor, bool isTrans);
	static void					renderImmediately(const HDC& hdc, const char* fontName, uint16 value, uint16 value2, uint16 fontSize, const POINT& sp, COLORREF fontColor, bool isTrans);
	static void					renderImmediately(const HDC& hdc, const char* fontName, int value, uint16 fontSize, const POINT& sp, COLORREF fontColor, bool isTrans);
	static void					renderImmediately(const HDC& hdc, const char* fontName, const char* textName, uint16 fontSize, const POINT& sp, COLORREF fontColor, bool isTrans);

	void						setStartPosition(POINT startPoint) { _startPosition = move(startPoint); }
	void						setFontSize(uint16 fontSize) { _fontSize = fontSize; }
	void						setFontColor(COLORREF fontColor) { _fontColor = move(fontColor); }
	const char*					getTextName() const { return _textName; }

	void						modifyTextName(const char* textToReplace);
	
private:
	void						setTextColor(const HDC& hdc) const;

	pair<HFONT, HFONT>			generateHFonts(const HDC& hdc, const char* fontName, uint16 fontSize) const;
	void						checkNullptr() const { assert(_fontName && _textName); }
	void						printBy(const HDC& hdc) const;
	void						printBy(const HDC& hdc, char c) const;
	void						printBy(const HDC& hdc, uint64 value) const;
	void						printBy(const HDC& hdc, double value) const;
	void						printBy(const HDC& hdc, POINT value) const;
	void						releaseFont(const HDC& hdc, HFONT& oldFont, HFONT& font) const;

	static void					setTextColor(const HDC& hdc, bool isTrans, COLORREF fontColor);
	static pair<HFONT, HFONT>	generateHFontsForStatic(const HDC& hdc, const char* fontName, uint16 fontSize);
	static void					releaseFontForStatic(const HDC& hdc, HFONT& oldFont, HFONT& font);

private:
	POINT						_startPosition;
	uint16						_fontSize;
	COLORREF					_fontColor;
	const char*					_fontName;
	const char*					_textName;
	bool						_isTrans;
};

