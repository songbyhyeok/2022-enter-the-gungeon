#pragma once

#include "pch.h"

class FunctionsStorage 
{
public:
	void setIconInformation(const vector<VPointerData>& vPointerDataVec) const;
	void setIconMatrix(const vector<VPointerData>& vPointerDataVec) const;
	void setIconInfoTransparency(const vector<VPointerData>& vPointerDataVec) const;
	void setButtonTransparency(const vector<VPointerData>& vPointerDataVec) const;

	void toTitleScene(const vector<VPointerData>& vPointerDataVec) const;
	void sceneChange(const vector<VPointerData>& vPointerDataVec) const;
	void moveToBackup(const vector<VPointerData>& vPointerDataVec) const;
	void updateSetData(const vector<VPointerData>& vPointerDataVec) const;
	void moveToOptionNewMenu(const vector<VPointerData>& vPointerDataVec) const;
	void changeButtonShape(const vector<VPointerData>& vPointerDataVec) const;
	void checkBulletCollision(const vector<VPointerData>& vPointerDataVec) const;
	void calculateBulletActivatedCount(const vector<VPointerData>& vPointerDataVec) const;
	void moveToNextContent(const vector<VPointerData>& vPointerDataVec) const;
	void activateSelectedCategory(const vector<VPointerData>& vPointerDataVec) const;
	void pullScrollBar(const vector<VPointerData>& vPointerDataVec) const;
	void putScrollBar(const vector<VPointerData>& vPointerDataVec) const;
	void handleFile(const vector<VPointerData>& vPointerDataVec) const;
	void clearCanvas(const vector<VPointerData>& vPointerDataVec) const;
	void quit(const vector<VPointerData>& vPointerDataVec) const;
};

