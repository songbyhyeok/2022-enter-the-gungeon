#pragma once

#include "Scene.h"

DWORD CALLBACK loadingThread(LPVOID pvParam = nullptr);

class SceneManager
{
public:   
    static void                                 update();
    static void                                 render(const HDC& hdc);
    static void                                 release(unique_ptr<Scene>& scene, bool isStopPlayingSound, bool isDeletingPtrData = true, bool isDeletingPtr = false);

    static const  unique_ptr<Scene>&            getRunningScene() { return RunningScene; }

    static void                                 allRelease();
    static void                                 requestOptionInit();
    static void                                 toTitleScene(const SCENE_NAME& newSceneName);
    static void                                 changeSceneByReadyScene(const SCENE_NAME &newSceneName = SCENE_NAME::END);
    static void                                 changeScene(const SCENE_NAME& newSceneName);
    static void                                 changeScene(const SCENE_NAME& newSceneName, const SCENE_NAME& newLoadingSceneName);
    static void                                 swapReadySceneAndOptionScene();

private:
    static bool                                 checkScene(const unique_ptr<Scene>& scene);
    static void                                 swapSceneBy(unique_ptr<Scene>& fromScene, unique_ptr<Scene>& toScene);
    static void                                 moveSceneBy(unique_ptr<Scene>& fromScene, unique_ptr<Scene>& toScene);
    static unique_ptr<Scene>                    createSceneBy(const SCENE_NAME& sceneName);
    static unique_ptr<Scene>                    findScene(SCENE_NAME sceneName);
    static void                                 addScene(SCENE_NAME sceneName, unique_ptr<Scene>& scene);
    static void                                 deleteScene(SCENE_NAME sceneName);
    static void                                 allDeleteBlockedScenes();
    static void                                 controlRunningSceneMusic(bool isExecuting) { isExecuting == true ? RunningScene->resumeMusic() : RunningScene->pauseMusic(); }
    static bool                                 compareSceneNameBy(const SCENE_NAME& sceneNameA, const SCENE_NAME& sceneNameB) { return sceneNameA == sceneNameB; }

public:
    static map<SCENE_NAME, unique_ptr<Scene>>	BlockedScenes;
    static unique_ptr<Scene>                    RunningScene;
    static unique_ptr<Scene>                    ReadyScene;
};