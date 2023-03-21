#include "Scene.h"
#include "ImageManager.h"
#include "ButtonState.h"
#include "FunctionsStorage.h"
#include "MouseHandler.h"
#include "SceneManager.h"
#include "ScreenManager.h"

bool Scene::init()
{
	const auto& screenMgr = ScreenManager::getInstance();
	_scale = screenMgr.getCalculatedScale();
	if (_scale.x > 1.0f)
		_scale.x = 1.0f;

	_startPosition = move(screenMgr.generateStartPositionOfScreen());
	if (_startPosition.x < 0 || _startPosition.y < 0)
		_startPosition = { 0, 0 };

	_isModifiable = true;

	return true;
}

void Scene::update(float deltaTime)
{
	assert(0 < deltaTime);
}

void Scene::render(const HDC& hdc) const
{}

bool GameScene::init()
{
	if (!_isModifiable)
	{
		::Scene::init();
		setSoundFile( { "Sounds/Music/Game.mp3", SOUND_TYPE::MUSIC, SoundController::musicListToInt(MUSIC_LIST::GAME) });
		_world.init(_scale, _startPosition);
		_world.setCanvasGamma(255);
		_world.setCurrentAreaGamma(_gammaCount);
	}

	return true;
}

void GameScene::update(float deltaTime)
{
	moveToPreviousScene();
	_world.update();
}

void GameScene::render(const HDC& hdc) const
{
	_world.render(hdc);
}

void GameScene::release()
{
	::Scene::release();
	_world.release();
}

void GameScene::setGamma(const int16 gammaCount)
{
	setGammaCount(gammaCount);
	_world.setCurrentAreaGamma(gammaCount);
}

void GameScene::moveToPreviousScene() const
{
	if (KeyController::isOnceKeyUp(VK_ESCAPE)) 
		SceneManager::changeScene(SCENE_NAME::OPTION);
}

bool MapToolScene::init()
{
	if (!_isModifiable)
	{
		::Scene::init();
		setSoundFile( { "Sounds/Music/Maptool.mp3", SOUND_TYPE::MUSIC, SoundController::musicListToInt(MUSIC_LIST::MAPTOOL)} );
		_mapTool.init(_scale, _startPosition);		
		_mapTool.setCurrentAreaGamma(22);
	}

	return true;
}

void MapToolScene::update(float deltaTime)
{
	moveToPreviousScene();

	if (KeyController::isOnceKeyUp(VK_F1))
	{
		_mapTool.isGammaMode() == true ? _mapTool.setGammaMode(false) : _mapTool.setGammaMode(true);
	}

	_mapTool.update();
}

void MapToolScene::render(const HDC& hdc) const
{
	_mapTool.render(hdc);
}

void MapToolScene::release()
{
	::Scene::release();
	_mapTool.release();
}

void MapToolScene::reset()
{
	::Scene::init();
	_mapTool.reset(_startPosition, _scale);
}

void MapToolScene::setGamma(const int16 gammaCount)
{
	setGammaCount(gammaCount);
	_mapTool.setCanvasGamma(255);
	_mapTool.setCurrentAreaGamma(_gammaCount);
}

void MapToolScene::moveToPreviousScene() const
{
	if (KeyController::isOnceKeyUp(VK_ESCAPE))
	{
		SceneManager::changeScene(SCENE_NAME::OPTION);
	}
}

bool OptionScene::init()
{	
	if (!_isModifiable)
	{
		::Scene::init();
		_option.init(_startPosition, _scale);
	}

	return true;
}

void OptionScene::update(float deltaTime)
{
	_option.update();
	moveToPreviousScene();
}

void OptionScene::render(const HDC& hdc) const
{
	_option.render(hdc);
}

void OptionScene::release()
{
	::Scene::release();
	_option.release();
}

void OptionScene::reset()
{	
	::Scene::release();
}

void OptionScene::moveToPreviousScene() const
{
	if (KeyController::isOnceKeyUp(VK_ESCAPE))
	{
		if (_option.getCurrentUseMenuType() == typeid(BaseMenu).name())
			SceneManager::swapReadySceneAndOptionScene();
	}
}

bool TitleScene::init()
{
	if (!_isModifiable)
	{
		::Scene::init();

		initVideos();
		initButtons();
		initImage();
	}

	return true;
}

void TitleScene::update(float deltaTime)
{
	skipOpeningVideo();

	::Scene::update(deltaTime);
	_videoCtr.update(deltaTime);

	if (isTitleFrameSection())
	{
		_playButton.update();
		_maptoolButton.update();
		_optionButton.update();
		_exitButton.update();
	}
}

