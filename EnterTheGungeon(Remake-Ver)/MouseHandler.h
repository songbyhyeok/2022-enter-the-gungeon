#pragma once

#include "TextHandler.h"
#include "KeyController.h"

class MouseHandler
{
public:
	static void				init();
	static void				update(POINT mouse) { Mouse = move(mouse); }
	static void				render(const HDC& hdc);

	static bool				isOnceKeyDownByLeftClick() { return KeyController::isOnceKeyDown(VK_LBUTTON); }
	static bool				isStayKeyDownByLeftClick() { return KeyController::isStayKeyDown(VK_LBUTTON); }
	static bool				isStayKeyDownByRightClick() { return KeyController::isStayKeyDown(VK_RBUTTON); }
	static bool				isLeftClick() { return KeyController::isOnceKeyUp(VK_LBUTTON); }
	static bool				isRightClick() { return KeyController::isOnceKeyUp(VK_RBUTTON); }

	static void				resetPositionOfText();

private:
	static void				initText();

public:
	static TextHandler		TextHandler;
	static POINT			Mouse;
};

