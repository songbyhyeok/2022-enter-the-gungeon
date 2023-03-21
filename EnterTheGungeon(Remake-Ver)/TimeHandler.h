#pragma once

#include "Timer.h"
#include "Singleton.h"
#include "TextHandler.h"

class TimeHandler : public Singleton<TimeHandler>
{
public:
	void							init();
	void							initTextHandlers();
	void							update();
	void							render(const HDC& hdc) const;

	void							setFPS(float fps) { _timer.setFPS(fps); }
	uint64							getSeconds() const { return _timer.getSeconds(); }
	uint64							getFrameRate() const { return _timer.getFrameRate(); }
	float							getWorldTime() const { return _timer.getWorldTime(); }
	float							getDeltaTime() const { return _timer.getDeltaTime(); }
	float							getStopwatchTime() const { return _timer.getTestTime(); }

	bool							startAlarm(float fixedTime);
	void							startStopwatch() { _timer.setStopWatchUse(true); }
	void							stopStopwatch() { _timer.initTestTime(); }
	

private:
	std::array<TextHandler, 3>		_textHandlers;
	Timer							_timer;
};


