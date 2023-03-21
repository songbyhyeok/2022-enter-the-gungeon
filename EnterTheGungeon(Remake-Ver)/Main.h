#pragma once

#include "pch.h"

class CameraController;
class Main
{
public:
	HRESULT								init();
	void								update();
	void								render() const;
	void								release();

	LRESULT								mainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool								checkInit() const { return _isInit; };

private:
	shared_ptr<CameraController>		_cameraController;
	bool								_isInit;
};

