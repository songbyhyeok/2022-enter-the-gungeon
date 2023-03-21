#pragma once

#include "ImageManager.h"

struct POINT_FOR_ASTAR
{
public:
	POINT_FOR_ASTAR() = default;
	POINT_FOR_ASTAR(const int a_x, const int a_y) : x(a_x), y(a_y) {}
	POINT_FOR_ASTAR(const POINT & point) : x(point.x), y(point.y) {}

	bool operator==(const POINT_FOR_ASTAR& other)
	{
		return y == other.y && x == other.x;
	}

	bool operator!=(const POINT_FOR_ASTAR& other)
	{
		return !(*this == other);
	}

	bool operator<(const POINT_FOR_ASTAR& other) const
	{
		if (y != other.y)
			return y < other.y;
		return x < other.x;
	}

	POINT_FOR_ASTAR operator+(const POINT_FOR_ASTAR& other)
	{
		POINT_FOR_ASTAR ret;
		ret.y = y + other.y;
		ret.x = x + other.x;
		return ret;
	}

	POINT_FOR_ASTAR& operator+=(const POINT_FOR_ASTAR& other)
	{
		y += other.y;
		x += other.x;
		return *this;
	}

	int x;
	int y;
};

struct PQ_NODE
{
public:
	bool operator<(const PQ_NODE& other) const { return f < other.f; }
	bool operator>(const PQ_NODE& other) const { return f > other.f; }

	POINT_FOR_ASTAR		calcIdx;
	LONG				g;
	LONG				f; 
};

constexpr uint16 DirectionCount = 8;

class MapTool;
class AStar
{
public:
	void									init();

	void									setMapToolPtr(MapTool* mapToolPtr) { _mapToolPtr = make_unique<MapTool*>(mapToolPtr); }

	void									renderPath(const HDC& hdc, const vector<POINT>& pathVec) const;
	vector<POINT> 							findPath(const POINT& sIdx, const POINT& eIdx) const;


private:
	Image									_pathColor;
	array<POINT_FOR_ASTAR, DirectionCount>	_directions;
	array<uint16, DirectionCount>			_directionCosts;
	unique_ptr<MapTool*>					_mapToolPtr;
};

