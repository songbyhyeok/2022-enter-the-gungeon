#include "Video.h"
#include "AnimationManager.h"
#include "ScreenManager.h"

void Video::update(double deltaTime)
{
	_ani.updateKeyFrame(deltaTime);
}

void Video::render(const HDC& hdc) const
{
	renderAnimation(hdc, { 0, 0 });
}

void Video::resetVideo(POINT startPosition, POINT printSize, POINT videoMaxFrame)
{
	ImageManager::resetImage(_image, move(startPosition), move(printSize));
	//AnimationManager::resetFrameListOfAnimation(_ani, _image.getFrameSize(), move(videoMaxFrame));
}

void Video::addVideo(const char* source, POINT sp, FPOINT scale, POINT videoFileSize, POINT videoMaxFrame, double delta, uint16 frameCount, uint16 stopFrameSection, bool isReverse, bool isLoop, bool nextPlay)
{
	init();
	_image = ImageManager::makeImage(move(source), move(sp), move(scale), move(videoFileSize), videoMaxFrame);
	_image.setPrintSize(move(ScreenManager::getInstance().getSubResolution()));
	_ani = AnimationManager::makeAnimation(_image.getFrameSize(), move(videoMaxFrame), delta, frameCount, stopFrameSection, isReverse, isLoop);
	_isNextPlay = nextPlay;
}

void Video::init()
{
	_soundFile = { "", SOUND_TYPE::END };
	_isNextPlay = false;
}

void Video::renderAnimation(const HDC& hdc, const POINT& dest) const
{
	if (isPlaying())
	{
		_image.renderAnimation(hdc, dest, getCurrentPlayingPosition());
	}
}
