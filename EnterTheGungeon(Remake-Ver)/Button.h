#pragma once

#include "ImageManager.h"
#include "TextHandler.h"
#include "SoundController.h"
#include "FunctionsStorage.h"

constexpr uint16 ButtonStateEA = 4;

class ButtonState;
class Button
{
public:
	void											update();
	void											render(const HDC& hdc, bool isRenderRect = false, bool isRenderImage = false, bool isRenderText = false) const;
	void											release();

	void											setImageStartPosition(POINT pos) { _image.setStartPosition(move(pos)); }
	void											setImagePrintSize(POINT printSize) { _image.setPrintSize(move(printSize)); }
	void											setRectStartPosition(POINT pos, POINT size) { _rect = move(generateRect(pos, size)); }
	void											setTransparency(BYTE alpha) { _image.setTransparency(alpha); }
	const string&									getButtonName() const { return _sourceName; }
	const char*										getButtonTitleName() const { return _textHandler.getTextName(); }
	const POINT&									getImageSP() const { return _image.getStartPosition(); }
	const POINT&									getImageSize() const { return _image.getSize(); }
	const POINT&									getImagePrintSize() const { return _image.getImagePrintSize(); }
	const string&									getPreviousButtonState() { return _previousButtonState; }
	shared_ptr<ButtonState>&						getCurrentButtonState() { return _currentState; }

	bool											compareButtonStates(const shared_ptr<ButtonState>& buttonState) const;

	void											initImage(POINT startPoint, FPOINT scale, POINT size);
	void											initImage(const char* fileName, POINT startPoint, FPOINT scale, POINT size, bool isTrans = false, REVERSE_DATA reverseData = {false, false}, COLORREF transColor = 0);
	void											initImage(const char* fileName, POINT startPoint, FPOINT scale, POINT size, POINT keyFrame, bool isTrans = false, REVERSE_DATA reverseData = { false, false }, COLORREF transColor = 0);
	void											initText(const char* fontName, const char* textName, POINT sp, uint16 fontSize, COLORREF fontColor, bool isTrans);
	void											resetImage(POINT startPosition, FPOINT scale);
	void											resetImage(POINT startPosition, POINT printSize);
	void											resetText(uint16 fontSize, POINT startPoint);
	void											createAndInitToButtonStateBy(const shared_ptr<ButtonState>& btnState, SOUND_FILE soundFile, function<void(const shared_ptr<FunctionsStorage>&, const vector<VPointerData>&)> function, vector<VPointerData> vPtrDataVec = vector<VPointerData>(0));
	void											renderFrame(const HDC& hdc, bool isRenderRect = false, bool isRenderImage = false, bool isRenderText = false) const;
	void											renderTransparently(const HDC& hdc, bool isRenderRect = false, bool isRenderImage = false, bool isRenderText = false) const;
	void											renderTransparentlyFrame(const HDC& hdc, bool isRenderRect = false, bool isRenderImage = false, bool isRenderText = false) const;
	void											flipImageBy(bool toggle) { _image.setCurrentKeyFrameY(toggle == true ? 1 : 0); };
	void											toggle(bool value);
	void											changeButtonShape(uint16 keyFrameY);
	void											toggleButtonShape();

private:
	void											init();
	void											setImage(POINT startPoint, FPOINT scale, POINT size);
	void											setImage(const char* fileName, POINT startPoint, FPOINT scale, POINT size, bool isTrans = false, REVERSE_DATA reverseData = { false, false }, COLORREF transColor = false);
	void											setImage(const char* fileName, POINT startPoint, FPOINT scale, POINT size, POINT keyFrame, bool isTrans = false, REVERSE_DATA reverseData = { false, false }, COLORREF transColor = false);
	void											setRect() { _rect = move(generateRect(_image.getStartPosition(), _image.getImagePrintSize())); }
	void											setCurrentState(const shared_ptr<ButtonState>& btnState);

	bool											checkPoint(const char* typeID);
	bool											checkExistenceByButtonState(const shared_ptr<ButtonState>& buttonState) const { return buttonState != nullptr; }

	void											initLaunchKey() { _launchKey = make_shared<FunctionsStorage>(); }
	void											findSourceName(const char* fileName);	
	void											releaseImage() const { ImageManager::findImageToDelete(_image.getIdentityNumber()); }	
	const shared_ptr<ButtonState>&					createButtonState(const shared_ptr<ButtonState> &buttonState);
	const shared_ptr<FunctionsStorage>&				permissionToLaunch() const { return _launchKey; }
	void											transitionTo(const shared_ptr<ButtonState>& btnState) { setCurrentState(btnState); }
	void											changeTextColor(const COLORREF& color);

private:
	array<shared_ptr<ButtonState>, ButtonStateEA>	_btnStates;
	Image											_image;
	shared_ptr<ButtonState>							_currentState;
	shared_ptr<FunctionsStorage>					_launchKey;
	TextHandler										_textHandler;
	string											_sourceName;
	string											_previousButtonState;
	RECT											_rect;
	bool											_isUnselected;	
};