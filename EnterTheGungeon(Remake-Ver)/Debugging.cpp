#include "Debugging.h"

TextHandler Debugging::TextHandler;
POINT		Debugging::valueOfPoint = { 0, 0 };
string		Debugging::valueOfString = "";
double		Debugging::valueOfDouble = 0;
int			Debugging::valueOfInteger = 0;

void Debugging::renderPoint(const HDC& hdc, const char* fontName, uint16 fontSize, POINT sp, COLORREF fontColor, bool isTrans)
{
	if (valueOfPoint.x != 0 && valueOfPoint.y != 0)
	{
		TextHandler.renderImmediately(hdc, move(fontName), valueOfPoint, fontSize, move(sp), fontColor, isTrans);
	}
}

void Debugging::renderString(const HDC& hdc, const char* fontName, uint16 fontSize, POINT sp, COLORREF fontColor, bool isTrans)
{
	if (valueOfString != "")
	{
		TextHandler.renderImmediately(hdc, move(fontName), move(valueOfString.c_str()), fontSize, move(sp), fontColor, isTrans);
	}
}

void Debugging::renderDouble(const HDC& hdc, const char* fontName, uint16 fontSize, POINT sp, COLORREF fontColor, bool isTrans)
{
	if (valueOfDouble != 0.0)
	{
		TextHandler.renderImmediately(hdc, move(fontName), valueOfDouble, fontSize, move(sp), fontColor, isTrans);
	}
}

void Debugging::renderInteger(const HDC& hdc, const char* fontName, uint16 fontSize, POINT sp, COLORREF fontColor, bool isTrans)
{
	if (valueOfInteger != 0)
	{
		TextHandler.renderImmediately(hdc, move(fontName), valueOfInteger, fontSize, move(sp), fontColor, isTrans);
	}
}




