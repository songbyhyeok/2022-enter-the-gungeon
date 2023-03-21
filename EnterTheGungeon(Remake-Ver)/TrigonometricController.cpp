#include "TrigonometricController.h"
#include "ScreenManager.h"
#include "MouseHandler.h"
#include "Debugging.h"

void TrigonometricController::init(POINT startPosition, POINT resolution, POINT halfResolution)
{
    _startPosition = move(startPosition);
    _destinationPosition = { _startPosition.x + resolution.x, _startPosition.y + resolution.y };

    _radiusOfCircle = halfResolution;
    _practicalRadius = { _startPosition.x + _radiusOfCircle.x, _startPosition.y + _radiusOfCircle.y };
    _upperLeftDegree = getDegree({ -halfResolution.x, -halfResolution.y });
    _upperRightDegree = getDegree({ halfResolution.x, -halfResolution.y });
    _lowerLeftDegree = getDegree({ -halfResolution.x, halfResolution.y });
    _lowerRightDegree = getDegree({ halfResolution.x, halfResolution.y });
}

void TrigonometricController::drawCircle(const HDC& hdc) const
{
    HPEN hPen, hPenOld;
    hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 125)); // 선 스타일, 굵기, 색상
    hPenOld = (HPEN)::SelectObject(hdc, (HGDIOBJ)hPen); // 펜 선택

    //// X축 그리기
    MoveToEx(hdc, _startPosition.x, _startPosition.y + _radiusOfCircle.y, nullptr);
    LineTo(hdc, _destinationPosition.x, _startPosition.y + _radiusOfCircle.y);

    //// Y축 그리기
    MoveToEx(hdc, _startPosition.x + _radiusOfCircle.x, _startPosition.y, nullptr);
    LineTo(hdc, _startPosition.x + _radiusOfCircle.x, _destinationPosition.y);

    //// 좌측 사선
    MoveToEx(hdc, _startPosition.x, _startPosition.y, nullptr);
    LineTo(hdc, _destinationPosition.x, _destinationPosition.y);

    //// 우측 사선
    MoveToEx(hdc, _destinationPosition.x, _startPosition.y, nullptr);
    LineTo(hdc, _startPosition.x, _destinationPosition.y);

    float radianOfCircle = 0.0f;
    MoveToEx(hdc, _startPosition.x + _radiusOfCircle.x + cos(radianOfCircle) * _radiusOfCircle.x, _startPosition.y + _radiusOfCircle.y + -sin(radianOfCircle) * _radiusOfCircle.y, nullptr);

    // 원을 1도부터 1도씩 증가시킨다.
    // 도가 360도와 같을 때까지 반복하는 이유는 359도의 좌표까지 선을 이으면 359도와 0도(360도) 사이가 이어지지 않아
    // 열린 원이 되기 때문에 359도와 0도(360도)를 이어주기 위해서이다.
    for (int i = 1; i <= 360; i++)
    {
        radianOfCircle = i * Radian;
        // 해당 각의 x, y좌표를 계산하여 선을 그린다.
        LineTo(hdc, _startPosition.x + _radiusOfCircle.x + cos(radianOfCircle) * _radiusOfCircle.x, _startPosition.y + _radiusOfCircle.y + -sin(radianOfCircle) * _radiusOfCircle.y);
    }

    hPen = (HPEN)::SelectObject(hdc, hPenOld); // 기존의 펜 선택
    ::DeleteObject(hPen); // 생성한 펜 삭제

    const uint16 spX = ScreenManager::getInstance().getSubResolution().x - 200;
    Debugging::renderDouble(hdc, "돋움", 20, { spX, 50 }, WhiteRGB, true);
    Debugging::renderString(hdc, "돋움", 20, { spX, 70 }, WhiteRGB, true);
}

int16 TrigonometricController::getCurrentAngle() const
{
    int16 selectedAngle = -1;
    const double locatedDegree = getDegree(getDistance(MouseHandler::Mouse, _practicalRadius));
    Debugging::setPrintValue(locatedDegree);
    if (350 <= locatedDegree || locatedDegree <= 10)
    {
        Debugging::setPrintValue(" 0도 사이");
        selectedAngle = 0;
    }
    else if (10 <= locatedDegree && locatedDegree <= 70)
    {
        Debugging::setPrintValue(" 45도 사이");
        selectedAngle = 45;
    }
    else if (70 <= locatedDegree && locatedDegree <= 110)
    {
        Debugging::setPrintValue(" 90도 사이");
        selectedAngle = 90;
    }
    else if (110 <= locatedDegree && locatedDegree <= 170)
    {
        Debugging::setPrintValue(" 135도 사이");
        selectedAngle = 135;
    }
    else if (170 <= locatedDegree && locatedDegree <= 190)
    {
        Debugging::setPrintValue(" 180도 사이");
        selectedAngle = 180;
    }
    else if (190 <= locatedDegree && locatedDegree <= 240)
    {
        Debugging::setPrintValue(" 225도 사이");
        selectedAngle = 225;
    }
    else if (240 <= locatedDegree && locatedDegree <= 300)
    {
        Debugging::setPrintValue(" 270도 사이");
        selectedAngle = 270;
    }
    else if (300 <= locatedDegree && locatedDegree <= 350)
    {
        Debugging::setPrintValue(" 315도 사이");
        selectedAngle = 315;      
    }

    return selectedAngle;
}

int16 TrigonometricController::getPreciseCurrentAngle() const
{
    return getDegree(getDistance(MouseHandler::Mouse, _practicalRadius));
}

double TrigonometricController::getDegree(const pair<double, double>& pairForDoubleType) const
{
    /*
    * 지정된 x, y 좌표의 아크탄젠트, 즉 역 탄젠트 값을 반환합니다.
    * 아크탄젠트 값은 원점(0, 0)에서 좌표(x_num, y_num)까지의 선과 X축이 이루는 각도입니다.
    * 이 각도는 -pi와 pi 사이의 라디안(-pi 제외)으로 표시됩니다.
    */
    double x = pairForDoubleType.first;
    double y = pairForDoubleType.second;
    const double radian = atan2((-y), x);
    double degree = Degree * radian;
    degree = radian < 0 ? degree + 360 : degree;
    return degree;
}