void TitleScene::render(const HDC& hdc) const
{
	_videoCtr.render(hdc);

	if (isTitleFrameSection())
	{
		_titleName.render(hdc, { 0, 0 });

		_playButton.render(hdc, false, false, true);
		_maptoolButton.render(hdc, false, false, true);
		_optionButton.render(hdc, false, false, true);
		_exitButton.render(hdc, false, false, true);
	}

	_gamma.renderTransparent(hdc, {});
}

void TitleScene::release()
{
	::Scene::release();

	_videoCtr.release();

	_playButton.release();
	_maptoolButton.release();
	_optionButton.release();
	_exitButton.release();
}

void TitleScene::reset()
{
	::Scene::init();

	resetPlayingVideoImage();
	resetImage();
	resetButtons();
}

void TitleScene::startScene()
{
	static bool isFirstCall = false;
	switch (isFirstCall)
	{
	case false:
		isFirstCall = true;
		_videoCtr.playVideoBy("Images/Scene/Title/Opening.bmp");
		break;
	case true:
		_videoCtr.playVideoBy("Images/Scene/Title/BackgroundVideo.bmp");
		break;
	}
}

void TitleScene::setGamma(const int16 gammaCount)
{
	setGammaCount(gammaCount);
	_gamma.setTransparency(255 - TitleGammaValue * _gammaCount);
}

void TitleScene::initImage()
{
	_gamma = ImageManager::makeImage("Images/Scene/Maptool/Black.bmp", {}, { 1.0f, 1.0f }, move(ScreenManager::getInstance().getSubResolution()), true, { false, false }, MagentaRGB);
	_gamma.setTransparency(255 - TitleGammaValue * _gammaCount);

	_titleName = ImageManager::makeImage("Images/Scene/Title/TitleName.bmp", { TextAlignment::alignMiddleCenterX(900), TextAlignment::alignStartY(_startPosition.y, 300, _scale.y) }, _scale, { 900, 380 }, true, { false, false }, BlackRGB);

	assert(true);
}

