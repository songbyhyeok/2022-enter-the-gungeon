#include "CameraController.h"
#include "TimeHandler.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include "MouseHandler.h"

void CameraController::init()
{
	_backBufferHDC = ::GetDC(G_hWnd);	
	_frontBufferImg = ImageManager::makeImage( {}, { 1.0f, 1.0f }, move(ScreenManager::getInstance().getMainResolution()));
}

void CameraController::update()
{	
	::InvalidateRect(G_hWnd, NULL, false); // 갱신해 주는 함수
}

void CameraController::render() const
{
	drawFrontBufferBy(_frontBufferImg.getMemDC());
	drawBackBuffer();
}

void CameraController::release()
{
	DeleteDC(_backBufferHDC);
}

void CameraController::resetResolutionOfFrontBuffer(POINT startPosition, POINT printSize)
{
	ImageManager::resetImage(_frontBufferImg, move(startPosition), move(printSize), move(printSize));
	MouseHandler::resetPositionOfText();
}

void CameraController::drawFrontBufferBy(const HDC& hdc) const
{	
	::PatBlt(hdc, 0, 0, _frontBufferImg.getImagePrintSize().x, _frontBufferImg.getImagePrintSize().y, BLACKNESS);

	SceneManager::render(hdc);
	TimeHandler::getInstance().render(hdc);
	MouseHandler::render(hdc);
}

// front의 memHDC에 그려진 정보를 다시 백버퍼 HDC에 옮기는 작업
void CameraController::drawBackBuffer() const 
{
	_frontBufferImg.render(_backBufferHDC, {});
}

