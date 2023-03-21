#pragma once

#include <Windows.h>
#include <tchar.h>
#include <cassert>
#include <memory>
#include <bitset>
#include <functional>
#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <array>
#include <queue>
#include <map>

using namespace std;

using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

extern HWND                         G_hWnd;
extern HINSTANCE                    G_hInstance;
extern WCHAR                        G_szWindowClass[30];

constexpr double PI = 3.141592;
constexpr double Degree = 180 / PI;

constexpr COLORREF WhiteRGB{ RGB(255, 255, 255) };
constexpr COLORREF GrayRGB{ RGB(129, 129, 129) };
constexpr COLORREF MagentaRGB{ RGB(255, 0, 255) };
constexpr COLORREF IvoryRGB{ RGB(236, 216, 192) };
constexpr COLORREF BlackRGB{ RGB(0, 0, 0) };

struct FPOINT
{
public:
    FPOINT() = default;
    FPOINT(float aX, float aY) : x(aX), y(aY) {}
    float x;
    float y;
};

typedef pair<void*, bool> VPointerData;

inline bool compareSourceNameBy(const char* a, const char* b) { return a == b; }

inline uint32 floatToInt(float fValue) { return static_cast<uint32>(fValue); }

inline RECT generateRect(POINT sp, POINT size)
{
    return { sp.x, sp.y, sp.x + size.x, sp.y + size.y };
}

// GDC를 구하는 알고리즘
inline int16 euclidean(int16 a, int16 b)
{
    while (a % b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }

    return b;
}

inline double getDistance(const POINT& p1, const POINT& p2)
{
    double d1x{ static_cast<double>(p1.x)};
    double d1y{ static_cast<double>(p1.y)};
    double d2x{ static_cast<double>(p2.x)};
    double d2y{ static_cast<double>(p2.y)};
    return sqrt(pow(d1x - d2x, 2) + pow(d1y - d2y, 2));
}

inline double getAngle(const POINT& p1, const POINT& p2)
{
    const int x = p2.x - p1.x;
    const int y = p2.y - p1.y;
           
    const double radian = atan2((-y), x);
    double degree = Degree * radian; // 라디안 -> 디그리 변환
    degree = radian < 0 ? degree + 360 : degree;
    return degree;
}

inline uint16 filterPreciseAngleToSimple(const double currDegree)
{
    int16 modifiableAngle = -1;
    if (350 <= currDegree || currDegree <= 10)
        modifiableAngle = 0;
    else if (10 <= currDegree && currDegree <= 70)
        modifiableAngle = 45;
    else if (70 <= currDegree && currDegree <= 110)
        modifiableAngle = 90;
    else if (110 <= currDegree && currDegree <= 170)
        modifiableAngle = 135;
    else if (170 <= currDegree && currDegree <= 190)
        modifiableAngle = 180;
    else if (190 <= currDegree && currDegree <= 240)
        modifiableAngle = 225;
    else if (240 <= currDegree && currDegree <= 300)
        modifiableAngle = 270;
    else if (300 <= currDegree && currDegree <= 350)
        modifiableAngle = 315;

    return modifiableAngle;
}

inline void renderRectToCenter(const HDC& hdc, int16 spX, int16 spY, int16 width, int16 height)
{
    const int16 halfW = width / 2;
    const int16 halfY = height / 2;

    const int16 left = spX;
    const int16 top = spY;
    const int16 right = spX + halfW;
    const int16 bottom = spY + halfY;
    Rectangle(hdc, left, top, right, bottom);
}