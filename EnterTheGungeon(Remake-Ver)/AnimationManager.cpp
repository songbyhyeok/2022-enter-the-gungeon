#include "AnimationManager.h"

Animation AnimationManager::makeAnimation(POINT frameSize, POINT videoMaxFrame, double delta, uint16 frameCount, uint16 frameStopSection, bool isReverse, bool isLoop)
{
	Animation ani;
	ani.init(frameCount, move(videoMaxFrame), move(frameSize));
	ani.setUpdateTime(delta);
	ani.setPlayFrame(frameStopSection, isReverse, isLoop);
	return ani;
}
