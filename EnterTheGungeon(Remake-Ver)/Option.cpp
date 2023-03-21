#include "Option.h"
#include "ImageManager.h"
#include "ButtonState.h"
#include "ScreenManager.h"
#include "SceneManager.h"
#include "MapTool.h"

void ArrowButtonsController::init(const char* borderName, const char* contents, uint16 startPointY, const FPOINT& scale)
{
	initBorder(move(borderName), move(contents), startPointY, scale);
	initArrowButton(scale);
}

void ArrowButtonsController::update()
{
	_borderButton.update();

	_leftArrowButton.update();	
	_rightArrowButton.update();
}

void ArrowButtonsController::render(const HDC& hdc) const
{
	_borderButton.render(hdc, false, false, true);

	_leftArrowButton.renderFrame(hdc, false, true, false);
	_rightArrowButton.renderFrame(hdc, false, true, false);

	_contents.render(hdc);
}

void ArrowButtonsController::release()
{
	_borderButton.release();

	_leftArrowButton.release();
	_rightArrowButton.release();	
}

void ArrowButtonsController::initContents()
{
	_backUpCurrentIdx = _currentIdx;

	ScreenManager& screenMgr = ScreenManager::getInstance();
	if (_borderButton.getButtonTitleName() == "Resolution")
	{
		_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toResolution(_currentIdx)));
		screenMgr.changeResolution(ScreenContents::toResolution(_currentIdx));
	}
	else if (_borderButton.getButtonTitleName() == "Scaling Mode")
	{
		_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toScalingMode(_currentIdx)));
		screenMgr.changeScalingMode(ScreenContents::toScalingMode(_currentIdx));
	}
	else if (_borderButton.getButtonTitleName() == "Screen Mode")
	{
		_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toScreenMode(_currentIdx)));
		screenMgr.changeScreenMode(move(ScreenContents::toScreenMode(_currentIdx)));

		(*_optionPtr.get())->resetOptionAndOptionMenu(screenMgr.generateStartPositionOfScreen(), screenMgr.getCalculatedScale());
	}
}

void ArrowButtonsController::initContents(ResolutionList resolution, ScalingModeList scalingMode, ScreenModeList screenMode)
{
	ScreenManager& screenMgr = ScreenManager::getInstance();
	if (_borderButton.getButtonTitleName() == "Resolution" && resolution != ResolutionList::END)
	{
		_currentIdx = ScreenContents::toInt(move(resolution));

		_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toResolution(_currentIdx)));
		screenMgr.changeResolution(ScreenContents::toResolution(_currentIdx));
	}
	else if (_borderButton.getButtonTitleName() == "Scaling Mode" && scalingMode != ScalingModeList::END)
	{
		_currentIdx = ScreenContents::toInt(move(scalingMode));

		_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toScalingMode(_currentIdx)));
		screenMgr.changeScalingMode(ScreenContents::toScalingMode(_currentIdx));
	}
	else if (_borderButton.getButtonTitleName() == "Screen Mode" && screenMode != ScreenModeList::END)
	{
		_currentIdx = ScreenContents::toInt(move(screenMode));

		_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toScreenMode(_currentIdx)));
		screenMgr.changeScreenMode(move(ScreenContents::toScreenMode(_currentIdx)));

		(*_optionPtr.get())->resetOptionAndOptionMenu(screenMgr.generateStartPositionOfScreen(), screenMgr.getCalculatedScale());
	}
}

void ArrowButtonsController::resetImageOfButtons(const char* borderName, const char* contents, uint16 startPointY, const FPOINT& scale)
{
	_borderButton.resetImage({ TextAlignment::alignMiddleCenterX(550), startPointY }, scale);
	_borderButton.resetText(TextManager::generateFontSizeByResolution(), { TextAlignment::alignMiddleCenterX(move(borderName)), startPointY });
	_contents.setFontSize(TextManager::generateFontSizeByResolution());
	_contents.setStartPosition({ TextAlignment::alignMiddleCenterX(move(contents)), (static_cast<LONG>((_borderButton.getImageSP().y + (60 * scale.y)))) });
	
	const uint16 arrowSizeX{ 18 };
	const LONG spX{ static_cast<LONG>(scale.x * arrowSizeX) };
	const LONG spY{ static_cast<LONG>(scale.y * 10) };
	_leftArrowButton.resetImage({ _borderButton.getImageSP().x - spX, _borderButton.getImageSP().y + (_borderButton.getImagePrintSize().y / 3) + spY }, scale);
	_rightArrowButton.resetImage({ _borderButton.getImageSP().x + _borderButton.getImagePrintSize().x, _borderButton.getImageSP().y + (_borderButton.getImagePrintSize().y / 3) + spY }, scale);
}

void ArrowButtonsController::resetSetting()
{
	ScreenManager& screenMgr = ScreenManager::getInstance();
	if (checkModifiedTrace() == false)
	{
		if (_borderButton.getButtonTitleName() == "Resolution")
		{
			_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toResolution(_backUpCurrentIdx)));
			screenMgr.changeResolution(ScreenContents::toResolution(_backUpCurrentIdx));
		}
		else if (_borderButton.getButtonTitleName() == "Scaling Mode")
		{
			_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toScalingMode(_backUpCurrentIdx)));
			screenMgr.changeScalingMode(ScreenContents::toScalingMode(_backUpCurrentIdx));
		}
		else if (_borderButton.getButtonTitleName() == "Screen Mode")
		{
			auto a = ScreenContents::toScreenMode(_backUpCurrentIdx);
			_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toScreenMode(_backUpCurrentIdx)));
			screenMgr.changeScreenMode(move(ScreenContents::toScreenMode(_backUpCurrentIdx)));
		}

		(*_optionPtr.get())->resetImageScale(screenMgr.generateStartPositionOfScreen(), screenMgr.getCalculatedScale());
		_currentIdx = _backUpCurrentIdx;
	}
}

