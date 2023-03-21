#include "ScreenManager.h"
#include "CameraController.h"

POINT ScreenContents::toPoint(const ResolutionList& resolutionValue)
{
	POINT appliedResolution{0,0};
	switch (resolutionValue)
	{
	case ResolutionList::W2560H1080:
		appliedResolution = { 2560, 1080 };
		break;
	case ResolutionList::W1920H1080:
		appliedResolution = { 1920, 1080 };
		break;
	case ResolutionList::W1280H720:
		appliedResolution = { 1280, 720 };
		break;
	case ResolutionList::W1024H768:
		appliedResolution = { 1024, 768 };
		break;
	case ResolutionList::W800H600:
		appliedResolution = { 800, 600 };
		break;
	case ResolutionList::W640H480:
		appliedResolution = { 640, 480 };
		break;
	default:
		assert(0);
		break;
	}

	return appliedResolution;
}

ResolutionList ScreenContents::toResolution(const POINT& resolution)
{
	ResolutionList appliedResolutionValue{ ResolutionList::END };
	if (resolution.x == 2560 && resolution.y == 1080 || resolution.x == 2540 && resolution.y == 1060)
	{
		appliedResolutionValue = ResolutionList::W2560H1080;
	}
	else if (resolution.x == 1920 && resolution.y == 1080)
	{
		appliedResolutionValue = ResolutionList::W1920H1080;
	}
	else if (resolution.x == 1280 && resolution.y == 720)
	{
		appliedResolutionValue = ResolutionList::W1280H720;
	}
	else if (resolution.x == 1024 && resolution.y == 768)
	{
		appliedResolutionValue = ResolutionList::W1024H768;
	}
	else if (resolution.x == 800 && resolution.y == 600)
	{
		appliedResolutionValue = ResolutionList::W800H600;
	}
	else if (resolution.x == 640 && resolution.y == 480)
	{
		appliedResolutionValue = ResolutionList::W640H480;
	}
	else
	{
		assert(0);
	}

	return appliedResolutionValue;
}

const char* ScreenContents::toConstCharPtr(ResolutionList resolution)
{
	const char* newContentsName = "";
	switch (resolution)
	{
	case ResolutionList::W2560H1080:
		newContentsName = "2560x1080(64:27)";
		break;
	case ResolutionList::W1920H1080:
		newContentsName = "1920x1080(16:9)";
		break;
	case ResolutionList::W1280H720:
		newContentsName = "1280x720(16:9)";
		break;
	case ResolutionList::W1024H768:
		newContentsName = "1024x768(4:3)";
		break;
	case ResolutionList::W800H600:
		newContentsName = "800x600(4:3)";
		break;
	case ResolutionList::W640H480:
		newContentsName = "640x480(4:3)";
		break;
	}

	return newContentsName;
}

const char* ScreenContents::toConstCharPtr(ScalingModeList scalingMode)
{
	const char* newContentsName = "";
	switch (scalingMode)
	{
	case ScalingModeList::FORCEPIXELPERFECT:
		newContentsName = "ForcePixelPerfect";
		break;
	case ScalingModeList::FASTSCALING:
		newContentsName = "FastScaling";
		break;
	}

	return newContentsName;
}

const char* ScreenContents::toConstCharPtr(ScreenModeList screenMode)
{
	const char* newContentsName = "";
	switch (screenMode)
	{
	case ScreenModeList::FULLSCREEN:
		newContentsName = "Fullscreen";
		break;
	case ScreenModeList::WINDOWED:
		newContentsName = "Windowed";
		break;
	}

	return newContentsName;
}

void ScreenManager::init(bool scalingModeValue, bool screenModeValue)
{
	_mainAspect = {0, 0};
	_subAspect = {0, 0};
	_subResolution = ResolutionList::END;
	_calculatedScale = {1.0f, 1.0f};
	
	setScalingMode(scalingModeValue);
	setScreenMode(screenModeValue);
}

