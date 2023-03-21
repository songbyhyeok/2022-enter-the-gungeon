#include "TextHandler.h"

uint16 TextManager::generateFontSizeByResolution()
{
	uint16 fontSize = 0;
	switch (ScreenManager::getInstance().getSubResolutionValue())
	{
	case ResolutionList::W2560H1080:
		fontSize = 55;
	case ResolutionList::W1920H1080:
		fontSize = 50;
		break;
	case ResolutionList::W1280H720:
		fontSize = 35;
		break;
	case ResolutionList::W1024H768:
		fontSize = 30;
		break;
	case ResolutionList::W800H600:
		fontSize = 25;
		break;
	case ResolutionList::W640H480:
		fontSize = 20;
		break;
	}

	return fontSize;
}

uint16 TextAlignment::alignStartX(uint16 startPositionX, uint16 imgSizeX, float scaleX)
{
	return startPositionX + static_cast<uint16>(imgSizeX * scaleX);
}

uint16 TextAlignment::alignStartY(uint16 startPositionY, uint16 imgSizeY, float scaleY)
{
	return startPositionY + static_cast<uint16>(imgSizeY * scaleY);
}

uint16 TextAlignment::alignStartY(uint16 imgSizeY)
{
	const auto& screenMgr = ScreenManager::getInstance();
	return screenMgr.generateStartPositionOfScreen().y + screenMgr.getCalculatedScale().y * imgSizeY;
}

uint16 TextAlignment::alignMiddleCenterX(uint16 imgSizeX)
{
	const auto& screenMgr = ScreenManager::getInstance();
	return ((screenMgr.generateHalfOfResolution(screenMgr.getMainResolution()).x) - ((imgSizeX / 2) * screenMgr.getCalculatedScale().x));
}

uint16 TextAlignment::alignMiddleCenterX(const char* textName)
{
	const auto& screenMgr = ScreenManager::getInstance();

	uint16 result = 0;
	switch (TextManager::generateFontSizeByResolution())
	{
	case 55:
		result = (screenMgr.generateHalfOfResolution(screenMgr.getMainResolution()).x) - ((strlen(textName) * 23 * screenMgr.getCalculatedScale().x) / 2);
		if (textName == "Gamma") result -= (15);
		break;
	case 50:
		result = (screenMgr.generateHalfOfResolution(screenMgr.getMainResolution()).x) - ( (strlen(textName) * 23 * screenMgr.getCalculatedScale().x) / 2);
		if (textName == "Gamma") result -= (15 );
		break;
	case 35:
		result = (screenMgr.generateHalfOfResolution(screenMgr.getMainResolution()).x) - ((strlen(textName) * 23 * screenMgr.getCalculatedScale().x) / 2);
		if (textName == "Gamma") result -= (15);
		break;
	case 30:
		result = (screenMgr.generateHalfOfResolution(screenMgr.getMainResolution()).x) - ((strlen(textName) * 23 * screenMgr.getCalculatedScale().x) / 2);
		if (textName == "Gamma") result -= (15);
		break;
	case 25:
		result = (screenMgr.generateHalfOfResolution(screenMgr.getMainResolution()).x) - ((strlen(textName) * 23 * screenMgr.getCalculatedScale().x) / 2);
		if (textName == "Gamma") result -= (15);
		break;
	case 20:
		result = (screenMgr.generateHalfOfResolution(screenMgr.getMainResolution()).x) - ((strlen(textName) * 23 * screenMgr.getCalculatedScale().x) / 2);
		if (textName == "Gamma") result -= (15);
		break;
	}

	return result;
}

void TextHandler::init(const char* fontName, const char* textName, POINT sp, uint16 fontSize, COLORREF fontColor, bool isTrans)
{
	_fontName = move(fontName);
	_textName = move(textName);
	_startPosition = move(sp);
	_fontSize = fontSize;
	_fontColor = fontColor;
	_isTrans = isTrans;
}

void TextHandler::render(const HDC& hdc) const
{
	pair<HFONT, HFONT> hFonts = move(generateHFonts(hdc, _fontName, _fontSize));
	printBy(hdc);

	releaseFont(hdc, hFonts.second, hFonts.first);
}

void TextHandler::render(const HDC& hdc, char value) const
{
	pair<HFONT, HFONT> hFonts = move(generateHFonts(hdc, _fontName, _fontSize));
	printBy(hdc, value);

	releaseFont(hdc, hFonts.second, hFonts.first);
}

void TextHandler::render(const HDC& hdc, uint64 value) const
{
	pair<HFONT, HFONT> hFonts = move(generateHFonts(hdc, _fontName, _fontSize));
	printBy(hdc, value);

	releaseFont(hdc, hFonts.second, hFonts.first);
}

void TextHandler::render(const HDC& hdc, double value) const
{
	pair<HFONT, HFONT> hFonts = move(generateHFonts(hdc, _fontName, _fontSize));
	printBy(hdc, value);

	releaseFont(hdc, hFonts.second, hFonts.first);
}

void TextHandler::render(const HDC& hdc, POINT value) const
{
	pair<HFONT, HFONT> hFonts = move(generateHFonts(hdc, _fontName, _fontSize));
	printBy(hdc, move(value));

	releaseFont(hdc, hFonts.second, hFonts.first);
}

void TextHandler::renderImmediately(const HDC& hdc, const char* fontName, POINT value, uint16 fontSize, const POINT& sp, COLORREF fontColor, bool isTrans)
{
	pair<HFONT, HFONT> hFonts = move(generateHFontsForStatic(hdc, move(fontName), fontSize));
	setTextColor(hdc, true, fontColor);

	char strText[25]{};
	sprintf_s(strText, "%d, %d", value.x, value.y);
	TextOutA(hdc, sp.x, sp.y, strText, strlen(strText));

	releaseFontForStatic(hdc, hFonts.second, hFonts.first);
}

