#pragma once

#include "Singleton.h"
#include "BaseInterface.h"
#include "SoundController.h"
#include "AnimationManager.h"

class EffectManager : public Singleton<EffectManager>, public BaseInterface
{
public:
	void					init(SOUND_FILE splashingSoundFile, const FPOINT& scale);
	void					update() override;
	void					render(const HDC& hdc) const override;
	void					release() override {};

	void					setHitAniSP(POINT sp) { _hitAni.image.setStartPosition(move(sp)); }

	void					playHitAni() { _hitAni.anim.start(); }
	void					playSplashingSound() { SoundController::getInstance().play(_splashingSoundFile); }
	void					stopSplashingSound() { SoundController::getInstance().stop(_splashingSoundFile); }

private:
	ANIMATION_SET			_hitAni;
	SOUND_FILE				_splashingSoundFile;
};

