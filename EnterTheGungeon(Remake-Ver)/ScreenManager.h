#pragma once

#include "Singleton.h"

enum class ResolutionList
{
	W640H480,
	W800H600,
	W1024H768,
	W1280H720,
	W1920H1080,
	W2560H1080,
	END
};

enum class ScalingModeList
{
	FORCEPIXELPERFECT,
	FASTSCALING,
	END
};

enum class ScreenModeList
{
	FULLSCREEN,
	WINDOWED,
	END
};

class ScreenContents
{
public:	
	static uint16					toInt(ResolutionList resolution) { return static_cast<uint16>(resolution); }
	static uint16					toInt(ScalingModeList scalingMode) { return static_cast<uint16>(scalingMode); }
	static uint16					toInt(ScreenModeList screenMode) { return static_cast<uint16>(screenMode); }
	static POINT					toPoint(const ResolutionList &resolutionValue);
	static ResolutionList			toResolution(const POINT& resolution);
	static ResolutionList			toResolution(int16 movedValue) { return static_cast<ResolutionList>(movedValue); }
	static ScalingModeList			toScalingMode(int16 movedValue) { return static_cast<ScalingModeList>(movedValue); }
	static ScreenModeList			toScreenMode(int16 movedValue) { return static_cast<ScreenModeList>(movedValue); }
	static const char*				toConstCharPtr(ResolutionList resolution);
	static const char*				toConstCharPtr(ScalingModeList scalingMode);
	static const char*				toConstCharPtr(ScreenModeList screenMode);

private:
	
	
private:
	
};

struct ASPECT
{
	LONG	x;
	LONG	y;
	FPOINT	ratio;
};

class CameraController;
class ScreenManager : public Singleton<ScreenManager>
{
public:
	void							init(bool scalingModeValue, bool screenModeValue);

	void							setCameraController(const shared_ptr<CameraController>& cameraController) { _cameraController = cameraController; }

	void							setMainAspect(POINT resolutionApply);
	void							setSubAspect(POINT resolutionApply);
	const ASPECT&					getMainAspect() const { return _mainAspect; }
	POINT							getMainResolution() const { return move(POINT{ _mainAspect.x, _mainAspect.y }); }
	const ASPECT&					getSubAspect() const { return _subAspect; }
	POINT							getSubResolution() const { return move(POINT{ _subAspect.x, _subAspect.y }); }
	void							setSubResolutionValue(POINT resolution) { _subResolution = move((ScreenContents::toResolution(move(resolution)))); }
	ResolutionList					getSubResolutionValue() const { return _subResolution; }
	bool							getScalingMode() const { return _isScalingMode; }
	bool							getScreenMode() const { return _isFullScreen; }
	const FPOINT&					getCalculatedScale() const { return _calculatedScale; }

	void							initScreen();
	void							changeResolution(ResolutionList resolutionValue);
	void							changeScalingMode(ScalingModeList scalingModeValue);
	void							changeScreenMode(ScreenModeList screenModeValue);
	FPOINT							calculateScaleOf(POINT relativeResolution, POINT fixedResolution) const { return move(FPOINT{ static_cast<float>(relativeResolution.x) / fixedResolution.x, static_cast<float>(relativeResolution.y) / fixedResolution.y }); }
	POINT							generateToResolution(ResolutionList selectedResolution);
	POINT							generateHalfOfResolution(const POINT& resolution) const { return move(POINT{ resolution.x / 2, resolution.y / 2 }); }
	POINT							generateStartPositionOfScreen() const;
	POINT							generateStartPositionOfScreen(const POINT& pos1, const POINT& pos2) const { return { (pos1.x / 2) - (pos2.x / 2), (pos1.y / 2) - (pos2.y / 2) }; }

private:
	void							setScalingMode(bool scalingModeValue) { _isScalingMode = scalingModeValue; }
	void							setScreenMode(bool screenModeValue) { _isFullScreen = screenModeValue; }

	void							changeSubResolutionToFitScreen(FPOINT ratio);
	FPOINT							calculateRatioOfAspect(POINT convertedResolution) const;	
	void							scaleUpOfSubScreen(float scaleValue);
	
private:
	shared_ptr<CameraController>	_cameraController;
	ASPECT							_mainAspect;
	ASPECT							_subAspect;
	FPOINT							_calculatedScale;
	ResolutionList					_subResolution;
	bool							_isScalingMode;
	bool							_isFullScreen;
};

