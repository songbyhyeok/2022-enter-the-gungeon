#pragma once

#include "pch.h"

class Animation
{
public:
	void			init(int16 frameCount, POINT maxKeyFrame, POINT frameSize);

	void			setPlayFrame(uint16 frameStopSection = 0, bool isReverse = false, bool isLoop = false);
	void			setPlayFrame(vector<int16>& playList, bool isReverse = false, bool isLoop = false);
	void			setUpdateTime(double fps) { _keyFrameUpdateTime = 1.0 / fps; }
	const	POINT&  getCurrFramePos() const { return _frameList[_playList[_playingIdx]]; }

	bool			isPlaying() const { return _isPlaying; }
	bool			StopFrameSection() const { return _stopSectionFrame != 0 && _stopSectionFrame <= _playingIdx; }

	void			resetFrameList(POINT frameSize, POINT maxKeyFrame);
	void			updateKeyFrame(double deltaTime, const uint16 selectedYSize = 0);
	void			start();
	void			start(const uint16 playingIdx, const uint16 stopSectionFrame);
	void			stop();
	void			pause() { _isPlaying = false; }
	void			resume() { _isPlaying = true; }
	void			moveToStopFrameSection() { _playingIdx = _stopSectionFrame; }	
	
private:
	void			setPlayingIdx(const uint16 idx) { _playingIdx = idx; }
	void			setStopSectionFrame(const uint16 stopSectionFrame) { _stopSectionFrame = stopSectionFrame; }

private:
	vector<POINT>	_frameList;
	vector<int16>	_playList;
	POINT			_frameSize;
	double			_keyFrameUpdateTime;
	double			_accumulateTime;
	uint16			_stopSectionFrame;
	int16			_frameCount;
	int16			_playingIdx;
	bool			_isLoop;
	bool			_isPlaying;
};
