#include "VideoController.h"

void VideoController::init(uint16 capacity)
{
	_playListVec.reserve(capacity);
	_playingIdx = -1;
}

void VideoController::update(double deltaTime)
{
	switch (isPlayingVideo())
	{
	case true:
		updateCurrentVideo(deltaTime);
		break;
	case false:
		stopCurrentVideo(true);
		if (isNextPlay())
		{
			playNextVideo();
		}
		break;
	}
}

void VideoController::render(const HDC& hdc) const
{
	if (isPlayingVideo())
	{
		renderCurrentVideo(hdc);
	}
}

void VideoController::release()
{
	stopCurrentVideo(true);
 	_playListVec.clear();
	_playingIdx = -1;
}

void VideoController::resetPlayingVideo(POINT sp, POINT videoFileSize, POINT videoMaxFrame)
{
	getPlayingVideo().resetVideo(move(sp), move(videoFileSize), move(videoMaxFrame));
}

void VideoController::addVideo(const char* source, SOUND_FILE soundFile, POINT sp, FPOINT scale, POINT videoFileSize, POINT videoMaxFrame, double delta, uint16 frameCount, uint16 stopFrameSection, bool isReverse, bool isLoop, bool nextPlay)
{
	unique_ptr<Video> video = make_unique<Video>();
	video->addVideo(move(source), move(sp), move(scale), move(videoFileSize), move(videoMaxFrame), delta, frameCount, stopFrameSection, isReverse, isLoop, nextPlay);
	video->setSound(move(soundFile));

	_playListVec.emplace_back(move(video));
}

void VideoController::playVideoBy(const char* source, bool isStopMusic)
{
	assert(source);

	if (_playingIdx != -1 && isPlayingVideo())
	{
		stopCurrentVideo(isStopMusic);
	}

	_playingIdx = findIdxOfVideoBy(source);
	playCurrentVideo();
}

void VideoController::stopCurrentVideo(bool isStopMusic) const
{
	getPlayingVideo().stopVideo();
	if (isStopMusic)
		stopMusicCurrentVideo();
}

void VideoController::pauseCurrentVideo() const
{
	getPlayingVideo().pauseVideo();
	pauseMusicCurrentVideo();
}

void VideoController::resumeCurrentVideo() const
{
	getPlayingVideo().resumeVideo();
	resumeMusicCurrentVideo();
}

Video& VideoController::getPlayingVideo() const
{
	assert(_playingIdx >= 0);
	return *_playListVec[_playingIdx];
}

void VideoController::playCurrentVideo() const
{
	getPlayingVideo().playVideo();
	playMusicCurrentVideo();
}

void VideoController::playNextVideo()
{
	++_playingIdx;
	assert(_playingIdx < _playListVec.size());
	playCurrentVideo();
}

int16 VideoController::findIdxOfVideoBy(const char* source) const
{
	for (int i = 0; i < _playListVec.size(); i++)
	{
		if (compareSourceNameBy(_playListVec[i]->getImageSource(), source))
		{
			return i;
		}
	}

	assert(-1);
	return -1;
}


