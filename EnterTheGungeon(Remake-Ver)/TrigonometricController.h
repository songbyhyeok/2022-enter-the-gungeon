#pragma once

#include "pch.h"

constexpr double Radian = PI / 180.0f;

class TrigonometricController
{
public:
    void                        init(POINT startPosition, POINT resolution, POINT halfResolution);
    void                        drawCircle(const HDC& hdc) const;

    int16                       getCurrentAngle() const;
    int16                       getPreciseCurrentAngle() const;

    static double               getDegreeToRadian(int16 x) { return x * Radian; }
    static int16                getRadianToDegree(int16 x) { return static_cast<int16>(x * Degree); }

private:    
    pair<double, double>        getDistance(const POINT& a, const POINT& b) const { return { static_cast<double>(a.x) - b.x, static_cast<double>(a.y) - b.y }; }
    double                      getDegree(const pair<double, double>& pairForDoubleType) const;    

private:
    POINT                       _startPosition;
    POINT                       _destinationPosition;
    POINT                       _radiusOfCircle;
    POINT                       _practicalRadius;
    double                      _upperLeftDegree;
    double                      _upperRightDegree;
    double                      _lowerLeftDegree;
    double                      _lowerRightDegree;

};

