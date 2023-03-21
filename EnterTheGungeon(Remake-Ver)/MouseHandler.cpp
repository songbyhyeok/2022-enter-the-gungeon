#include "MouseHandler.h"
#include "ScreenManager.h"

TextHandler		MouseHandler::TextHandler;
POINT			MouseHandler::Mouse{ 0,0 };

void MouseHandler::init()
{
	initText();
}

void MouseHandler::render(const HDC& hdc)
{
	TextHandler.render(hdc, Mouse);
}

void MouseHandler::resetPositionOfText()
{
	const uint16 textStrlen = strlen("Mouse X:0000, Y:0000") * 10 + 30;
	POINT textPosition{ ScreenManager::getInstance().getMainResolution().x - textStrlen, 10 };
	TextHandler.setStartPosition(move(textPosition));
}

void MouseHandler::initText()
{
	const uint16 textStrlen = strlen("Mouse X:0000, Y:0000") * 10 + 30;
	POINT textPosition{ ScreenManager::getInstance().getMainResolution().x - textStrlen, 10 };
	TextHandler.init("µ¸¿ò", "Mouse X:%I64u, Y:%I64u", move(textPosition), 20, WhiteRGB, true);
}
