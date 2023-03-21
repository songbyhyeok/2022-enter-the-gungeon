#include "SceneManager.h"
#include "TimeHandler.h"
#include "KeyController.h"

unique_ptr<Scene> SceneManager::RunningScene = nullptr;
unique_ptr<Scene> SceneManager::ReadyScene = nullptr;
map<SCENE_NAME, unique_ptr<Scene>> SceneManager::BlockedScenes;

DWORD CALLBACK loadingThread(LPVOID pvParam)
{
	// 쓰레드 종료 시점 (방법)
	/*
		1. 호출된 함수가 반환될 때 (우리가 사용할 방법)
		2. 호출된 함수 내부에서 exitthread()를 호출했을 때
		3. 동일한 프로세스나 다른 프로세스에서 terminatethread()를 호출했을 때
		4. 현재 쓰레드가 포함된 프로세스가 종료될 때
	*/

	switch (TimeHandler::getInstance().startAlarm((*dynamic_cast<LoadingScene*>(SceneManager::RunningScene.get())).getMinLoadingTime()))
	{
	case true:
		SceneManager::changeSceneByReadyScene();
		break;
	case false:
		assert(-1);
		break;
	}
	
	return 0;
}

void SceneManager::update()
{
	RunningScene->update(TimeHandler::getInstance().getDeltaTime());
}

void SceneManager::render(const HDC& hdc)
{
	RunningScene->render(hdc);
}

void SceneManager::release(unique_ptr<Scene>& scene, bool isStopPlayingSound, bool isDeletingPtrData, bool isDeletingPtr)
{
	if (scene)
	{
		if (isStopPlayingSound)
			scene->stopCurrentSound();
		if (isDeletingPtrData)
			scene->release();		
		if (isDeletingPtr)
			scene.reset();
	}
}

void SceneManager::allRelease()
{
	release(RunningScene, true, true, true);
	release(ReadyScene, true, true, true);
	allDeleteBlockedScenes();
}

void SceneManager::requestOptionInit()
{
	ReadyScene = move(createSceneBy(SCENE_NAME::OPTION));
	ReadyScene->init();
	addScene(ReadyScene->getSceneID(), ReadyScene); // 옵션 씬은 블락드 씬으로 들어간다.
}

void SceneManager::toTitleScene(const SCENE_NAME& newSceneName)
{	
	(*ReadyScene).release();
	changeScene(newSceneName);
}

void SceneManager::changeSceneByReadyScene(const SCENE_NAME& newSceneName)
{
	if (checkScene(ReadyScene))
	{
		if (compareSceneNameBy(ReadyScene->getSceneID(), SCENE_NAME::OPTION))
		{
			swapReadySceneAndOptionScene();
			return;
		}

		// 게임, 맵툴 씬에서 옵션 씬으로 들어가 타이틀 씬으로 이동할 경우 넘어가는 코드.
		if (ReadyScene.get()->getSceneID() == SCENE_NAME::TITLE && RunningScene.get()->getSceneID() == SCENE_NAME::OPTION)
		{
			ReadyScene.get()->reset();
		}

		moveSceneBy(ReadyScene, RunningScene); 
		if (compareSceneNameBy(RunningScene->getSceneID(), SCENE_NAME::LOADING)) //이미 체인지 씬이 한 번 이상 호출한 상태에서 체인지씬(기본, 로딩)이 다시 콜된 상황
		{
			ReadyScene = move(findScene(newSceneName));
			switch (ReadyScene != nullptr)
			{
			case true:
				deleteScene(newSceneName);
				break;
			case false:
				ReadyScene = move(createSceneBy(newSceneName));
				break;
			}
		}

		RunningScene->startScene();
		return;
	}
}

/*
* 체인지 씬은 바꿀 씬과 그 준비 과정을 로딩할 씬 인자를 받아 알고리즘을 통해 씬 변경이 됩니다.
* 알고리즘은 크게 4가지 정도의 경우의 수로 구현되어 있습니다.
*
* 이 알고리즘은 시스템 프로그래밍 스케줄링 동작 원리를 그대로 따라해서 만들었습니다.
*
* 멤버 변수 RunningScene, ReadyScene, BlockedScene
* RunningScene -> 현재 실행 중인 프로세스
* ReadyScene -> 다음 프로세스를 미리 예약
* BlockedScene -> 사용하지 않을 씬을 보관하고 다시 필요 시 꺼낸다. 속도 증가 목적
*
* 알고리즘 과정
* 체인지씬 뉴 씬, 로딩 신
1. 런닝씬 null
런닝 create(로딩)
레디씬 create (뉴씬)
런닝씬 init -> start
쓰레드 가동
레디 씬 init
스왑 레디 <> 런닝
스왑된 레디 릴리즈 및 -> 블락드 씬
레디 -> empty

2 런닝씬이 != nullptr (이미 한 번 실행된 이후)
레디 씬 = findScene (로딩)
레디 씬 init
스왑 레디 <> 런닝
스왑된 레디 릴리즈 및 -> 블락드 씬(기존 씬)
if (런닝(뉴 로딩) == 로딩) 이라면 true 해당
레디 = find 뉴 씬
런닝씬 스타트(뉴로딩)
쓰레드 가동
레디 씬 init(뉴씬)
스왑 레디<>런닝
스왑된 레디 릴리즈 및 블락드 씬(뉴 로딩)
런닝씬(뉴씬) 스타트

체인지씬 뉴 씬
1. 런닝씬 null
런닝씬 = create 뉴씬
런닝 씬 init
런닝 씬 스타트

2 레디 씬 = findScene(뉴씬)
레디 씬 init
스왑 레디, 런닝
기존 씬 -> 블락드씬
런닝씬 스타트 뉴씬
*/

