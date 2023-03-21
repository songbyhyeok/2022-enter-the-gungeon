#pragma once

#include "pch.h"

class Timer
{
public:
	void			init();
	void			update();

	void			setFPS(float fps) { _lockFPS = fps; }
	void			setStopWatchUse(bool use) { _stopwatchUse = use; }
	int64			getFrameRate() const { return _frameRate; }
	float			getWorldTime() const { return _worldTime; }
	float			getDeltaTime() const { return _timeElapsed; }
	float			getTestTime() const { return _testTime; }
	uint64			getSeconds() const { return _seconds; }
	uint64			getMinutes() const { return _minutes; }
	uint64			getHours() const { return _hours; }

	void			initTestTime();
	
private:
	void			calculateCurrentTime();

private:
	uint64			_seconds;
	uint64			_minutes;
	uint64			_hours;
	int64			_currTime;			// 현재 시간 (고성능 타이머 값)
	int64			_lastTime;			// 이전 시간 (고성능 타이머 값)
	int64			_frequency;			// 고성능 타이머의 주파수 (초당 진동 수), 지원하지 않으면 0 반환
	int64			_frameRate;
	int64			_fpsFrameCount;
	float			_timeScale;			// 경과 진동 수를 초당 진동 수로 나누어서 진동 수를 시간 단위로 변환
	float			_timeElapsed;
	float			_fpsTimeElapsed;
	float			_worldTime;
	float			_testTime;
	float			_lockFPS;
	bool			_isHardware;		// 고성능 타이머를 쓸 수 있는지 여부
	bool			_stopwatchUse;
};