void ArrowButtonsController::moveToNextContent(int16 movedValue)
{
	ScreenManager& screenMgr = ScreenManager::getInstance();
	int16 virtualCurrentIdx = _currentIdx + movedValue;
	if (_borderButton.getButtonTitleName() == "Resolution")
	{
		if (virtualCurrentIdx < 0)
			virtualCurrentIdx = ScreenContents::toInt(ResolutionList::W2560H1080);
		else if (virtualCurrentIdx >= ScreenContents::toInt(ResolutionList::END))
			virtualCurrentIdx = ScreenContents::toInt(ResolutionList::W640H480);

		_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toResolution(virtualCurrentIdx)));
		screenMgr.changeResolution(ScreenContents::toResolution(virtualCurrentIdx));
	}
	else if (_borderButton.getButtonTitleName() == "Scaling Mode")
	{
		if (virtualCurrentIdx < 0)
			virtualCurrentIdx = ScreenContents::toInt(ScalingModeList::FASTSCALING);
		else if (virtualCurrentIdx >= ScreenContents::toInt(ScalingModeList::END))
			virtualCurrentIdx = ScreenContents::toInt(ScalingModeList::FORCEPIXELPERFECT);

		_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toScalingMode(virtualCurrentIdx)));
		screenMgr.changeScalingMode(ScreenContents::toScalingMode(virtualCurrentIdx));
	}
	else if (_borderButton.getButtonTitleName() == "Screen Mode")
	{
		if (virtualCurrentIdx < 0)
			virtualCurrentIdx = ScreenContents::toInt(ScreenModeList::WINDOWED);
		else if (virtualCurrentIdx >= ScreenContents::toInt(ScreenModeList::END))
			virtualCurrentIdx = ScreenContents::toInt(ScreenModeList::FULLSCREEN);

		_contents.modifyTextName(ScreenContents::toConstCharPtr(ScreenContents::toScreenMode(virtualCurrentIdx)));
		screenMgr.changeScreenMode(move(ScreenContents::toScreenMode(virtualCurrentIdx)));
	}

	(*_optionPtr.get())->resetImageScale(screenMgr.generateStartPositionOfScreen(), screenMgr.getCalculatedScale());
	_currentIdx = virtualCurrentIdx;
}