// 바로 새로운 씬을 부르겠다.
void SceneManager::changeScene(const SCENE_NAME& newSceneName) 
{
	switch (RunningScene == nullptr)
	{
	case true:
		RunningScene = move(createSceneBy(newSceneName));
		if (checkScene(RunningScene))
		{
			RunningScene->startScene();
		}
		break;
	case false:
		ReadyScene = move(findScene(newSceneName));
		switch (ReadyScene != nullptr)
		{
		case true:
			deleteScene(newSceneName);
			break;
		case false:
			ReadyScene = move(createSceneBy(newSceneName));
			break;
		}

		changeSceneByReadyScene(newSceneName);
		break;
	}
}

void SceneManager::changeScene(const SCENE_NAME& newSceneName, const SCENE_NAME& newLoadingSceneName)
{
	switch (RunningScene == nullptr)
	{
	case true:
		RunningScene = move(createSceneBy(newLoadingSceneName));
		ReadyScene = move(createSceneBy(newSceneName));
		if (checkScene(RunningScene))
		{
			RunningScene->startScene();
		}
		break;
	case false:
		ReadyScene = move(findScene(newLoadingSceneName));
		switch (ReadyScene != nullptr)
		{
		case true:
			deleteScene(newLoadingSceneName);
			break;
		case false:
			ReadyScene = move(createSceneBy(newLoadingSceneName));
			break;
		}

		changeSceneByReadyScene(newSceneName);
		break;
	}

	HANDLE		hThread;
	DWORD		loadThreadID;
	CloseHandle(CreateThread(NULL, 0, loadingThread, NULL, 0, &loadThreadID));
}

// 1.런닝씬이 로딩씬이 아니고 레디씬이 옵션씬이라면 초기화 및 블락씬으로 진행되지 않는다.
// 2.런닝씬이 옵션씬이고 레디씬이 Scene이라면 init을 하지 않는다.
// 3.런닝씬과 레디씬이 스왑을 그냥 진행한다.
void SceneManager::swapReadySceneAndOptionScene()
{
	switch (compareSceneNameBy(ReadyScene->getSceneID(), SCENE_NAME::OPTION))
	{
	case true:
	{
		auto& optionScene = *dynamic_cast<OptionScene*>(ReadyScene.get());
		// 런닝 씬이 게임, 맵툴 씬일 경우 옵션에 타이틀 버튼 형성
		switch (RunningScene->getSceneID())
		{
		case SCENE_NAME::GAME:
			optionScene.requestToMakeTitleButton();
			optionScene.notifyToScenePartner("Game");
			break;
		case SCENE_NAME::MAPTOOL:
			optionScene.requestToMakeTitleButton();
			optionScene.notifyToScenePartner("MapTool");
			break;
		case SCENE_NAME::TITLE:
			optionScene.requestToDeleteTitleButton();
			break;
		}

		controlRunningSceneMusic(false);
		swapSceneBy(RunningScene, ReadyScene);
	}
		break;
	case false:
		switch (compareSceneNameBy(RunningScene->getSceneID(), SCENE_NAME::OPTION))
		{
		case true:
		{
			// 옵션 씬에서 변경한 적이 있다면 씬 리셋을 한다.
			auto& optionScene = *dynamic_cast<OptionScene*>(RunningScene.get());
			if (optionScene.isDataRecorded())
			{
				optionScene.setDataChangeRecode(false);
				ReadyScene->reset();
			}

			/*unique_ptr<Scene> loadingScene = move(findScene(SCENE_NAME::LOADING));
			if (loadingScene != nullptr)
			{
				deleteScene(SCENE_NAME::LOADING);
				loadingScene->reset();
				addScene(SCENE_NAME::LOADING, loadingScene);
			}*/
		}
		
			swapSceneBy(RunningScene, ReadyScene);
			controlRunningSceneMusic(true);

			addScene(ReadyScene->getSceneID(), ReadyScene); // 옵션씬은 블락드씬으로 들어간다.
			break;
		case false:
			assert(false);
			break;
		}
		break;
	}
}

