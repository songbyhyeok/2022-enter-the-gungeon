#pragma once

#include "Video.h"

class VideoController
{
public:
    void                            init(uint16 capacity);
    void                            update(double deltaTime = 0.0);
    void                            render(const HDC& hdc) const;
    void                            release();

    const char*                     getCurrentVideoName() const { return getPlayingVideo().getImageSource(); }    

    bool                            isStopSection() const { return getPlayingVideo().isStopSection(); }
    bool                            compareCurrentVideoWith(const char* source) const { return compareSourceNameBy(getCurrentVideoName(), source); };

    void                            resetPlayingVideo(POINT sp, POINT videoFileSize, POINT videoMaxFrame);
    void                            addVideo(const char* source, SOUND_FILE soundFile, POINT sp, FPOINT scale, POINT videoFileSize, POINT videoMaxFrame, double delta, uint16 frameCount, uint16 stopFrameSection, bool isReverse, bool isLoop, bool nextPlay);
    void                            playVideoBy(const char* source, bool isStopMusic = false);
    void                            stopCurrentVideo(bool isStopMusic = false) const;
    void                            pauseCurrentVideo() const;
    void                            resumeCurrentVideo() const;
    void                            skipToVideoStopSectionBy(const char* source) { findVideoBy(findIdxOfVideoBy(source)).skipToStopSection(); }
    void                            playMusicCurrentVideo() const { getPlayingVideo().playSoundVideo(); }
    void                            stopMusicCurrentVideo() const { getPlayingVideo().stopSoundVideo(); }
    void                            pauseMusicCurrentVideo() const { getPlayingVideo().pauseSoundVideo(); }
    void                            resumeMusicCurrentVideo() const { getPlayingVideo().resumeSoundVideo(); }    

private:
    Video&                          getPlayingVideo() const;

    bool                            isPlayingVideo() const { return getPlayingVideo().isPlaying(); }
    bool                            isNextPlay() const { return getPlayingVideo().isNextPlay(); }

    void                            playCurrentVideo() const;
    void                            updateCurrentVideo(double deltaTime = 0.0) const { getPlayingVideo().update(deltaTime); }
    void                            renderCurrentVideo(const HDC& hdc) const { getPlayingVideo().render(hdc); };
    void                            playNextVideo();                 
    int16                           findIdxOfVideoBy(const char* source) const;
    Video&                          findVideoBy(int16 idx) const { return *_playListVec[idx]; }
    
private:
	vector<unique_ptr<Video>>       _playListVec;
	int16                           _playingIdx;
};

