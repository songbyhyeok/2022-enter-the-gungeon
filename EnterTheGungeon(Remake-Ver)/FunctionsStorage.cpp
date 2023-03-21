#include "FunctionsStorage.h"
#include "SceneManager.h"
#include "Category.h"

void FunctionsStorage::setIconInformation(const vector<VPointerData>& vPointerDataVec) const
{
	Palette& palette = *static_cast<Palette*>(vPointerDataVec[0].first);
	Inventory& inventory = *static_cast<Inventory*>(vPointerDataVec[1].first);
	ICON_INFORMATION iconInformation = *static_cast<ICON_INFORMATION*>(vPointerDataVec[2].first);
	palette.setIconInformation(move(iconInformation), inventory);
}

void FunctionsStorage::setIconMatrix(const vector<VPointerData>& vPointerDataVec) const
{
	Category& category = *static_cast<Category*>(vPointerDataVec[0].first);
	POINT value = *static_cast<POINT*>(vPointerDataVec[1].first);
	category.setIconMatrix(move(value));
}

void FunctionsStorage::setIconInfoTransparency(const vector<VPointerData>& vPointerDataVec) const
{
	Category& category = *static_cast<Category*>(vPointerDataVec[0].first);
	const uint16 idx = *static_cast<uint16*>(vPointerDataVec[1].first);
	const BYTE alpha = *static_cast<BYTE*>(vPointerDataVec[2].first);
	category.setIconInfoTransparency(idx, alpha);
}

void FunctionsStorage::setButtonTransparency(const vector<VPointerData>& vPointerDataVec) const
{
	Button& button = *static_cast<Button*>(vPointerDataVec[0].first);
	const BYTE alpha = *static_cast<BYTE*>(vPointerDataVec[1].first);
	button.setTransparency(alpha);
}

void FunctionsStorage::toTitleScene(const vector<VPointerData>& vPointerDataVec) const
{
	SceneManager::toTitleScene(*static_cast<SCENE_NAME*>(vPointerDataVec[0].first));
}

void FunctionsStorage::sceneChange(const vector<VPointerData>& vPointerDataVec) const
{
	SCENE_NAME newSceneNamePtr = *static_cast<SCENE_NAME*>(vPointerDataVec[0].first);
	switch (vPointerDataVec.size())
	{
	case 1:
		SceneManager::changeScene(move(newSceneNamePtr));
		break;
	case 2:
		SCENE_NAME newLoadingSceneNamePtr = *static_cast<SCENE_NAME*>(vPointerDataVec[1].first);
		SceneManager::changeScene(move(newSceneNamePtr), move(newLoadingSceneNamePtr));
		break;
	}
}

void FunctionsStorage::moveToBackup(const vector<VPointerData>& vPointerDataVec) const
{
	OptionDataController& optionDataController = *static_cast<OptionDataController*>(vPointerDataVec[0].first);
	optionDataController.moveToBackup();
}

void FunctionsStorage::updateSetData(const vector<VPointerData>& vPointerDataVec) const
{
	OptionDataController& optionDataController = *static_cast<OptionDataController*>(vPointerDataVec[0].first);
	optionDataController.updateSetData();
}

void FunctionsStorage::moveToOptionNewMenu(const vector<VPointerData>& vPointerDataVec) const
{
	Option& option = *static_cast<Option*>(vPointerDataVec[0].first);
	string newOptionTypeID = *static_cast<string*>(vPointerDataVec[1].first);
	option.moveToNewMenu(move(newOptionTypeID));
}

void FunctionsStorage::changeButtonShape(const vector<VPointerData>& vPointerDataVec) const
{
	Button& button = *static_cast<Button*>(vPointerDataVec[0].first);
	uint16 keyFrameY = *static_cast<uint16*>(vPointerDataVec[1].first);
	button.changeButtonShape(keyFrameY);
}

void FunctionsStorage::checkBulletCollision(const vector<VPointerData>& vPointerDataVec) const
{
	BulletButtonsController& bulletBtnController = *static_cast<BulletButtonsController*>(vPointerDataVec[0].first);
	uint16 bulletIdx = *static_cast<uint16*>(vPointerDataVec[1].first);
	bulletBtnController.checkBulletCollision(bulletIdx);
}

void FunctionsStorage::calculateBulletActivatedCount(const vector<VPointerData>& vPointerDataVec) const
{
	BulletButtonsController& bulletBtnController = *static_cast<BulletButtonsController*>(vPointerDataVec[0].first);
	bulletBtnController.calculateBulletActivatedCount();
}

void FunctionsStorage::moveToNextContent(const vector<VPointerData>& vPointerDataVec) const
{
	ArrowButtonsController& arrowButtonsController = *static_cast<ArrowButtonsController*>(vPointerDataVec[0].first);
	int16 value = *static_cast<int16*>(vPointerDataVec[1].first);	
	arrowButtonsController.moveToNextContent(value);
}

void FunctionsStorage::activateSelectedCategory(const vector<VPointerData>& vPointerDataVec) const
{
	Inventory& inventory = *static_cast<Inventory*>(vPointerDataVec[0].first);
	CATEGORY_TYPE selectedCategoryType = *static_cast<CATEGORY_TYPE*>(vPointerDataVec[1].first);
	inventory.activateSelectedCategory(move(selectedCategoryType));
}

void FunctionsStorage::pullScrollBar(const vector<VPointerData>& vPointerDataVec) const
{
	Scroll& scroll = *static_cast<Scroll*>(vPointerDataVec[0].first);
	scroll.pullBar();
}

void FunctionsStorage::putScrollBar(const vector<VPointerData>& vPointerDataVec) const
{
	Scroll& scroll = *static_cast<Scroll*>(vPointerDataVec[0].first);
	scroll.putBar();
}

void FunctionsStorage::handleFile(const vector<VPointerData>& vPointerDataVec) const
{
	MapTool& mapTool = *static_cast<MapTool*>(vPointerDataVec[0].first);
	FILE_MODE fileMode = *static_cast<FILE_MODE*>(vPointerDataVec[1].first);
	mapTool.handleFile(move(fileMode));
}

void FunctionsStorage::clearCanvas(const vector<VPointerData>& vPointerDataVec) const
{
	MapTool& mapTool = *static_cast<MapTool*>(vPointerDataVec[0].first);
	mapTool.clearCanvas();
}

void FunctionsStorage::quit(const vector<VPointerData>& vPointerDataVec) const
{
	PostQuitMessage(0);
}