bool SceneManager::checkScene(const unique_ptr<Scene>& scene)
{
	if (scene->getSceneID() != SCENE_NAME::OPTION && scene->getSceneID() != SCENE_NAME::LOADING)
	{		
		int16 gammaCount = 0;

		OptionScene* optScenePtr = nullptr;
		unique_ptr<Scene> optionScenePtr = findScene(SCENE_NAME::OPTION);
		switch (optionScenePtr == nullptr)
		{
		case true:
			if ((*RunningScene).getSceneID() != SCENE_NAME::OPTION)
				assert(false);
	
			optScenePtr = dynamic_cast<OptionScene*>(&(*RunningScene));
			gammaCount = optScenePtr->getGammaCount();
			optScenePtr->setScenePtrFromOption(&(*scene));
			break;
		case false:
			optScenePtr = dynamic_cast<OptionScene*>(&(*optionScenePtr));
			gammaCount = optScenePtr->getGammaCount();
			optScenePtr->setScenePtrFromOption(&(*scene));
			deleteScene(SCENE_NAME::OPTION);
			addScene(SCENE_NAME::OPTION, optionScenePtr);
			break;
		}

		switch (scene->getSceneID())
		{
		case SCENE_NAME::TITLE:
			switch (scene->isModifiable())
			{
			case true:
				dynamic_cast<TitleScene*>(&(*scene))->setGamma(gammaCount);
				break;
			case false:
				dynamic_cast<TitleScene*>(&(*scene))->setGammaCount(gammaCount);
				break;
			}
			break;
		case SCENE_NAME::GAME:
			switch (scene->isModifiable())
			{
			case true:
				dynamic_cast<GameScene*>(&(*scene))->setGamma(gammaCount);
				break;
			case false:
				dynamic_cast<GameScene*>(&(*scene))->setGammaCount(gammaCount);
				break;
			}
			break;
		case SCENE_NAME::MAPTOOL:
			switch (scene->isModifiable())
			{
			case true:
				dynamic_cast<MapToolScene*>(&(*scene))->setGamma(gammaCount);
				break;
			case false:
				dynamic_cast<MapToolScene*>(&(*scene))->setGammaCount(gammaCount);
				break;
			}
			break;
		}	
	}

	if (compareSceneNameBy(scene->getSceneID(), SCENE_NAME::TITLE))
	{
		TitleScene& titleScene = *dynamic_cast<TitleScene*>(scene.get());
		// 로딩씬이 포함된 함수가 호출될 경우
		if (compareSceneNameBy(RunningScene->getSceneID(), SCENE_NAME::LOADING) && compareSceneNameBy(ReadyScene->getSceneID(), SCENE_NAME::TITLE))
		{
			titleScene.setPlaySpeedOfOpeningVideo(titleScene.getToLoadingSpeed());
		}
		// 로딩씬이 포함되지 않은 함수
		else if (compareSceneNameBy(RunningScene->getSceneID(), SCENE_NAME::TITLE) || compareSceneNameBy(ReadyScene->getSceneID(), SCENE_NAME::TITLE))
		{	
			titleScene.setPlaySpeedOfOpeningVideo(titleScene.getToBaseSpeed());
		}		
	}

	if (scene->init() == false)
		assert(false);

	return true;
}

void SceneManager::swapSceneBy(unique_ptr<Scene>& sceneA, unique_ptr<Scene>& sceneB)
{
	unique_ptr<Scene> tempSceneA = move(sceneA);
	sceneA = move(sceneB);
	sceneB = move(tempSceneA);
}

void SceneManager::moveSceneBy(unique_ptr<Scene>& fromScene, unique_ptr<Scene>& toScene)
{
	swapSceneBy(fromScene, toScene);
	auto& swappedScene = fromScene;
	if (swappedScene)
	{
		release(swappedScene, true, false, false);
		addScene(swappedScene->getSceneID(), swappedScene);
	}
}

unique_ptr<Scene> SceneManager::createSceneBy(const SCENE_NAME& sceneName)
{
	unique_ptr<Scene> scene;
	switch (sceneName)
	{
	case SCENE_NAME::GAME:
		scene = make_unique<GameScene>();
		break;
	case SCENE_NAME::MAPTOOL:
		scene = make_unique<MapToolScene>();
		break;
	case SCENE_NAME::OPTION:
		scene = make_unique<OptionScene>();
		break;
	case SCENE_NAME::TITLE:
		scene = make_unique<TitleScene>();
		break;
	case SCENE_NAME::LOADING:
		scene = make_unique<LoadingScene>();
		break;
	default:
		assert(false);
		break;
	}

	return move(scene);
}

unique_ptr<Scene> SceneManager::findScene(SCENE_NAME sceneName)
{
	const auto& iter = BlockedScenes.find(move(sceneName));
	if (iter == BlockedScenes.end())
	{
		return nullptr;
	}

	return move(iter->second);
}

void SceneManager::addScene(SCENE_NAME sceneName, unique_ptr<Scene>& scene)
{
	assert(scene);

	BlockedScenes.insert(make_pair(move(sceneName), move(scene)));
}

void SceneManager::deleteScene(SCENE_NAME sceneName)
{
	const auto& iter = BlockedScenes.find(move(sceneName));
	if (iter == BlockedScenes.end())
	{
		assert(false);
		return;
	}

	BlockedScenes.erase(iter);
}

void SceneManager::allDeleteBlockedScenes()
{
	for (auto& scene : BlockedScenes)
	{
		if (scene.second)
			release(scene.second, true, true, true);
	}

	BlockedScenes.clear();
}


