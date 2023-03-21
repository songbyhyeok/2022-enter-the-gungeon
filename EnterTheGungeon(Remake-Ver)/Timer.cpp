#include "Timer.h"
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

void Timer::init()
{
	_frameRate = 0;
	_fpsFrameCount = 0;
	_fpsTimeElapsed = 0;
	_seconds = 0;
	_minutes = 0;
	_hours = 0;
	_worldTime = 0.0f;
	_testTime = 0.0f;
	_lockFPS = 0.0f;

	initTestTime();

	_isHardware = false;

	switch (QueryPerformanceFrequency(reinterpret_cast<PLARGE_INTEGER>(&_frequency)))
	{
	case true:
		_isHardware = true;
		QueryPerformanceCounter(reinterpret_cast<PLARGE_INTEGER>(&_lastTime));	// 현재시간 (진동수의 누적값)
		break;
	case false:
		_isHardware = false;
		_lastTime = timeGetTime();
		break;
	}
}

void Timer::update()
{
	calculateCurrentTime();

	_timeElapsed = (_currTime - _lastTime) / static_cast<float>(_frequency); // 마지막 시간과 현재 시간의 경과량
	while (_timeElapsed < (1.0 / _lockFPS))
	{
		calculateCurrentTime();

		_timeElapsed = (_currTime - _lastTime) / static_cast<float>(_frequency);
	}
	_lastTime = _currTime;

	_worldTime += _timeElapsed;

	if (_stopwatchUse)
		_testTime += _timeElapsed;

	auto worldTime = floatToInt(_worldTime);	 
	_seconds = worldTime % 60;
	_minutes = (worldTime / 60) % 60;
	_hours = (worldTime / 60 / 60) % 60;

	++_fpsFrameCount;
	_fpsTimeElapsed += _timeElapsed;
	if (_fpsTimeElapsed > 1.0)
	{
		_frameRate = _fpsFrameCount;
		_fpsFrameCount = 0;
		_fpsTimeElapsed = 0;
	}
}

void Timer::initTestTime()
{
	_stopwatchUse = false;
	_testTime = 0.0f;
}

void Timer::calculateCurrentTime()
{
	switch (_isHardware)
	{
	case true:
		QueryPerformanceCounter(reinterpret_cast<PLARGE_INTEGER>(&_currTime));
		break;
	case false:
		_currTime = timeGetTime();
		break;
	}
}
