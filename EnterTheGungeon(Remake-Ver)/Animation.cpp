#include "Animation.h"

void Animation::init(int16 frameCount, POINT maxKeyFrame, POINT frameSize)
{
	_keyFrameUpdateTime = 0.0;
	_accumulateTime = 0.0;
	_frameSize = { 0, 0 };
    _stopSectionFrame = 0;
	_playingIdx = 0;
	_isLoop = false;
	_isPlaying = false;

	_frameCount = frameCount == 0 ? maxKeyFrame.x * maxKeyFrame.y : frameCount;
	_frameList.resize(_frameCount);	
	for (int i = 0; i < _frameCount; i++)
	{
		_frameList[i].x = (i % maxKeyFrame.x) * frameSize.x;
        _frameList[i].y = (i / maxKeyFrame.x) * frameSize.y;
	}
}

void Animation::setPlayFrame(uint16 frameStopSection, bool isReverse, bool isLoop)
{
    _isLoop = isLoop;
    _stopSectionFrame = frameStopSection;

    _playList.clear();
    _playList.resize(_frameCount);
    for (int i = 0; i < _frameCount; i++)
    {
        _playList[i] = i;
    }

    if (isReverse)
    {
        for (int i = _frameCount - 1; i >= 0; i--)
        {
            _playList[i] = i;
        }
    }
}

void Animation::setPlayFrame(vector<int16>& playList, bool isReverse, bool isLoop)
{
    _isLoop = isLoop;

    _playList.clear();

    _playList = move(playList);
    if (isReverse)
    {
        for (int i = _playList.size() - 1; i >= 0; i--)
        {
            _playList.emplace_back(i);
        }
    }
}

void Animation::resetFrameList(POINT frameSize, POINT maxKeyFrame)
{
    for (int i = _stopSectionFrame; i < _frameCount; i++)
    {
        _frameList[i].x = (i % maxKeyFrame.x) * frameSize.x;
        _frameList[i].y = (i / maxKeyFrame.x) * frameSize.y;
    }
}

void Animation::updateKeyFrame(double deltaTime, const uint16 selectedYSize)
{
    if (_isPlaying)
    {
        _accumulateTime += deltaTime;

        if (_accumulateTime >= _keyFrameUpdateTime)
        {
            ++_playingIdx;
            const uint16 selectedSize = (selectedYSize == 0) ? _playList.size() : selectedYSize;
            if (_playingIdx >= selectedSize)
            {
                switch (_isLoop)
                {
                case true:
                    _playingIdx = _stopSectionFrame;
                    break;
                case false:
                    //--_playingIdx;
                    _playingIdx = _stopSectionFrame;
                    _isPlaying = false;
                    break;
                }
            }

            _accumulateTime -= _keyFrameUpdateTime;
        }
    }
}

void Animation::start()
{
    setPlayingIdx(0);
    resume();
}

void Animation::start(const uint16 playingIdx, const uint16 stopSectionFrame)
{
    setPlayingIdx(playingIdx);
    setStopSectionFrame(stopSectionFrame);
    resume();
}

void Animation::stop()
{
    setPlayingIdx(0);
    pause();
}
