#pragma once

#include "TextHandler.h"

class Debugging
{
public:
	static void renderPoint(const HDC& hdc, const char* fontName, uint16 fontSize, POINT sp, COLORREF fontColor, bool isTrans);
	static void renderString(const HDC& hdc, const char* fontName, uint16 fontSize, POINT sp, COLORREF fontColor, bool isTrans);
	static void renderDouble(const HDC& hdc, const char* fontName, uint16 fontSize, POINT sp, COLORREF fontColor, bool isTrans);
	static void renderInteger(const HDC& hdc, const char* fontName, uint16 fontSize, POINT sp, COLORREF fontColor, bool isTrans);	

	static void setPrintValue(string str) { valueOfString = move(str); }
	static void setPrintValue(POINT point) { valueOfPoint = move(point); }
	static void setPrintValue(double db) { valueOfDouble = db; }
	static void setPrintValue(int integer) { valueOfInteger = integer; }

private:
	static TextHandler	TextHandler;
	static string		valueOfString;
	static POINT		valueOfPoint;
	static double		valueOfDouble;
	static int			valueOfInteger;
};

