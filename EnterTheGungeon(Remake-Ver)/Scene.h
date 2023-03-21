#pragma once

#include "Button.h"

enum class SCENE_NAME
{
    GAME,
    MAPTOOL,
    OPTION,
    TITLE,
    LOADING,
    END
};

class Scene 
{
public:
    virtual bool                        init();
    virtual void                        update(float deltaTime = 0.0);
    virtual void                        render(const HDC& hdc) const = 0;
    virtual void                        release() { _isModifiable = false; }

    const SCENE_NAME&                   getSceneID() const { return _sceneID; }

    bool                                isModifiable() const { return _isModifiable; }

    virtual void                        reset() = 0;
    virtual void                        startScene() = 0;
    virtual void                        stopCurrentSound() const = 0;
    void                                pauseMusic() const { SoundController::getInstance().pause(_soundFile); }
    void                                resumeMusic() const { SoundController::getInstance().resume(_soundFile); }

    virtual                             ~Scene() = default;

protected:
    void                                setSoundFile(SOUND_FILE soundFile) { _soundFile = move(soundFile); }

protected:
    SOUND_FILE                          _soundFile;
    FPOINT                              _scale;
    POINT                               _startPosition;
    SCENE_NAME                          _sceneID;
    bool                                _isModifiable;
};

#include "World.h"
class GameScene : public Scene
{
public:
    GameScene() 
    { 
        _sceneID = SCENE_NAME::GAME;
        _isModifiable = false;
    }

    bool                        init() override;
    void                        update(float deltaTime = 0.0) override;
    void                        render(const HDC& hdc) const override;
    void                        release() override;

    void                        reset() override {};
    void                        startScene() override { SoundController::getInstance().play(_soundFile); }
    void                        stopCurrentSound() const override {};
    void                        setGamma(const int16 gammaCount);
    void                        setGammaCount(const int16 gammaCount) { _gammaCount = gammaCount; }

private:
    void                        moveToPreviousScene() const;

private:
    World                       _world;    
    int16                       _gammaCount;
};

#include "MapTool.h"
class MapToolScene : public Scene
{
public:
    MapToolScene()
    { 
        _sceneID = SCENE_NAME::MAPTOOL;
        _isModifiable = false;
    }

    bool                        init() override;
    void                        update(float deltaTime = 0.0) override;
    void                        render(const HDC& hdc) const override;
    void                        release() override;

    void                        reset() override;
    void                        startScene() override { SoundController::getInstance().play(_soundFile); }
    void                        stopCurrentSound() const override {};
    void                        setGamma(const int16 gammaCount);
    void                        setGammaCount(const int16 gammaCount) { _gammaCount = gammaCount; }

private:
    void                        moveToPreviousScene() const;

private:
    MapTool                     _mapTool;
    int16                       _gammaCount;
};

#include "Option.h"

class OptionScene : public Scene
{
public:
    OptionScene() 
    { 
        _sceneID = SCENE_NAME::OPTION;
        _isModifiable = false;
    }

    bool                        init() override;
    void                        update(float deltaTime = 0.0) override;
    void                        render(const HDC& hdc) const override;
    void                        release() override;

    void                        setScenePtrFromOption(Scene* scenePtr) { _option.setScenePtr(scenePtr); }

    void                        requestToMakeTitleButton() { _option.requestToMakeTitleButtonByBaseMenu(); }
    void                        requestToDeleteTitleButton() { _option.requestToDeleteTitleButtonByBaseMenu(); }
    void                        notifyToScenePartner(const char* sceneName) { _option.setCommonSceneName(move(sceneName)); }

    void                        reset() override;
    void                        startScene() override {};
    void                        stopCurrentSound() const override {};
    void                        setDataChangeRecode(bool isKeepDataChangeRecord) { _option.setDataChangeRecode(isKeepDataChangeRecord); }
    bool                        isDataRecorded() const { return _option.isDataChanged(); }
    uint16                      getGammaCount() const { return _option.getVideoMenuGammaCount(); }

private:
    void                        moveToPreviousScene() const;

private:
    Option                      _option;
}; 

constexpr float TitleGammaValue = 12.75;

#include "VideoController.h"
class TitleScene : public Scene
{
public:
    TitleScene() 
    { 
        _sceneID = SCENE_NAME::TITLE;
        _isModifiable = false;
    }

    bool                                init() override;
    void                                update(float deltaTime = 0.0) override;
    void                                render(const HDC& hdc) const override;
    void                                release() override;

    void                                reset() override;
    void                                startScene() override;
    void                                stopCurrentSound() const override { _videoCtr.stopMusicCurrentVideo(); };
    void                                setPlaySpeedOfOpeningVideo(float playSpeed) { _openingPlaySpeed = playSpeed; }
    float                               getToBaseSpeed() const { return 7.5f; }
    float                               getToLoadingSpeed() const { return 13.5f; }
    void                                setGamma(const int16 gammaCount);
    void                                setGammaCount(const int16 gammaCount) { _gammaCount = gammaCount; }

private:                                   
    void                                initImage();
    void                                initButtons();
    void                                initVideos();
    void                                resetImage();
    void                                resetButtons();
    void                                resetPlayingVideoImage() { _videoCtr.resetPlayingVideo(_startPosition, move(ScreenManager::getInstance().getSubResolution()), { 7, 11 }); }
    void                                skipToVideoStopSectionBy(const char* source) { _videoCtr.skipToVideoStopSectionBy(move(source)); }
    void                                skipOpeningVideo();
    bool                                isTitleFrameSection() const { return _videoCtr.isStopSection(); }                          

private:
    Button                              _playButton;
    Button                              _maptoolButton;
    Button                              _optionButton;
    Button                              _exitButton;
    Image                               _gamma;
    Image                               _titleName;
    VideoController                     _videoCtr;
    float                               _openingPlaySpeed; 
    float                               _gammaCount;
};

class LoadingScene : public Scene
{
public:
    LoadingScene() 
    { 
        _sceneID = SCENE_NAME::LOADING;
        _isModifiable = false;
    }

    bool                        init() override;
    void                        update(float deltaTime = 0.0) override;
    void                        render(const HDC& hdc) const override;
    void                        release() override;

    void                        initVideo();
    void                        reset() override;
    void                        startScene() override { _videoCtr.playVideoBy("Images/Scene/Loading/Loading.bmp"); };
    void                        stopCurrentSound() const override { _videoCtr.stopMusicCurrentVideo(); }

    void                        setMinLoadingTime(float time) { _minLoadingTime = time; }
    float                       getMinLoadingTime() const { return _minLoadingTime; }

private:
    void                        resetVideo();

private:
    VideoController             _videoCtr;
    float                       _minLoadingTime;
};