void TextHandler::renderImmediately(const HDC& hdc, const char* fontName, double value, uint16 fontSize, const POINT& sp, COLORREF fontColor, bool isTrans)
{
	pair<HFONT, HFONT> hFonts = move(generateHFontsForStatic(hdc, move(fontName), fontSize));
	setTextColor(hdc, true, fontColor);

	char strText[25]{};
	sprintf_s(strText, " %f", value);
	TextOutA(hdc, sp.x, sp.y, strText, strlen(strText));

	releaseFontForStatic(hdc, hFonts.second, hFonts.first);
}

void TextHandler::renderImmediately(const HDC& hdc, const char* fontName, uint16 value, uint16 value2, uint16 fontSize, const POINT& sp, COLORREF fontColor, bool isTrans)
{
	pair<HFONT, HFONT> hFonts = move(generateHFontsForStatic(hdc, move(fontName), fontSize));
	setTextColor(hdc, true, fontColor);

	char strText[25]{};
	sprintf_s(strText, " %d %d", value, value2);
	TextOutA(hdc, sp.x, sp.y, strText, strlen(strText));

	releaseFontForStatic(hdc, hFonts.second, hFonts.first);
}

void TextHandler::renderImmediately(const HDC& hdc, const char* fontName, int value, uint16 fontSize, const POINT& sp, COLORREF fontColor, bool isTrans)
{
	pair<HFONT, HFONT> hFonts = move(generateHFontsForStatic(hdc, move(fontName), fontSize));
	setTextColor(hdc, true, fontColor);

	char strText[25]{};
	sprintf_s(strText, " %d", value);
	TextOutA(hdc, sp.x, sp.y, strText, strlen(strText));

	releaseFontForStatic(hdc, hFonts.second, hFonts.first);
}

void TextHandler::renderImmediately(const HDC& hdc, const char* fontName, const char* textName, uint16 fontSize, const POINT& sp, COLORREF fontColor, bool isTrans)
{
	pair<HFONT, HFONT> hFonts = move(generateHFontsForStatic(hdc, move(fontName), fontSize));
	setTextColor(hdc, true, fontColor);

	char strText[25]{};
	sprintf_s(strText, textName);
	TextOutA(hdc, sp.x, sp.y, strText, strlen(strText));

	releaseFontForStatic(hdc, hFonts.second, hFonts.first);
}

void TextHandler::modifyTextName(const char* textToReplace)
{
	_textName = move(textToReplace);	
	setStartPosition({ TextAlignment::alignMiddleCenterX(_textName), _startPosition.y });
}

void TextHandler::setTextColor(const HDC& hdc) const
{
	SetTextColor(hdc, _fontColor);
	switch (_isTrans)
	{
	case true:
		SetBkMode(hdc, TRANSPARENT);
		break;
	case false:
		SetBkMode(hdc, OPAQUE);
	}
}

pair<HFONT, HFONT> TextHandler::generateHFonts(const HDC& hdc, const char* fontName, uint16 fontSize) const
{
	//checkNullptr();

	HFONT font = move(CreateFontA(_fontSize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _fontName));
	HFONT oldFont = move(static_cast<HFONT>(SelectObject(hdc, font)));

	return make_pair(font, oldFont);
}

pair<HFONT, HFONT> TextHandler::generateHFontsForStatic(const HDC& hdc, const char* fontName, uint16 fontSize)
{
	HFONT font = move(CreateFontA(fontSize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, fontName));
	HFONT oldFont = move(static_cast<HFONT>(SelectObject(hdc, font)));

	return make_pair(font, oldFont);
}

void TextHandler::setTextColor(const HDC& hdc, bool isTrans, COLORREF fontColor)
{
	SetTextColor(hdc, fontColor);
	switch (isTrans)
	{
	case true:
		SetBkMode(hdc, TRANSPARENT);
		break;
	case false:
		SetBkMode(hdc, OPAQUE);
	}
}

void TextHandler::printBy(const HDC& hdc) const
{
	setTextColor(hdc);

	char strText[25]{};
	sprintf_s(strText, _textName);
	TextOutA(hdc, _startPosition.x, _startPosition.y, strText, strlen(strText));
}

void TextHandler::printBy(const HDC& hdc, char c) const
{
	setTextColor(hdc);

	char strText[25]{};
	sprintf_s(strText, _textName, c);
	TextOutA(hdc, _startPosition.x, _startPosition.y, strText, strlen(strText));
}

void TextHandler::printBy(const HDC& hdc, uint64 value) const
{
	setTextColor(hdc);

	char strText[25]{};
	sprintf_s(strText, _textName, value);
	TextOutA(hdc, _startPosition.x, _startPosition.y, strText, strlen(strText));
}

void TextHandler::printBy(const HDC& hdc, double value) const
{
	setTextColor(hdc);

	char strText[25]{};
	sprintf_s(strText, _textName, value);
	TextOutA(hdc, _startPosition.x, _startPosition.y, strText, strlen(strText));
}

void TextHandler::printBy(const HDC& hdc, POINT value) const
{
	setTextColor(hdc);

	char strText[25]{};
	sprintf_s(strText, _textName, value.x, value.y);
	TextOutA(hdc, _startPosition.x, _startPosition.y, strText, strlen(strText));
}

void TextHandler::releaseFont(const HDC& hdc, HFONT& oldFont, HFONT& font) const
{
	SelectObject(hdc, oldFont);
	DeleteObject(font);
}

void TextHandler::releaseFontForStatic(const HDC& hdc, HFONT& oldFont, HFONT& font)
{
	SelectObject(hdc, oldFont);
	DeleteObject(font);
}


