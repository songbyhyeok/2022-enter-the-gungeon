#include "Button.h"
#include "ButtonState.h"
#include "MouseHandler.h"
#include "debugging.h"

void Button::update()
{
	switch (PtInRect(&_rect, MouseHandler::Mouse) || compareButtonStates(_btnStates[ButtonState::toInt(typeid(Pressed).name())]))
	{
	case true:
		if (_isUnselected && checkPoint(typeid(Active).name()))
		{
			_isUnselected = false;
			Debugging::setPrintValue("Active "); 
			
			const auto& activeState = *dynamic_cast<Active*>(_currentState.get());

			if (((*dynamic_cast<Active*>(_currentState.get())).getSoundFileSource() == "") == false)
			{
				activeState.stopSound();
				activeState.playSound();
			}

				activeState.toggleOn();
				activeState.launchToFunction(_launchKey);
		}

		{
			static uint16 pressedCount = 0;
			// 키를 계속해서 누르고 있는 상태.
			if (MouseHandler::isOnceKeyDownByLeftClick() || MouseHandler::isStayKeyDownByLeftClick())
			{
				if (checkPoint(typeid(Pressed).name()))
				{
					_previousButtonState = "";
					switch (pressedCount <= 5)
					{
					case true:
						++pressedCount;
						break;
					case false:
						Debugging::setPrintValue("누르고있어요 ");
						(*dynamic_cast<Pressed*>(_currentState.get())).launchToFunction(_launchKey);
						break;
					}
				}
			}
			// 키를 누르지 않은 상태로 들어와서 여기서 동일 키를 누른 상태
			if (MouseHandler::isLeftClick())
			{
				if (checkPoint(typeid(PressUp).name()))
				{
					if (5 < pressedCount)
						_previousButtonState = typeid(Pressed).name();

					pressedCount = 0;

					Debugging::setPrintValue("클릭했어요");

					const auto& pressUpState = *dynamic_cast<PressUp*>(_currentState.get());
					pressUpState.stopSound();
					pressUpState.playSound();
					pressUpState.launchToFunction(_launchKey);
				}
			}
		}
		break;
	case false:
		if (checkPoint(typeid(Disabled).name()))
		{
			_isUnselected = true;
			Debugging::setPrintValue("Disabled ");

			const auto& disabledState = *dynamic_cast<Disabled*>(_currentState.get());
			disabledState.toggleOff();
			disabledState.launchToFunction(_launchKey);
		}
		break;
	}
}

void Button::render(const HDC& hdc, bool isRenderRect, bool isRenderImage, bool isRenderText) const
{
	if (isRenderRect)
		ImageManager::renderRectBy(hdc, _rect);
	if (isRenderImage)
		_image.render(hdc, { 0, 0 });
	if (isRenderText)
		_textHandler.render(hdc);

	Debugging::renderString(hdc, "돋움", 55, { 2200, 70 }, WhiteRGB, true);
}

void Button::release()
{
	for (auto& buttonState : _btnStates)
	{
		if (buttonState)
			buttonState->release();
	}
}

bool Button::compareButtonStates(const shared_ptr<ButtonState>& buttonState) const
{
	return _currentState == buttonState;
}

void Button::initImage(POINT startPoint, FPOINT scale, POINT size)
{
	setImage(move(startPoint), move(scale), move(size));
	init();
}

void Button::initImage(const char* fileName, POINT startPoint, FPOINT scale, POINT size, bool isTrans, REVERSE_DATA reverseData, COLORREF transColor)
{
	findSourceName(fileName);
	setImage(move(fileName), move(startPoint), move(scale), move(size), isTrans, move(reverseData), transColor);
	init();
}

void Button::initImage(const char* fileName, POINT startPoint, FPOINT scale, POINT size, POINT keyFrame, bool isTrans, REVERSE_DATA reverseData, COLORREF transColor)
{
	findSourceName(fileName);
	setImage(move(fileName), move(startPoint), move(scale), move(size), move(keyFrame), isTrans, move(reverseData), transColor);
	init();
}

void Button::initText(const char* fontName, const char* textName, POINT sp, uint16 fontSize, COLORREF fontColor, bool isTrans)
{
	_textHandler.init(move(fontName), move(textName), move(sp), fontSize, fontColor, isTrans);
}

void Button::resetImage(POINT startPosition, FPOINT scale)
{
	ImageManager::resetImage(_image, move(startPosition), move(scale));
	setRect();
}

void Button::resetImage(POINT startPosition, POINT printSize)
{
	ImageManager::resetImage(_image, move(startPosition), move(printSize));
	setRect();
}