void TitleScene::initButtons()
{
	const uint16 buttonPosX{ TextAlignment::alignStartX(_startPosition.x, 35, _scale.x) };

	_playButton.initImage({ buttonPosX, TextAlignment::alignStartY(855) }, _scale, { 80, 40 });
	_playButton.initText("µ¸¿ò", "Play", { buttonPosX, TextAlignment::alignStartY(850) }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_playButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_playButton.createAndInitToButtonStateBy(make_shared<Active>(), { "Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
	_playButton.createAndInitToButtonStateBy(make_shared<PressUp>(), { "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::BUTTON_CLICK) }, &FunctionsStorage::sceneChange, { {new SCENE_NAME(SCENE_NAME::GAME), true}, {new SCENE_NAME(SCENE_NAME::LOADING), true} });
	//_playButton.createAndInitToButtonStateBy(make_shared<PressUp>(::PressUp), {"Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::musicListToInt(MUSIC_LIST::OPENING)}, &FunctionsStorage::sceneChange, { new SceneName(SceneName::Title) });

	_maptoolButton.initImage({ buttonPosX, TextAlignment::alignStartY(910) }, _scale, { 160, 40 });
	_maptoolButton.initText("µ¸¿ò", "Maptool", { buttonPosX, TextAlignment::alignStartY(905) }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_maptoolButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_maptoolButton.createAndInitToButtonStateBy(make_shared<Active>(), { "Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
	_maptoolButton.createAndInitToButtonStateBy(make_shared<PressUp>(), { "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::BUTTON_CLICK) }, &FunctionsStorage::sceneChange, { {new SCENE_NAME(SCENE_NAME::MAPTOOL), true}, {new SCENE_NAME(SCENE_NAME::LOADING), true} });

	_optionButton.initImage({ buttonPosX, TextAlignment::alignStartY(965) }, _scale, { 130, 40 });
	_optionButton.initText("µ¸¿ò", "Option", { buttonPosX, TextAlignment::alignStartY(960) }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_optionButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_optionButton.createAndInitToButtonStateBy(make_shared<Active>(), { "Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
	_optionButton.createAndInitToButtonStateBy(make_shared<PressUp>(), { "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::BUTTON_CLICK) }, &FunctionsStorage::sceneChange, { {new SCENE_NAME(SCENE_NAME::OPTION), true} });

	_exitButton.initImage({ buttonPosX, TextAlignment::alignStartY(1020) }, _scale, { 80, 40 });
	_exitButton.initText("µ¸¿ò", "Exit", { buttonPosX, TextAlignment::alignStartY(1015) }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_exitButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_exitButton.createAndInitToButtonStateBy(make_shared<Active>(), {"Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
	_exitButton.createAndInitToButtonStateBy(make_shared<PressUp>(), { "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::BUTTON_CLICK) }, &FunctionsStorage::quit);

	assert(true);
}

void TitleScene::initVideos()
{
	const POINT& matrixScreen = move(POINT{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) });

	_videoCtr.init(2);
	_videoCtr.addVideo("Images/Scene/Title/Opening.bmp", { "Sounds/Music/Opening.mp3", SOUND_TYPE::MUSIC, SoundController::musicListToInt(MUSIC_LIST::OPENING) }, _startPosition, { 1.0f, 1.0f },
		{ matrixScreen.x * 9, matrixScreen.y * 14 }, { 9, 14 }, _openingPlaySpeed, 0, 0, false, false, true);
	_videoCtr.addVideo("Images/Scene/Title/BackgroundVideo.bmp", { "Sounds/Music/Title.mp3", SOUND_TYPE::MUSIC, SoundController::musicListToInt(MUSIC_LIST::TITLE) }, _startPosition, { 1.0f, 1.0f },
		{ matrixScreen.x * 7, matrixScreen.y * 11 }, { 7, 11 }, 9.7, 76, 39, false, true, false);

	assert(true);
}

void TitleScene::resetImage()
{
	ImageManager::resetImage(_gamma, {}, move(ScreenManager::getInstance().getSubResolution()));
	ImageManager::resetImage(_titleName, { TextAlignment::alignMiddleCenterX(900), TextAlignment::alignStartY(_startPosition.y, 300, _scale.y) }, move(POINT{ static_cast<uint16>(_scale.x * 900), static_cast<uint16>(_scale.y * 380) }));
}

void TitleScene::resetButtons()
{
	const uint16 buttonPosX{ TextAlignment::alignStartX(_startPosition.x, 35, _scale.x) };

	_playButton.resetImage({ buttonPosX, TextAlignment::alignStartY(855) }, _scale);
	_playButton.resetText(TextManager::generateFontSizeByResolution(), { buttonPosX, TextAlignment::alignStartY(850) });

	_maptoolButton.resetImage({ buttonPosX, TextAlignment::alignStartY(910) }, _scale);
	_maptoolButton.resetText(TextManager::generateFontSizeByResolution(), { buttonPosX, TextAlignment::alignStartY(905) });
	
	_optionButton.resetImage({ buttonPosX, TextAlignment::alignStartY(965) }, _scale);
	_optionButton.resetText(TextManager::generateFontSizeByResolution(), { buttonPosX, TextAlignment::alignStartY(960) });
	
	_exitButton.resetImage({ buttonPosX, TextAlignment::alignStartY(1020) }, _scale);
	_exitButton.resetText(TextManager::generateFontSizeByResolution(), { buttonPosX, TextAlignment::alignStartY(1015) });
		
	assert(true);
}

void TitleScene::skipOpeningVideo()
{
	if (_videoCtr.compareCurrentVideoWith("Images/Scene/Title/Opening.bmp"))
	{
		if (MouseHandler::isLeftClick())
		{
			_videoCtr.playVideoBy("Images/Scene/Title/BackgroundVideo.bmp", true);
		}

		if (KeyController::isOnceKeyUp(VK_Ykey))
		{
			SceneManager::changeScene(SCENE_NAME::GAME);
		}
	}
}

bool LoadingScene::init()
{
	if (!_isModifiable)
	{
		::Scene::init();
		setMinLoadingTime(0);
		initVideo();
	}

	return true;
}

void LoadingScene::initVideo()
{	
	_videoCtr.init(1);
	const POINT& matrixScreen = move(POINT{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) });
	_videoCtr.addVideo("Images/Scene/Loading/Loading.bmp", { "Sounds/Music/Loading.mp3", SOUND_TYPE::MUSIC, SoundController::musicListToInt(MUSIC_LIST::LOADING) }, _startPosition, { 1.0f, 1.0f },
		{ matrixScreen.x * 6, matrixScreen.y * 10 }, { 6, 10 }, 12, 0, 0, false, true, false);
	
	assert(true);
}

void LoadingScene::reset()
{
	::Scene::init();

	resetVideo();
}

void LoadingScene::resetVideo()
{
	_videoCtr.resetPlayingVideo(_startPosition, move(ScreenManager::getInstance().getSubResolution()), { 6, 10 });
}

void LoadingScene::update(float deltaTime)
{
	::Scene::update(deltaTime);
	_videoCtr.update(deltaTime);
}

void LoadingScene::render(const HDC& hdc) const
{
	const POINT& subResolution = ScreenManager::getInstance().getSubResolution();
	::PatBlt(hdc, 0, 0, subResolution.x, subResolution.y, BLACKNESS);
	_videoCtr.render(hdc);
}

void LoadingScene::release()
{
	::Scene::release();

	_videoCtr.release();
}

