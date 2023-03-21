// Enter the Gungeon.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "EnterTheGungeon(Remake-Ver).h"
#include "Main.h"
#include "ScreenManager.h"
//#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE               G_hInstance;                                // 현재 인스턴스입니다.
HWND                    G_hWnd;                                     // 윈도우 핸들
WCHAR                   G_szWindowClass[30];                        // 기본 창 클래스 이름입니다.
unique_ptr<Main>        G_mainPtr;

//WCHAR szTitle[MAX_LOADSTRING];                                    // 제목 표시줄 텍스트입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow);
void                InitWindowClassName(const char* name);
ATOM                MyRegisterClass();
BOOL                InitInstance(int nCmdShow);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 정의
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    G_hInstance = move(hInstance); // 인스턴스 핸들을 전역 변수에 저장합니다.

    // 전역 문자열을 초기화합니다.
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_IDC_ENTERTHEGUNGEON, szWindowClass, MAX_LOADSTRING);
    InitWindowClassName("Hyeok");
    MyRegisterClass();

    // 애플리케이션 초기화를 수행합니다:
    InitInstance(nCmdShow);

    HACCEL hAccelTable = LoadAccelerators(G_hInstance, MAKEINTRESOURCE(IDC_ENTERTHEGUNGEONREMAKEVER));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                if (msg.message == WM_QUIT)
                    break;

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            G_mainPtr->update();
            G_mainPtr->render();
        }
    }

    G_mainPtr->release();

    /*while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }*/

    return (int)msg.wParam;
}

void InitWindowClassName(const char* name)
{
    int nameLen = strlen(name);
    for (int i = 0; i < nameLen; ++i)
    {
        G_szWindowClass[i] = name[i];
    }
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass()
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = G_hInstance;
    wcex.hIcon = LoadIcon(nullptr, MAKEINTRESOURCE(IDI_ENTERTHEGUNGEONREMAKEVER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = G_szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(int nCmdShow)
{
    ScreenManager& screenMgr = ScreenManager::getInstance();
    screenMgr.init(false, true);
    screenMgr.setMainAspect(move(POINT{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) }));
    screenMgr.setSubAspect(screenMgr.generateToResolution(ResolutionList::W1920H1080));
    screenMgr.setSubResolutionValue(move(screenMgr.getSubResolution()));
    screenMgr.initScreen();

    ShowWindow(G_hWnd, nCmdShow);
    UpdateWindow(G_hWnd);

    G_mainPtr = make_unique<Main>();
    G_mainPtr->init();

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{    
    return G_mainPtr->mainProc(move(hWnd), move(message), move(wParam), move(lParam));
}

// 정보 대화 상자의 메시지 처리기입니다.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}
