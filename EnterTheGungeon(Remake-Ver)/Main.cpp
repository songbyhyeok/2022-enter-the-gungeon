#include "Main.h"
#include "TimeHandler.h"
#include "SceneManager.h"
#include "CameraController.h"
#include "SoundController.h"
#include "ImageManager.h"
#include "KeyController.h"
#include "MouseHandler.h"

HRESULT Main::init()
{
    _isInit = true;

    TimeHandler::getInstance().init();
    TimeHandler::getInstance().setFPS(30.0f);

    SoundController::getInstance().init();

    KeyController::init();

    MouseHandler::init();

    _cameraController = make_shared<CameraController>();
    _cameraController->init();

    ScreenManager::getInstance().setCameraController(_cameraController);
    SceneManager::requestOptionInit();
    SceneManager::changeScene(SCENE_NAME::TITLE, SCENE_NAME::LOADING);
    //SceneManager::changeScene(SCENE_NAME::TITLE);
    //SceneManager::changeScene(SCENE_NAME::OPTION);
    //SceneManager::changeScene(SCENE_NAME::MAPTOOL);
    //SceneManager::changeScene(SCENE_NAME::GAME);
    
    return S_OK;
}

void Main::update()
{
    if (checkInit() == false) return;

    TimeHandler::getInstance().update();
    SoundController::getInstance().update();
    SceneManager::update();
    _cameraController->update();
}

void Main::render() const
{
    if (checkInit() == false) return;

    _cameraController->render();
}

void Main::release()
{    
    SceneManager::allRelease();
    SoundController::getInstance().release();
    ImageManager::allDelete();
    _cameraController->release();
}

LRESULT Main::mainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        /*case WM_TIMER:
            break;*/
            //case WM_PAINT:
            //{
            //    PAINTSTRUCT ps;
            //    //HDC hdc;
            //    ::BeginPaint(hWnd, &ps);
            //    // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            //    /*if (_isInit)
            //        this->render();*/

            //    ::EndPaint(hWnd, &ps);
            //}
           // break;
    case WM_MOUSEMOVE:
        if (_isInit)
        {
            MouseHandler::update(POINT{ LOWORD(lParam), HIWORD(lParam) });
        }
        break;
        /*case WM_KEYDOWN:
            switch (wParam)
            {
            case VK_F1:
                ::PostQuitMessage(0);
                break;
            }
            break;*/
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    }

    return ::DefWindowProc(move(hWnd), move(message), move(wParam), move(lParam));
}
