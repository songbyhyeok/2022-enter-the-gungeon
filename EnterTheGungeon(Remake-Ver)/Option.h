#pragma once

#include "BaseInterface.h"
#include "Button.h"

class Option;
class VideoMenu;
class ArrowButtonsController : public BaseInterface
{
public:
	void					init(const char* borderName, const char* contents, uint16 startPointY, const FPOINT& scale = {});
	void					update() override;
	void					render(const HDC& hdc) const override;
	void					release() override;

	void					setOptionPtr(const shared_ptr<Option*> optionPtr) { _optionPtr = optionPtr; }
	void					setVideoMenuPtr(VideoMenu * videoMenuPtr) { _videoMenuPtr = make_unique<VideoMenu*>(videoMenuPtr); }
	const char*				getBorderTitleName() const { return _borderButton.getButtonTitleName(); }	
	uint16&					getCurrentIdx() { return _currentIdx; }

	// 수정된 적이 있는지 검사하는 함수
	bool					checkModifiedTrace() const { return _backUpCurrentIdx == _currentIdx; }

	void					initContents();
	void					initContents(ResolutionList resolution, ScalingModeList scalingMode, ScreenModeList screenMode);
	void					resetImageOfButtons(const char* borderName, const char* contents, uint16 startPointY, const FPOINT& scale = {});
	void					resetSetting();
	void					saveSetting() { _backUpCurrentIdx = _currentIdx; }
	void					moveToNextContent(int16 idx);
	
private:
	void					initBorder(const char* borderName, const char* contents, uint16 startPointY, const FPOINT& scale);
	void					initArrowButton(const FPOINT& scale = {});

private:
	Button					_borderButton;
	Button					_leftArrowButton;
	Button					_rightArrowButton;
	TextHandler				_contents;
	shared_ptr<Option*>		_optionPtr;
	unique_ptr<VideoMenu*>	_videoMenuPtr;
	uint16					_currentIdx;
	uint16					_backUpCurrentIdx;	
};

constexpr uint16 BulletButtonEa = 20;
class BulletButtonsController : public BaseInterface
{
public:
	void									init(const char* borderName, uint16 borderStartY, const FPOINT& scale);
	void									update() override;
	void									render(const HDC& hdc) const override;
	void									release() override;

	const char*								getBorderTitleName() const { return _borderButton.getButtonTitleName(); }
	void									setActivatedCount(uint16 ea) { _activatedCount = ea; }
	uint16&									getActivatedCount() { return _activatedCount; }

	void									resetImageOfButtons(const char* borderName, uint16 borderStartY, const FPOINT& scale);
	void									setBackupActivatedCount(uint16 ea) { _backUpActivatedCount = ea; }
	void									initBulletButtonsSetting();
	void									resetBulletButtonsSetting();
	void									saveCurrentData() { _backUpActivatedCount = _activatedCount; }
	void									checkBulletCollision(uint16 bulletIdx) { _currentCollisionBulletIdx = bulletIdx; }
	void									calculateBulletActivatedCount();
	void									fillButtons();

private:
	void									initBorder(const char* borderName, uint16 borderStartY, const FPOINT& scale);
	void									initBulletButtons(const FPOINT &scale);
	void									controlBulletButtonsByKeyUp();
	void									controlBulletButtonsByDrag();

private:
	array<Button, BulletButtonEa>			_bulletButtons;
	Button									_borderButton;
	uint16									_activatedCount;
	uint16									_backUpActivatedCount;
	uint16									_currentCollisionBulletIdx;
};

struct DOCUMENT
{
public:
	uint16*			value;
	const char*		section;
	const char*		name;
};

class OpionData
{
public:
	OpionData(uint16* valuePtr, const char* section, const char* name) { setDocument( valuePtr, move(section), move(name)); }

	void			setValue(uint16 value) { *_document.value = value; }
	void			setDocument(uint16* valuePtr, const char* section, const char* name) { _document = { valuePtr, move(section), move(name) }; }
	const DOCUMENT& getDocument() const { return _document; }

private:
	DOCUMENT		_document;
};

class Option;
class OptionDataController : BaseInterface
{
public:
	void									init(const FPOINT& scale);
	void									update() override;
	void									render(const HDC& hdc) const override;
	void									release() override;

	void									initOptionDatasVec(const char* buttonMenuName);
	const vector<OpionData>&				getOptionDatasVec() const { return _optionDatasVec; }		
	void									initControllerDatas() const;					

	void									setOptionPtr(const shared_ptr<Option*> optionPtr) { _optionPtr = optionPtr; }					

	void									resetButtons(const FPOINT& scale);
	void									setBulletButtonsController(BulletButtonsController& bulletButtonsController) { _bulletButtonsControllerVec.emplace_back(&bulletButtonsController); }
	void									setArrowButtonsController(ArrowButtonsController& _arrowButtonsController);
	void									moveToBackup();
	void									updateSetData();

private:
	Button									_cancelButton;
	Button									_confirmButton;
	vector<OpionData>						_optionDatasVec;
	vector<BulletButtonsController*>		_bulletButtonsControllerVec;
	vector<ArrowButtonsController*>			_arrowButtonsControllerVec;
	shared_ptr<Option*>						_optionPtr;
};

class Option;
class OptionMenuType : public BaseInterface
{
public:
	virtual void						init(FPOINT scale) = 0;

	virtual void						initDatas() { _optionDataController.initControllerDatas(); }
	virtual OptionDataController&		getOptionDataController() { return _optionDataController; }
	virtual void						resetImageDatas(const FPOINT& scale) = 0;