void Button::resetText(uint16 fontSize, POINT startPoint)
{
	_textHandler.setFontSize(fontSize);
	_textHandler.setStartPosition(move(startPoint));
}

void Button::createAndInitToButtonStateBy(const shared_ptr<ButtonState>& btnState, SOUND_FILE soundFile, function<void(const shared_ptr<FunctionsStorage>&, const vector<VPointerData>&)> function, vector<VPointerData> vPtrDataVec)
{
	if (btnState)
	{
		const auto& madeButton = createButtonState(btnState);
		madeButton->init(this, move(soundFile), move(function), move(vPtrDataVec));
	}
}

void Button::renderFrame(const HDC& hdc, bool isRenderRect, bool isRenderImage, bool isRenderText) const
{
	if (isRenderRect)
		ImageManager::renderRectBy(hdc, _rect);
	if (isRenderImage)
		_image.renderFrame(hdc, { 0, 0 });
	if (isRenderText)
		_textHandler.render(hdc);
}

void Button::renderTransparently(const HDC& hdc, bool isRenderRect, bool isRenderImage, bool isRenderText) const
{
	if (isRenderRect)
		ImageManager::renderRectBy(hdc, _rect);
	if (isRenderImage)
		_image.renderTransparent(hdc, { 0, 0 });
	if (isRenderText)
		_textHandler.render(hdc);
}

void Button::renderTransparentlyFrame(const HDC& hdc, bool isRenderRect, bool isRenderImage, bool isRenderText) const
{
	if (isRenderRect)
		ImageManager::renderRectBy(hdc, _rect);
	if (isRenderImage)
		_image.renderTransparentFrame(hdc, { 0, 0 }, {});
	if (isRenderText)
		_textHandler.render(hdc);
}

void Button::toggle(bool value)
{
	switch (value)
	{
		case true:
			changeTextColor(WhiteRGB);
			break;
		case false:
			changeTextColor(GrayRGB);
			break;
	}
}

void Button::changeButtonShape(uint16 keyFrameY)
{
	_image.setCurrentKeyFrameY(keyFrameY);
}

void Button::toggleButtonShape()
{
	assert(_image.getCurrentKeyFrame().y < 2);

	if (_image.getMaxFrame().y != 0)
		_image.setCurrentKeyFrameY(_image.getCurrentKeyFrame().y == 0 ? 1 : 0);
}

void Button::init()
{
	initLaunchKey();
	_previousButtonState = "";
	_isUnselected = true;
}

void Button::setImage(POINT sp, FPOINT scale, POINT size)
{
	_image = ImageManager::makeImage(move(sp), move(scale), move(size));
	setRect();
}

void Button::setImage(const char* fileName, POINT sp, FPOINT scale, POINT size, bool isTrans, REVERSE_DATA reverseData, COLORREF transColor)
{
	_image = ImageManager::makeImage(move(fileName), move(sp), move(scale), move(size), isTrans, move(reverseData), transColor);
	setRect();
}

void Button::setImage(const char* fileName, POINT sp, FPOINT scale, POINT size, POINT keyFrame, bool isTrans, REVERSE_DATA reverseData, COLORREF transColor)
{
	_image = ImageManager::makeImage(move(fileName), sp, move(scale), move(size), move(keyFrame), isTrans, move(reverseData), transColor);
	setRect();
}

void Button::setCurrentState(const shared_ptr<ButtonState>& btnState)
{
	btnState->setButtonStateWith(_currentState, btnState);
}

bool Button::checkPoint(const char* typeID)
{	
	const auto& getBtnState = _btnStates[ButtonState::toInt(typeID)];
	if (checkExistenceByButtonState(getBtnState))
	{
		if (getBtnState->checkTransitionPossible(getBtnState))
		{
			transitionTo(getBtnState);
			return true;
		}
	}

	return false;
}

const shared_ptr<ButtonState>& Button::createButtonState(const shared_ptr<ButtonState>& buttonState)
{
	const auto& btnState = _btnStates[ButtonState::toInt(buttonState.get()->getTypeID())] = buttonState;
	return btnState;
}

void Button::findSourceName(const char* fileName)
{
	// .bmp except
	const int sourceStrlen = strlen(fileName) - 4;
	int sourIdx = 0;
	for (int i = sourceStrlen - 1; i >= 0; i--)
	{
		if (fileName[i] == '/')
		{
			sourIdx = i + 1;

			break;
		}
	}

	const int destIdx = sourceStrlen - sourIdx;
	_sourceName.append(fileName, sourIdx, destIdx);
}

void Button::changeTextColor(const COLORREF & color)
{
	if (_textHandler.getTextName() != "")
	{
		_textHandler.setFontColor(color);
	}
}

