#include "EffectManager.h"

void EffectManager::init(SOUND_FILE splashingSoundFile, const FPOINT& scale)
{	
	const POINT hitAniSize = { 30, 30 };
	_hitAni.image = ImageManager::makeImage("Images/EffectImage/Hit.bmp", {}, scale, { hitAniSize.x, hitAniSize.y * 2 }, { 1, 2 }, true, { false, false }, MagentaRGB);
	_hitAni.anim = AnimationManager::makeAnimation(hitAniSize, { 1, 2 }, 6.5, 0, 1, false, false);
	_hitAni.frameSpeed = 0.06f;
	_hitAni.frameLength = 0;
	_hitAni.soundFile = { "", SOUND_TYPE::END };

	_splashingSoundFile =  move(splashingSoundFile);
}

void EffectManager::update()
{
	if (_hitAni.anim.isPlaying())
		_hitAni.anim.updateKeyFrame(_hitAni.frameSpeed, _hitAni.frameLength);
}

void EffectManager::render(const HDC& hdc) const
{
	if (_hitAni.anim.isPlaying())
		_hitAni.image.renderAnimation(hdc, {}, _hitAni.anim.getCurrFramePos());
}
