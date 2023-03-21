#pragma once

#include "Animation.h"
#include "ImageManager.h"
#include "SoundController.h"

struct ANIMATION_SET
{
	Image			image;
	Animation		anim;
	SOUND_FILE		soundFile;
	float			frameSpeed;
	uint16			frameLength;
};

class AnimationManager
{
public:
	static Animation makeAnimation(POINT frameSize, POINT videoMaxFrame, double delta, uint16 frameCount, uint16 frameStopSection, bool isReverse, bool isLoop);
	static void		 resetFrameListOfAnimation(Animation& ani, POINT frameSize, POINT maxKeyFrame) { ani.resetFrameList(move(frameSize), move(maxKeyFrame)); }
};

