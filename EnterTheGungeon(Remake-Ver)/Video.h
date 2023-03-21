#pragma once

#include "ImageManager.h"
#include "Animation.h"
#include "SoundController.h"

class Video
{
public:
    void                    update(double deltaTime);
    void                    render(const HDC& hdc) const;

    void                    setSound(SOUND_FILE soundFile) { _soundFile = move(soundFile); }
    const POINT&            getCurrentPlayingPosition() const { return _ani.getCurrFramePos(); }
    const char*             getImageSource() const { return _image.getSource(); }

    bool                    isPlaying() const { return _ani.isPlaying(); }
    bool                    isNextPlay() const { return _isNextPlay; }
    bool                    isStopSection() const { return _ani.StopFrameSection(); }

    void                    resetVideo(POINT startPosition, POINT printSize, POINT videoMaxFrame);
    void                    addVideo(const char* source, POINT sp, FPOINT scale, POINT videoFileSize, POINT videoMaxFrame, double delta, uint16 frameCount, uint16 stopFrameSection, bool isReverse, bool isLoop, bool nextPlay);
    void                    releaseVideoImage() { ImageManager::findImageToDelete(_image.getIdentityNumber()); }
    void                    playVideo() { _ani.start(); }
    void                    stopVideo() { _ani.stop(); }    
    void                    pauseVideo() { _ani.pause(); }
    void                    resumeVideo() { _ani.resume(); }
    void                    skipToStopSection() { _ani.moveToStopFrameSection(); }
    void                    playSoundVideo() const { SoundController::getInstance().play(getSoundFile()); }
    void                    stopSoundVideo() const { SoundController::getInstance().stop(getSoundFile()); }
    void                    pauseSoundVideo() const { SoundController::getInstance().pause(getSoundFile()); }
    void                    resumeSoundVideo() const { SoundController::getInstance().resume(getSoundFile()); }

private:
    const   SOUND_FILE&     getSoundFile() const { return _soundFile; }

    void                    init();
    void                    renderAnimation(const HDC& hdc, const POINT& dest) const;

private:
    Image                   _image;
    Animation               _ani;
    SOUND_FILE              _soundFile;
    bool                    _isNextPlay;
};