void ArrowButtonsController::initBorder(const char* borderName, const char* contents, uint16 startPointY, const FPOINT& scale)
{
	_borderButton.initImage({ TextAlignment::alignMiddleCenterX(550), startPointY }, scale, { 550, 120 });
	_borderButton.initText("돋움", borderName, { TextAlignment::alignMiddleCenterX(move(borderName)), startPointY }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_borderButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_borderButton.createAndInitToButtonStateBy(make_shared<Active>(), { "Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);

	_contents.init("돋움", contents, { TextAlignment::alignMiddleCenterX(move(contents)), _borderButton.getImageSP().y + 60 }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
}

void ArrowButtonsController::initArrowButton(const FPOINT & scale)
{
	_backUpCurrentIdx = _currentIdx = 0;	
	const POINT arrowSize{ 18, 68 };
	_leftArrowButton.initImage("Images/Scene/Option/Arrow.bmp", { _borderButton.getImageSP().x - arrowSize.x, _borderButton.getImageSP().y + (_borderButton.getImagePrintSize().y / 3) + 10 }, scale, arrowSize, { 1, 2 }, true, {true, false}, MagentaRGB);
	_leftArrowButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, &FunctionsStorage::changeButtonShape, { {&_leftArrowButton, false}, {new int16(0), true} });
	_leftArrowButton.createAndInitToButtonStateBy(make_shared<Active>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::changeButtonShape, { {&_leftArrowButton, false}, {new int16(1), true} });	
	_leftArrowButton.createAndInitToButtonStateBy(make_shared<PressUp>(true), { "", SOUND_TYPE::UI }, &FunctionsStorage::moveToNextContent, { {this, false}, {new int16(-1), true}});

	_rightArrowButton.initImage("Images/Scene/Option/Arrow.bmp", { _borderButton.getImageSP().x + _borderButton.getImagePrintSize().x + 0, _borderButton.getImageSP().y + (_borderButton.getImagePrintSize().y / 3) + 10 }, scale, arrowSize, { 1, 2 }, true, { false, false }, MagentaRGB);
	_rightArrowButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, &FunctionsStorage::changeButtonShape, { {&_rightArrowButton, false}, {new int16(0), true} });
	_rightArrowButton.createAndInitToButtonStateBy(make_shared<Active>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::changeButtonShape, { {&_rightArrowButton, false}, {new int16(1), true} });
	_rightArrowButton.createAndInitToButtonStateBy(make_shared<PressUp>(true), { "", SOUND_TYPE::UI }, &FunctionsStorage::moveToNextContent, { {this, false}, {new int16(1), true}});
}

#include "MapTool.h"

void BulletButtonsController::init(const char* borderName, uint16 borderStartY, const FPOINT& scale)
{
	initBorder(move(borderName), borderStartY, scale);
	initBulletButtons(scale);
}

void BulletButtonsController::update()
{
	_borderButton.update();

	for (auto& bulletBtn : _bulletButtons)
		bulletBtn.update();
}

void BulletButtonsController::render(const HDC& hdc) const
{
	_borderButton.render(hdc, false, true, true);

	for (const auto& bulletBtn : _bulletButtons)
		bulletBtn.renderFrame(hdc, false, true, false);
}

void BulletButtonsController::release()
{
	_borderButton.release();

	for (auto& bullet : _bulletButtons)
	{
		bullet.release();
	}
}

void BulletButtonsController::resetImageOfButtons(const char* borderName, uint16 borderStartY, const FPOINT& scale)
{
	_borderButton.resetImage({ TextAlignment::alignMiddleCenterX(578), borderStartY }, scale);
	_borderButton.resetText(TextManager::generateFontSizeByResolution(), { TextAlignment::alignMiddleCenterX(move(borderName)), borderStartY - static_cast<uint16>((55 * scale.y)) });
	
	const uint16 startPointX = scale.x * 5;
	const POINT& borderSP = { _borderButton.getImageSP().x + startPointX, _borderButton.getImageSP().y + _borderButton.getImagePrintSize().y / 2 };
	for (int i = 0; i < BulletButtonEa; i++)
	{
		const POINT bulletSize{ 27, 90 };
		const float spacing = 1.4f;
		POINT bulletSP = { borderSP.x + (i * (bulletSize.x + spacing) * scale.x), borderSP.y - ((bulletSize.y / 2) / 2) * scale.y };
		auto& bulletButton = _bulletButtons[i];
		bulletButton.resetImage(move(bulletSP), scale);		
		if (i < _activatedCount)
			_bulletButtons[i].changeButtonShape(1);
	}
}

void BulletButtonsController::initBulletButtonsSetting()
{
	_backUpActivatedCount = _activatedCount;

	fillButtons();

	auto& soundMgr = SoundController::getInstance();
	soundMgr.controlVolumes(move(SoundController::generateNameToSoundFile(_borderButton.getButtonTitleName())), _activatedCount * 0.05);
}

void BulletButtonsController::resetBulletButtonsSetting()
{
	for (int i = 0; i < _activatedCount; i++)
	{
		_bulletButtons[i].changeButtonShape(0);
	}

	for (int i = 0; i < _backUpActivatedCount; i++)
	{
		_bulletButtons[i].changeButtonShape(1);
	}

	_activatedCount = _backUpActivatedCount;

	auto& soundCtr = SoundController::getInstance();
	soundCtr.controlVolumes(move(SoundController::generateNameToSoundFile(_borderButton.getButtonTitleName())), _activatedCount * 0.05);
}

void BulletButtonsController::calculateBulletActivatedCount()
{
	//이전 상태가 누르는 중이었다면
	if (_borderButton.getCurrentButtonState().get()->getTypeID() == typeid(Pressed).name())
	{
		controlBulletButtonsByDrag();
	}
	else if (_borderButton.getCurrentButtonState().get()->getTypeID() == typeid(PressUp).name())
	{
		if (_borderButton.getPreviousButtonState() == typeid(Pressed).name())
			return;

		controlBulletButtonsByKeyUp();
	}

	SoundController::getInstance().controlVolumes(move(SoundController::generateNameToSoundFile(_borderButton.getButtonTitleName())), _activatedCount * 0.05);
}

void BulletButtonsController::fillButtons()
{
	for (int i = 0; i < _activatedCount; i++)
		_bulletButtons[i].changeButtonShape(1);
}

void BulletButtonsController::initBorder(const char* borderName, uint16 borderStartY, const FPOINT& scale)
{
	_borderButton.initImage("Images/Scene/Option/Border.bmp", { TextAlignment::alignMiddleCenterX(578), borderStartY }, scale, { 578, 62 }, true);
	_borderButton.initText("돋움", borderName, { TextAlignment::alignMiddleCenterX(borderName), borderStartY - 55 }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_borderButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_borderButton.createAndInitToButtonStateBy(make_shared<Active>(), { "Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
	_borderButton.createAndInitToButtonStateBy(make_shared<Pressed>(true), { "", SOUND_TYPE::UI }, &FunctionsStorage::calculateBulletActivatedCount, { {this, false}, });
	_borderButton.createAndInitToButtonStateBy(make_shared<PressUp>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::calculateBulletActivatedCount, { {this, false}, });
}

void BulletButtonsController::initBulletButtons(const FPOINT& scale)
{	
	_activatedCount = 0;
	_backUpActivatedCount = 0;
	_currentCollisionBulletIdx = 0;

	const uint16 startPointX = scale.x * 5;
	const POINT& borderSP = { _borderButton.getImageSP().x + startPointX, _borderButton.getImageSP().y + _borderButton.getImagePrintSize().y / 2 };
	for (int i = 0; i < BulletButtonEa; i++)
	{
		POINT bulletSize{ 27, 90 };
		const float spacing = 1.4f;
		POINT bulletSP = { borderSP.x + (i * (bulletSize.x + spacing) * scale.x), borderSP.y - ((bulletSize.y / 2) / 2) * scale.y};
		auto& bulletButton = _bulletButtons[i];
		bulletButton.initImage("Images/Scene/Option/Bullet.bmp", move(bulletSP), scale, move(bulletSize), { 1, 2 }, true, { false, false }, MagentaRGB);
		bulletButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr, {});
		bulletButton.createAndInitToButtonStateBy(make_shared<Active>(), { "", SOUND_TYPE::UI }, &FunctionsStorage::checkBulletCollision, { {this, false }, { new int(i), true } });
	}
}

void BulletButtonsController::controlBulletButtonsByKeyUp()
{
	// 커지는 경우
	if (_activatedCount <= _currentCollisionBulletIdx)
	{
		for (int i = _activatedCount; i <= _currentCollisionBulletIdx; i++)
		{
			_bulletButtons[i].changeButtonShape(1);
		}
		
		_activatedCount = _currentCollisionBulletIdx + 1;
	}
	// 작아지거나 같은 경우
	else if (_activatedCount > _currentCollisionBulletIdx)
	{
		for (int i = _activatedCount - 1; i >= _currentCollisionBulletIdx; i--)
		{
			_bulletButtons[i].changeButtonShape(0);
		}

		_activatedCount = _currentCollisionBulletIdx;
	}
}

void BulletButtonsController::controlBulletButtonsByDrag()
{
	// 커지는 경우
	if (_activatedCount <= _currentCollisionBulletIdx)
	{
		for (int i = _activatedCount; i <= _currentCollisionBulletIdx; i++)
		{
			_bulletButtons[i].changeButtonShape(1);
		}
	}
	// 작아지거나 같은 경우(같은 경우에는 넘어간다.(끄지 않는다)
	else if (_activatedCount > _currentCollisionBulletIdx)
	{
		if (_activatedCount != _currentCollisionBulletIdx - 1)
		{
			for (int i = _activatedCount - 1; i > _currentCollisionBulletIdx; i--)
			{
				_bulletButtons[i].changeButtonShape(0);
			}
		}
	}

	_activatedCount = _currentCollisionBulletIdx + 1;
}

void OptionDataController::init(const FPOINT & scale)
{
	const uint16 spX = scale.x * 400;
	const uint16 spY = TextAlignment::alignStartY(910);

	const FPOINT calculatedScale = (scale.x > 1.0f) ? move(FPOINT{ 1.0f, 1.0f }) : scale;

	_cancelButton.initImage({ (TextAlignment::alignMiddleCenterX("Cancel") - spX), spY }, calculatedScale, { 175, 60 });
	_cancelButton.initText("돋움", "Cancel", { (TextAlignment::alignMiddleCenterX("Cancel") - spX), spY }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_cancelButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_cancelButton.createAndInitToButtonStateBy(make_shared<Active>(), { "Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
	_cancelButton.createAndInitToButtonStateBy(make_shared<PressUp>(), { "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::BUTTON_CLICK) }, &FunctionsStorage::moveToBackup, { {this, false} });

	_confirmButton.initImage({ (TextAlignment::alignMiddleCenterX("Confirm") + spX), spY }, calculatedScale, { 200, 60 });
	_confirmButton.initText("돋움", "Confirm", { (TextAlignment::alignMiddleCenterX("Confirm") + spX), spY }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_confirmButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_confirmButton.createAndInitToButtonStateBy(make_shared<Active>(), { "Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
	_confirmButton.createAndInitToButtonStateBy(make_shared<PressUp>(), { "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::BUTTON_CLICK) }, &FunctionsStorage::updateSetData, { {this, false} });
}

void OptionDataController::update()
{
	_cancelButton.update();
	_confirmButton.update();
}

void OptionDataController::render(const HDC& hdc) const
{
	_cancelButton.render(hdc, false, false, true);
	_confirmButton.render(hdc, false, false, true);
}

void OptionDataController::release()
{
	_cancelButton.release();
	_confirmButton.release();

	_bulletButtonsControllerVec.clear();
	_arrowButtonsControllerVec.clear();
}

void OptionDataController::initOptionDatasVec(const char* buttonMenuName)
{
	for (const auto& arrowButton : _arrowButtonsControllerVec)
	{
		_optionDatasVec.emplace_back( move(OpionData{ &arrowButton->getCurrentIdx(), buttonMenuName, arrowButton->getBorderTitleName() }));
	}

	for (const auto& bulletButton : _bulletButtonsControllerVec)
	{
		_optionDatasVec.emplace_back( move(OpionData{ &bulletButton->getActivatedCount(), buttonMenuName, bulletButton->getBorderTitleName() }));
	}
}

void OptionDataController::initControllerDatas() const
{
	for (auto& bulletButtonsController : _bulletButtonsControllerVec)
	{
		bulletButtonsController->initBulletButtonsSetting();
	}

	for (auto& _arrowButtonsController : _arrowButtonsControllerVec)
	{
		_arrowButtonsController->initContents();
	}
}

void OptionDataController::moveToBackup()
{
	for (auto& bulletButtonsController : _bulletButtonsControllerVec)
	{
		bulletButtonsController->resetBulletButtonsSetting();
	}
	
	for (auto& _arrowButtonsController : _arrowButtonsControllerVec)
	{			
		_arrowButtonsController->resetSetting();
	}

	(*_optionPtr)->moveToNewMenu(move(typeid(BaseMenu).name()));
}

void OptionDataController::updateSetData()
{	
	for (auto& bulletButtonsController : _bulletButtonsControllerVec)
	{
		bulletButtonsController->saveCurrentData();
	}

	for (auto& _arrowButtonsController : _arrowButtonsControllerVec)
	{
		_arrowButtonsController->saveSetting();
	}

	(*_optionPtr)->resetToOptionMenuTypes();
	(*_optionPtr)->writeData();
	(*_optionPtr)->setDataChangeRecode(true);
}

void OptionDataController::resetButtons(const FPOINT& scale)
{
	const uint16 spX = scale.x * 400;
	const uint16 spY = TextAlignment::alignStartY(910);
	FPOINT calculatedScale = (scale.x > 1.0f) ? move(FPOINT{ 1.0f, 1.0f }) : scale;

	_cancelButton.resetImage({ (TextAlignment::alignMiddleCenterX("Cancel") - spX), spY }, calculatedScale);
	_cancelButton.resetText(TextManager::generateFontSizeByResolution(), { (TextAlignment::alignMiddleCenterX("Cancel") - spX), spY });

	_confirmButton.resetImage({ (TextAlignment::alignMiddleCenterX("Confirm") + spX), spY }, calculatedScale);
	_confirmButton.resetText(TextManager::generateFontSizeByResolution(), { (TextAlignment::alignMiddleCenterX("Confirm") + spX), spY });
}

void OptionDataController::setArrowButtonsController(ArrowButtonsController& _arrowButtonsController)
{
	_arrowButtonsController.setOptionPtr(_optionPtr);
	_arrowButtonsControllerVec.emplace_back(&_arrowButtonsController);
}

uint16 OptionMenuType::getOptionMenuTypeIndex(string typeID)
{
	int16 idx = -1;
	if (typeID == typeid(BaseMenu).name())
	{
		idx = 0;
	}
	else if (typeID == typeid(GamePlayMenu).name())
	{
		idx = 1;
	}
	else if (typeID == typeid(VideoMenu).name())
	{
		idx = 2;
	}
	else if (typeID == typeid(AudioMenu).name())
	{
		idx = 3;
	}

	return idx;
}

void BaseMenu::init(FPOINT scale)
{
	initButtons(move(scale));
}

void BaseMenu::update()
{
	_gamePlayButton.update();
	_videoButton.update();
	_audioButton.update();
	if (_titleButton)
	{
		_titleButton->update();
	}
}

void BaseMenu::render(const HDC& hdc) const
{
	_gamePlayButton.render(hdc, false, false, true);
	_videoButton.render(hdc, false, false, true);
	_audioButton.render(hdc, false, false, true);
	if (_titleButton)
	{
		_titleButton->render(hdc, false, false, true);
	}
}

void BaseMenu::release()
{
	_gamePlayButton.release();
	_videoButton.release();
	_audioButton.release();

	if (_titleButton)
	{
		_titleButton->release();
		_titleButton.reset();
	}

	_optionPtr.reset();
}

void BaseMenu::makeTitleButton(FPOINT scale)
{
	if (_titleButton == nullptr)
	{
		_titleButton = make_unique<Button>();
		resetImageDatas(move(scale));
		_titleButton->createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
		_titleButton->createAndInitToButtonStateBy(make_shared<Active>(), {"Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
		_titleButton->createAndInitToButtonStateBy(make_shared<PressUp>(), { "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::BUTTON_CLICK) }, &FunctionsStorage::toTitleScene, { {new SCENE_NAME(SCENE_NAME::TITLE), true} });
	}
}

void BaseMenu::deleteTitleButton(FPOINT scale)
{
	if (_titleButton)
	{
		_titleButton->release();
		_titleButton.reset();

		resetImageDatas(move(scale));
	}
}

void BaseMenu::resetImageDatas(const FPOINT& scale)
{
	switch (_titleButton == nullptr)
	{
	case true:
		_gamePlayButton.resetImage({ TextAlignment::alignMiddleCenterX("Gameplay"), TextAlignment::alignStartY(440) }, scale);
		_gamePlayButton.resetText(TextManager::generateFontSizeByResolution(), { TextAlignment::alignMiddleCenterX("Gameplay"), TextAlignment::alignStartY(440) });

		_videoButton.resetImage({ TextAlignment::alignMiddleCenterX("Video"), TextAlignment::alignStartY(540) }, scale);
		_videoButton.resetText(TextManager::generateFontSizeByResolution(), { TextAlignment::alignMiddleCenterX("Video"), TextAlignment::alignStartY(540) });

		_audioButton.resetImage({ TextAlignment::alignMiddleCenterX("Audio"), TextAlignment::alignStartY(640) }, scale);
		_audioButton.resetText(TextManager::generateFontSizeByResolution(), { TextAlignment::alignMiddleCenterX("Audio"), TextAlignment::alignStartY(640) });
		break;
	case false:
		_gamePlayButton.resetImage({ TextAlignment::alignMiddleCenterX("Gameplay"), TextAlignment::alignStartY(430) }, scale);
		_gamePlayButton.resetText(TextManager::generateFontSizeByResolution(), { TextAlignment::alignMiddleCenterX("Gameplay"), TextAlignment::alignStartY(430) });

		_videoButton.resetImage({ TextAlignment::alignMiddleCenterX("Video"), TextAlignment::alignStartY(510) }, scale);
		_videoButton.resetText(TextManager::generateFontSizeByResolution(), { TextAlignment::alignMiddleCenterX("Video"), TextAlignment::alignStartY(510) });

		_audioButton.resetImage({ TextAlignment::alignMiddleCenterX("Audio"), TextAlignment::alignStartY(590) }, scale);
		_audioButton.resetText(TextManager::generateFontSizeByResolution(), { TextAlignment::alignMiddleCenterX("Audio"), TextAlignment::alignStartY(590) });

		_titleButton->initImage({ TextAlignment::alignMiddleCenterX("Title"), TextAlignment::alignStartY(670) }, scale, { 144, 60 });
		_titleButton->initText("돋움", "Title", { TextAlignment::alignMiddleCenterX("Title"), TextAlignment::alignStartY(670) }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
		break;
	}
}

void BaseMenu::initButtons(const FPOINT& scale)
{
	_gamePlayButton.initImage({ TextAlignment::alignMiddleCenterX("Gameplay"), TextAlignment::alignStartY(440) }, scale, { 255, 60 });
	_gamePlayButton.initText("돋움", "Gameplay", { TextAlignment::alignMiddleCenterX("Gameplay"), TextAlignment::alignStartY(440) }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_gamePlayButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_gamePlayButton.createAndInitToButtonStateBy(make_shared<Active>(), { "Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
	_gamePlayButton.createAndInitToButtonStateBy(make_shared<PressUp>(), { "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::BUTTON_CLICK) }, & FunctionsStorage::moveToOptionNewMenu, { { *getOption(), false}, { new string(typeid(GamePlayMenu).name()), true} });

	_videoButton.initImage({ TextAlignment::alignMiddleCenterX("Video"), TextAlignment::alignStartY(540) }, scale, { 144, 60 });
	_videoButton.initText("돋움", "Video", { TextAlignment::alignMiddleCenterX("Video"), TextAlignment::alignStartY(540)}, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_videoButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_videoButton.createAndInitToButtonStateBy(make_shared<Active>(), {"Sounds/UI/CollisionButton.mp3",  SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
	_videoButton.createAndInitToButtonStateBy(make_shared<PressUp>(), { "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::BUTTON_CLICK) }, &FunctionsStorage::moveToOptionNewMenu, { { *getOption(), false}, { new string(typeid(VideoMenu).name()), true} });
		
	_audioButton.initImage({ TextAlignment::alignMiddleCenterX("Audio"), TextAlignment::alignStartY(640) }, scale, { 144, 60 });
	_audioButton.initText("돋움", "Audio", { TextAlignment::alignMiddleCenterX("Audio"), TextAlignment::alignStartY(640) }, TextManager::generateFontSizeByResolution(), WhiteRGB, true);
	_audioButton.createAndInitToButtonStateBy(make_shared<Disabled>(), { "", SOUND_TYPE::END }, nullptr);
	_audioButton.createAndInitToButtonStateBy(make_shared<Active>(), { "Sounds/UI/CollisionButton.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::COLLISION_BUTTON) }, nullptr);
	_audioButton.createAndInitToButtonStateBy(make_shared<PressUp>(), { "Sounds/UI/ButtonClick.mp3", SOUND_TYPE::UI, SoundController::uiListToInt(UI_LIST::BUTTON_CLICK) }, &FunctionsStorage::moveToOptionNewMenu, { { *getOption(), false}, { new string(typeid(AudioMenu).name()), true} });
}

void GamePlayMenu::init(FPOINT scale)
{
	initButtons(scale);	
	_optionDataController.init(move(scale));
	_optionDataController.initOptionDatasVec(this->getTypeID().c_str());
}

void GamePlayMenu::update()
{
	_bulletButtonsController.update();
	_optionDataController.update();
}

void GamePlayMenu::render(const HDC& hdc) const
{
	_bulletButtonsController.render(hdc);
	_optionDataController.render(hdc);
}

void GamePlayMenu::release()
{
	_bulletButtonsController.release();
	_optionDataController.release();
	_optionPtr.reset();
}

void GamePlayMenu::resetImageDatas(const FPOINT& scale)
{
	_bulletButtonsController.resetImageOfButtons("Screen Shake Amount", TextAlignment::alignStartY(265), ScreenManager::getInstance().getCalculatedScale());
	_optionDataController.resetButtons(scale);
}

void GamePlayMenu::initButtons(const FPOINT& scale)
{
	_bulletButtonsController.init("Screen Shake Amount", TextAlignment::alignStartY(265), ScreenManager::getInstance().getCalculatedScale());
	_optionDataController.setOptionPtr(getOption());
	_optionDataController.setBulletButtonsController(_bulletButtonsController);
}

void VideoMenu::init(FPOINT scale)
{
	initButtons(scale);
	_optionDataController.init(move(scale));
	_optionDataController.initOptionDatasVec(this->getTypeID().c_str());
}

void VideoMenu::update()
{
	for (auto& _arrowButtonsController : _arrowButtonsControllers)
		_arrowButtonsController.update();

	_bulletButtonsController.update();
	_optionDataController.update();
}

void VideoMenu::render(const HDC& hdc) const
{
	for (auto& _arrowButtonsController : _arrowButtonsControllers)
		_arrowButtonsController.render(hdc);

	_bulletButtonsController.render(hdc);
	_optionDataController.render(hdc);
}

void VideoMenu::release()
{
	for(auto& _arrowButtonsController : _arrowButtonsControllers)
		_arrowButtonsController.release();

	_bulletButtonsController.release();
	_optionDataController.release();
	_optionPtr.reset();
}

void VideoMenu::resetImageDatas(const FPOINT& scale)
{
	_arrowButtonsControllers[0].resetImageOfButtons("Resolution", ScreenContents::toConstCharPtr(ScreenContents::toResolution(_arrowButtonsControllers[0].getCurrentIdx())), TextAlignment::alignStartY(170), scale);
	_arrowButtonsControllers[1].resetImageOfButtons("Scaling Mode", ScreenContents::toConstCharPtr(ScreenContents::toScalingMode(_arrowButtonsControllers[1].getCurrentIdx())), TextAlignment::alignStartY(360), scale);
	_arrowButtonsControllers[2].resetImageOfButtons("Screen Mode", ScreenContents::toConstCharPtr(ScreenContents::toScreenMode(_arrowButtonsControllers[2].getCurrentIdx())), TextAlignment::alignStartY(550), scale);

	_bulletButtonsController.resetImageOfButtons("Gamma", TextAlignment::alignStartY(780), scale);

	_optionDataController.resetButtons(scale);
}

void VideoMenu::initButtons(const FPOINT& scale)
{
	_arrowButtonsControllers[0].init("Resolution", ScreenContents::toConstCharPtr(ScreenContents::toResolution(_arrowButtonsControllers[0].getCurrentIdx())), TextAlignment::alignStartY(170), scale);
	_arrowButtonsControllers[0].setVideoMenuPtr(this);
	_arrowButtonsControllers[1].init("Scaling Mode", ScreenContents::toConstCharPtr(ScreenContents::toScalingMode(_arrowButtonsControllers[1].getCurrentIdx())), TextAlignment::alignStartY(360), scale);
	_arrowButtonsControllers[2].init("Screen Mode", ScreenContents::toConstCharPtr(ScreenContents::toScreenMode(_arrowButtonsControllers[2].getCurrentIdx())), TextAlignment::alignStartY(550), scale);

	_bulletButtonsController.init("Gamma", TextAlignment::alignStartY(780), scale);

	_optionDataController.setOptionPtr(getOption());
	_optionDataController.setBulletButtonsController(_bulletButtonsController);
	for(auto& arrowButtonsController : _arrowButtonsControllers)
		_optionDataController.setArrowButtonsController(arrowButtonsController);
}

void AudioMenu::init(FPOINT scale)
{
	initButtons(scale);
	_optionDataController.init(move(scale));
	_optionDataController.initOptionDatasVec(this->getTypeID().c_str());
}

void AudioMenu::update()
{
	for (auto& bulletButtonsController : _bulletButtonsControllers)
		bulletButtonsController.update();

	_optionDataController.update();
}

void AudioMenu::render(const HDC& hdc) const
{
	for (const auto& bulletButtonsController : _bulletButtonsControllers)
		bulletButtonsController.render(hdc);

	_optionDataController.render(hdc);
}

void AudioMenu::release()
{
	for (auto& bulletButtonsController : _bulletButtonsControllers)
		bulletButtonsController.release();

	_optionDataController.release();
	_optionPtr.reset();
}

void AudioMenu::resetImageDatas(const FPOINT& scale)
{
	_bulletButtonsControllers[0].resetImageOfButtons("Music Volume", TextAlignment::alignStartY(240), scale);
	_bulletButtonsControllers[1].resetImageOfButtons("SFX Volume", TextAlignment::alignStartY(410), scale);
	_bulletButtonsControllers[2].resetImageOfButtons("UI Volume", TextAlignment::alignStartY(580), scale);

	_optionDataController.resetButtons(scale);
}

void AudioMenu::initButtons(const FPOINT& scale)
{
	_bulletButtonsControllers[0].init("Music Volume", TextAlignment::alignStartY(240), scale);
	_bulletButtonsControllers[1].init("SFX Volume", TextAlignment::alignStartY(410), scale);
	_bulletButtonsControllers[2].init("UI Volume", TextAlignment::alignStartY(580), scale);

	_optionDataController.setOptionPtr(getOption());

	auto& soundMgr = SoundController::getInstance();
	for (auto& bulletButtonsController : _bulletButtonsControllers)
	{
		_optionDataController.setBulletButtonsController(bulletButtonsController);
	}
}

void Option::init(POINT startPoint, FPOINT scale)
{
	setDataChangeRecode(false);

	_scale = move(scale);
	_startPosition = move(startPoint);

	const POINT& matrixScreen = move(POINT{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) });
	_background = ImageManager::makeImage("Images/Scene/Option/Background.bmp", _startPosition, {1.0f, 1.0f}, { matrixScreen.x, matrixScreen.y * 2 }, { 1, 2 }, false, { false, false }, MagentaRGB);

	createOptionMenuTypes();
	_currentOptionMenuIdx = OptionMenuType::getOptionMenuTypeIndex(typeid(BaseMenu).name());

	_dataHandler.init("E:/Repository/EnterTheGungeon(Remake-Ver)/EnterTheGungeon(Remake-Ver)/Ini/Option.ini", this);

	initDataOfOptionMenuTypes();
}

void Option::update()
{
	_optionMenuTypePtrs[_currentOptionMenuIdx]->update();
}

void Option::render(const HDC& hdc) const
{
	_background.renderFrame(hdc, { 0, 0 });
	_optionMenuTypePtrs[_currentOptionMenuIdx]->render(hdc);
}

void Option::release()
{
	releaseAllOptionMenuTypeDatas();
}

void Option::resetOptionAndOptionMenu(POINT startPoint, FPOINT scale)
{
	reset(move(startPoint), move(scale));
	ImageManager::resetImage(_background, _startPosition, move(ScreenManager::getInstance().getSubResolution()));

	for (const auto& optionMenu : _optionMenuTypePtrs)
	{
		optionMenu->resetImageDatas(_scale);
	}
}

void Option::resetImageScale(POINT startPoint, FPOINT scale)
{
	reset(move(startPoint), move(scale));
	resetBackground();

	_optionMenuTypePtrs[OptionMenuType::getOptionMenuTypeIndex(typeid(VideoMenu).name())]->resetImageDatas(_scale);
}

void Option::resetToOptionMenuTypes()
{
	switch (getCurrentUseMenuType() == typeid(VideoMenu).name())
	{
	case true:
		for (auto& optionMenuType : _optionMenuTypePtrs)
		{
			if (optionMenuType.get()->getTypeID() != typeid(VideoMenu).name())
			{
				optionMenuType.get()->resetImageDatas(_scale);
				continue;
			}

			switch ((*_scenePtr)->getSceneID())
			{
			case SCENE_NAME::GAME:
				dynamic_cast<GameScene*>(*_scenePtr)->setGamma(getVideoMenuGammaCount());
				break;
			case SCENE_NAME::MAPTOOL:
				dynamic_cast<MapToolScene*>(*_scenePtr)->setGamma(getVideoMenuGammaCount());
				break;
			case SCENE_NAME::TITLE:
				dynamic_cast<TitleScene*>(*_scenePtr)->setGamma(getVideoMenuGammaCount());
				break;
			default:
				assert(false);
				break;
			}	
		}

		resetBackground(typeid(BaseMenu).name());
		break;
	case false:
		moveToNewMenu(typeid(BaseMenu).name());
		break;
	}
}

uint16 Option::getVideoMenuGammaCount() const
{
	for (auto& optionMenuType : _optionMenuTypePtrs)
	{
		if (optionMenuType.get()->getTypeID() == typeid(VideoMenu).name())
		{
			return dynamic_cast<VideoMenu*>(optionMenuType.get())->getGammaCount();
		}
	}

	assert(false);
	return 0;
}

void Option::writeData() const
{	
	switch (OptionMenuType::compareMenuType(getCurrentUseMenuType().c_str(), typeid(BaseMenu).name()) == false)
	{
	case true:
		_dataHandler.writeIniFile();
		break;
	case false:
		assert(0);
		break;
	}
}

void Option::readAndWriteIniFile(const char* iniFile)
{
	initOptionDatasVec();
	for (auto& optionData : _optionDatasVec)
	{
		const auto& document = optionData.getDocument();
		uint16 value = ::GetPrivateProfileIntA(document.section, document.name, 0, iniFile);
		optionData.setValue(value);
	}
	_optionDatasVec.clear();
}

void Option::writeIniFile(const char* iniFile)
{
	initOptionDatasVec();
	::WritePrivateProfileStringA("OptionDatas", "Existence", "True", iniFile);
	for (const auto& optionData : _optionDatasVec)
	{
		const auto& document = optionData.getDocument();
		::WritePrivateProfileStringA(document.section, document.name, to_string(*document.value).c_str(), iniFile);
	}
	_optionDatasVec.clear();
}

void Option::reset(POINT startPoint, FPOINT scale)
{
	_startPosition = move(startPoint);
	_scale = move(scale);
}

void Option::resetBackground(string typeID)
{
	ImageManager::resetImage(_background, _startPosition, move(ScreenManager::getInstance().getSubResolution()));

	moveToNewMenu(move(typeID));
}

void Option::createOptionMenuTypes()
{
	_optionMenuTypePtrs[0] = make_unique<BaseMenu>();
	_optionMenuTypePtrs[1] = make_unique<GamePlayMenu>();
	_optionMenuTypePtrs[2] = make_unique<VideoMenu>();
	_optionMenuTypePtrs[3] = make_unique<AudioMenu>();

	const auto& optionPtr = make_shared<Option*>(this);
	for (const auto& optionMenuTypePtr : _optionMenuTypePtrs)
	{
		optionMenuTypePtr->setOption(optionPtr);
		optionMenuTypePtr->init(_scale);
	}
}

void Option::initDataOfOptionMenuTypes()
{
	for (const auto& optionMenuTypePtr : _optionMenuTypePtrs)
	{
		optionMenuTypePtr->initDatas();
	}
}

void Option::releaseAllOptionMenuTypeDatas()
{
	for (auto& optionMenuTypePtr : _optionMenuTypePtrs)
	{
		if (optionMenuTypePtr)
		{
			optionMenuTypePtr->release();
			optionMenuTypePtr.reset();
		}
	}
}

void Option::moveToNewMenu(string typeID)
{
	int16 frameY = -1;
	frameY = (typeID == typeid(BaseMenu).name()) ? 0 : 1;
	assert(frameY != -1);
	_background.setCurrentKeyFrameY(frameY);
	_currentOptionMenuIdx = OptionMenuType::getOptionMenuTypeIndex(move(typeID));
}

void Option::initOptionDatasVec()
{
	_optionDatasVec.reserve(10);
	for (const auto& optionMenuType : _optionMenuTypePtrs)
	{
		if (optionMenuType.get()->getTypeID() == typeid(BaseMenu).name())
		{
			continue;
		}

		for (const auto& optionData : optionMenuType->getOptionDataController().getOptionDatasVec())
		{
			_optionDatasVec.emplace_back(optionData);
		}
	}
}