	void								setOption(const shared_ptr<Option*>& optionPtr) { _optionPtr = optionPtr; }
	const shared_ptr<Option*>&			getOption() const { return _optionPtr; }
	const string&						getTypeID() const { return _typeID; }
	static uint16						getOptionMenuTypeIndex(string typeID);
	static bool							compareMenuType(const char* a, const char* b) { return a == b; }

	virtual								~OptionMenuType() = default;

protected:
	virtual void						initButtons(const FPOINT& scale) = 0;

protected:		
	OptionDataController					_optionDataController;
	string									_typeID;
	shared_ptr<Option*>						_optionPtr;		

private:
	
};

class BaseMenu : public OptionMenuType
{
public:
	BaseMenu() { _typeID = typeid(BaseMenu).name(); }

	void init(FPOINT scale) override;
	void update() override;
	void render(const HDC& hdc) const override;
	void release() override;

	void makeTitleButton(FPOINT scale);
	void deleteTitleButton(FPOINT scale);
	void resetImageDatas(const FPOINT& scale) override;	

private:
	void initButtons(const FPOINT& scale) override;

private:
	unique_ptr<Button>	_titleButton;				
	Button				_gamePlayButton;
	Button				_videoButton;
	Button				_audioButton;
};

class GamePlayMenu : public OptionMenuType
{
public:
	GamePlayMenu() { _typeID = typeid(GamePlayMenu).name(); }

	void init(FPOINT scale) override;	
	void update() override;
	void render(const HDC& hdc) const override;
	void release() override;

	void resetImageDatas(const FPOINT& scale) override;

private:
	void initButtons(const FPOINT& scale) override;

private:
	BulletButtonsController					_bulletButtonsController;
	Button									_screenShakableButton;
};


class VideoMenu : public OptionMenuType
{
public:
	VideoMenu() { _typeID = typeid(VideoMenu).name(); }

	void			init(FPOINT scale) override;
	void			update() override;
	void			render(const HDC& hdc) const override;
	void			release() override;

	void			resetImageDatas(const FPOINT& scale) override;	
	void			setScreenMode(ScreenModeList screenMode) { _arrowButtonsControllers[2].initContents(ResolutionList::END, ScalingModeList::END, screenMode); }
	const uint16	getGammaCount() { return _bulletButtonsController.getActivatedCount(); }

private:
	void			initButtons(const FPOINT& scale) override;

private:
	array<ArrowButtonsController, 3>		_arrowButtonsControllers;
	BulletButtonsController					_bulletButtonsController;
};

class AudioMenu : public OptionMenuType
{
public:
	AudioMenu() { _typeID = typeid(AudioMenu).name(); }

	void init(FPOINT scale) override;
	void update() override;
	void render(const HDC& hdc) const override;
	void release() override;

	void resetImageDatas(const FPOINT& scale) override;

private:
	void initButtons(const FPOINT& scale) override;

private:
	array<BulletButtonsController, 3>			_bulletButtonsControllers;
};

#include "DataHandler.h"

constexpr uint16 OptionMenuTypeEA = 4;

class MapTool;
class Scene;
class Option : public BaseInterface
{
public:
	void													init(POINT startPoint, FPOINT scale);
	void													update() override;
	void													render(const HDC& hdc) const override;
	void													release() override;
	
	void													setMapToolPtr(MapTool* mapToolPtr) { _mapToolPtr = make_unique<MapTool*>(mapToolPtr); }
	void													setScenePtr(Scene* scenePtr) { _scenePtr = make_unique<Scene*>(scenePtr); }
	void													resetOptionAndOptionMenu(POINT startPoint, FPOINT scale);
	void													resetImageScale(POINT startPoint, FPOINT scale);
	void													resetToOptionMenuTypes();
	void													moveToNewMenu(string typeID);
	const string&											getCurrentUseMenuType() const { return _optionMenuTypePtrs[_currentOptionMenuIdx]->getTypeID(); }
	void													requestToMakeTitleButtonByBaseMenu() { (*dynamic_cast<BaseMenu*>(_optionMenuTypePtrs[0].get())).makeTitleButton(_scale); }
	void													requestToDeleteTitleButtonByBaseMenu() { (*dynamic_cast<BaseMenu*>(_optionMenuTypePtrs[0].get())).deleteTitleButton(_scale); }
	void													setCommonSceneName(const char* sceneName) { _commonSceneName = move(sceneName); }
	const char*												getCommonSceneName() const { return _commonSceneName; }
	void													setDataChangeRecode(bool isKeepDataChangeRecord) { _isDataChanged = isKeepDataChangeRecord; }
	bool													isDataChanged() const { return _isDataChanged; }	
	uint16													getVideoMenuGammaCount() const;
	void													writeData() const;
	void													readAndWriteIniFile(const char* iniFile);
	void													writeIniFile(const char* iniFile);

private:	
	void													reset(POINT startPoint, FPOINT scale);
	void													resetBackground(string typeID = typeid(VideoMenu).name());
	void													createOptionMenuTypes();
	void													initDataOfOptionMenuTypes();
	void													releaseAllOptionMenuTypeDatas();
	void													initOptionDatasVec();

private:
	Image													_background;
	array<unique_ptr<OptionMenuType>, OptionMenuTypeEA>		_optionMenuTypePtrs;
	vector<OpionData>										_optionDatasVec;
	DataHandler												_dataHandler;	
	unique_ptr<MapTool*>									_mapToolPtr;
	unique_ptr<Scene*>										_scenePtr;
	FPOINT													_scale;
	POINT													_startPosition;
	uint16													_currentOptionMenuIdx;
	const char*												_commonSceneName;
	bool													_isDataChanged;
};