void ScreenManager::setMainAspect(POINT resolutionApply)
{
	_mainAspect = { resolutionApply.x, resolutionApply.y, calculateRatioOfAspect(move(resolutionApply)) };
}

void ScreenManager::setSubAspect(POINT resolutionApply)
{
	_subAspect = { resolutionApply.x, resolutionApply.y, calculateRatioOfAspect(move(resolutionApply)) };	
	const POINT FixedResolution{1920, 1080};
	_calculatedScale = calculateScaleOf(move(getSubResolution()), FixedResolution);
}

void ScreenManager::initScreen()
{
	if (getScalingMode())
	{
		changeScalingMode(ScalingModeList::FASTSCALING);
	}

	switch (getScreenMode())
	{
	case true:
		G_hWnd = CreateWindowEx(WS_EX_APPWINDOW, G_szWindowClass, G_szWindowClass, WS_POPUPWINDOW,
			0, 0, getMainResolution().x, getMainResolution().y, nullptr, nullptr, G_hInstance, nullptr);
		break;
	case false:
		POINT systemMatrix{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
		POINT windowStartPoint = move(generateStartPositionOfScreen(move(systemMatrix), move(getMainResolution())));
		POINT changedAspect{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
		if (_mainAspect.x == GetSystemMetrics(SM_CXSCREEN))
		{
			windowStartPoint = { 10, 10 };
			changedAspect = { GetSystemMetrics(SM_CXSCREEN) - 20, GetSystemMetrics(SM_CYSCREEN) - 20 };
			setMainAspect(changedAspect);
			setSubAspect({ getSubResolution().x - 20, getSubResolution().y - 20 });
		}

		// WS_OVERLAPPED
		G_hWnd = CreateWindowEx(WS_EX_APPWINDOW, G_szWindowClass, G_szWindowClass, WS_POPUPWINDOW, windowStartPoint.x, windowStartPoint.y, changedAspect.x, changedAspect.y, nullptr, nullptr, G_hInstance, nullptr);
		break;
	}

	assert(G_hWnd);
}

void ScreenManager::changeResolution(ResolutionList resolutionValue)
{
	assert(resolutionValue != ResolutionList::END);
	setSubAspect(generateToResolution(move(resolutionValue)));

	switch (getScalingMode() == true)
	{
	case true:
		changeScalingMode(ScalingModeList::FASTSCALING);
		break;
	case false:
		setSubResolutionValue(move(getSubResolution()));
		break;
	}

	switch (getScreenMode())
	{
	case true:
		break;
	case false:
		changeScreenMode(ScreenModeList::WINDOWED);
	}
}

void ScreenManager::changeScalingMode(ScalingModeList scalingModeValue)
{
	assert(scalingModeValue != ScalingModeList::END);
	(scalingModeValue == ScalingModeList::FASTSCALING) ? setScalingMode(true) : setScalingMode(false);
	/*
	* FASTSCALING -> 가로 세로 비율이 올바르며 시스템 해상도에 맞춰 스케일링 처리 (스크린 픽셀이 현재 늘어난 형태로 보인다.)
	* FORCEPIXELPERFECT -> 디자이너가 의도한 대로 해상도 설정
	*/
	switch (getScalingMode())
	{
	case true:
		changeSubResolutionToFitScreen(_mainAspect.ratio);
		scaleUpOfSubScreen(1.0f);

		break;
	case false:
		setSubAspect(move(move(ScreenContents::toPoint(_subResolution))));
		break;
	}
}

void ScreenManager::changeScreenMode(ScreenModeList screenModeValue)
{
	POINT systemMatrix{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };

	assert(screenModeValue != ScreenModeList::END);
	(screenModeValue == ScreenModeList::FULLSCREEN) ? setScreenMode(true) : setScreenMode(false);
	switch (getScreenMode())
	{
	case true:
		setMainAspect(move(systemMatrix));
		_cameraController->resetResolutionOfFrontBuffer({ 0, 0 }, move(getMainResolution()));

		MoveWindow(G_hWnd, 0, 0, getMainResolution().x, getMainResolution().y, false);
		break;
	case false:
		setMainAspect(move(ScreenContents::toPoint(_subResolution)));
		_cameraController->resetResolutionOfFrontBuffer({ 0, 0 }, move(getMainResolution()));

		POINT windowStartPoint = move(generateStartPositionOfScreen(move(systemMatrix), move(getMainResolution())));
		POINT changedAspect{ move(getMainResolution()) };
		if (_mainAspect.x == GetSystemMetrics(SM_CXSCREEN))
		{
			windowStartPoint = { 10, 10 };
			changedAspect = { changedAspect.x - 20, changedAspect.y - 20 };
			setMainAspect(changedAspect);
			setSubAspect({ getSubResolution().x - 20, getSubResolution().y - 20 });
		}

		MoveWindow(G_hWnd, windowStartPoint.x, windowStartPoint.y, changedAspect.x, changedAspect.y, false);
		break;
	}
}

POINT ScreenManager::generateToResolution(ResolutionList selectedResolution)
{
	POINT		convertedResolution{ 0, 0 };
	switch (selectedResolution)
	{
	case ResolutionList::W2560H1080:
		convertedResolution = { 2560, 1080 };
		break;
	case ResolutionList::W1920H1080:
		convertedResolution = { 1920, 1080 };
		break;
	case ResolutionList::W1280H720:
		convertedResolution = { 1280, 720 };
		break;
	case ResolutionList::W1024H768:
		convertedResolution = { 1024, 768 };
		break;
	case ResolutionList::W800H600:
		convertedResolution = { 800, 600 };
		break;
	case ResolutionList::W640H480:
		convertedResolution = { 640, 480 };
		break;
	case ResolutionList::END:
		convertedResolution = { 0, 0 };
		break;
	}

	return move(convertedResolution);
}

POINT ScreenManager::generateStartPositionOfScreen() const
{
	const POINT& halfOfMainScreen = move(generateHalfOfResolution(getMainResolution()));
	const POINT& halfOfSubResolution = move(generateHalfOfResolution(getSubResolution()));
	return move(POINT{ halfOfMainScreen.x - halfOfSubResolution.x, halfOfMainScreen.y - halfOfSubResolution.y });
}

void ScreenManager::changeSubResolutionToFitScreen(FPOINT ratio)
{
	setSubResolutionValue(getSubResolution());

	const FPOINT fixedScale{ 16, 9 };
	FPOINT calculatedScale;
	calculatedScale.y = ((float)_subAspect.x / _subAspect.y) / (fixedScale.x / fixedScale.y);
	calculatedScale.x = 1.0f / calculatedScale.y;
	/*
	* scaleHeight 값이 1보다 작은 경우 날씬한 경우
							큰 경우 라면 뚱뚱한 경우
	*/
	POINT appliedResolution = getSubResolution();
	switch (calculatedScale.y < 1.0f)
	{
	case true:
		appliedResolution.y = _subAspect.y * calculatedScale.y;
		break;
	case false:
		appliedResolution.x = _subAspect.x * calculatedScale.x;
		break;
	}

	setSubAspect(move(appliedResolution));
}

FPOINT ScreenManager::calculateRatioOfAspect(POINT convertedResolution) const
{
	const uint16 gcd = (convertedResolution.x > convertedResolution.y) ? euclidean(convertedResolution.x, convertedResolution.y) : euclidean(convertedResolution.y, convertedResolution.x);
	FPOINT aspectRatio{ static_cast<float>(convertedResolution.x / gcd), static_cast<float>(convertedResolution.y / gcd) };
	return aspectRatio;
}

void ScreenManager::scaleUpOfSubScreen(float scaleValue)
{
	if (_subResolution == ResolutionList::W2560H1080 || _subResolution == ResolutionList::W1920H1080)
		return;

	POINT calculatedResolution{ _subAspect.x * scaleValue, _subAspect.y * scaleValue };
	setSubAspect(move(calculatedResolution));
}