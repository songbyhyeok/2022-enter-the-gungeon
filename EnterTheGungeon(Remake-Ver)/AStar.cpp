#include "AStar.h"
#include "MapTool.h"

void AStar::init()
{	
	const auto& tileSize{ (*_mapToolPtr)->getTileSize() };
	_pathColor = ImageManager::makeImage("Images/Scene/Maptool/Palette/InstallationColor.bmp", {}, { 1.0f, 1.0f }, { tileSize.x, tileSize.y * InstallationColorCount }, { 1, InstallationColorCount }, false);
	_pathColor.setTransparency(80);

	_directions =
	{
		POINT_FOR_ASTAR{ 0, -1 },		// UP
		POINT_FOR_ASTAR{ -1, 0 },		// LEFT
		POINT_FOR_ASTAR{ 0, 1 },		// DOWN
		POINT_FOR_ASTAR{ 1, 0 },		// RIGHT
		POINT_FOR_ASTAR{ -1, -1 },		// UP_LEFT
		POINT_FOR_ASTAR{ -1, 1 },		// DOWN_LEFT
		POINT_FOR_ASTAR{ 1, 1 },		// DOWN_RIGHT
		POINT_FOR_ASTAR{ 1, -1 }		// UP_RIGHT
	};

	_directionCosts =
	{
		10, // UP
		10, // LEFT
		10, // DOWN
		10, // RIGHT
		14,
		14,
		14,
		14
	};
}

void AStar::renderPath(const HDC& hdc, const vector<POINT>& pathVec) const
{	
	for (const auto& pathIdx : pathVec)
	{		
		_pathColor.renderTransparentFrame(hdc, (*_mapToolPtr)->convertIndexToCalcPos(pathIdx), {});
	}
}

	// 점수 매기기
	// F = G + H
	// F = 최종 점수 (작을 수록 좋음, 경로에 따라 달라짐)
	// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용 (작을 수록 좋음, 경로에 따라 달라짐)
	// H = 목적지에서 얼마나 가까운지 (작을 수록 좋음, 고정)
vector<POINT> AStar::findPath(const POINT& sIdx, const POINT& dIdx) const
{
	const POINT& startTile{ (*_mapToolPtr)->getStartTile() };
	POINT		 calcSIdx{ sIdx.x - startTile.x, sIdx.y - startTile.y };

	POINT addition{};
	if (calcSIdx.x < 0)
	{
		addition.x = -calcSIdx.x;
		calcSIdx.x = calcSIdx.x + addition.x;
	}

	if (calcSIdx.y < 0)
	{
		addition.y = -calcSIdx.y;
		calcSIdx.y = calcSIdx.y + addition.y;
	}

	POINT calcDIdx{ (dIdx.x - startTile.x) + addition.x, (dIdx.y - startTile.y) + addition.y };
	
	// ClosedList
	// close[y][x] -> (y, x)에 방문을 했는지 여부
	vector<vector<bool>> closed(100, vector<bool>(100, false));

	// best[y][x] -> 지금까지 (y, x)에 대한 가장 좋은 비용 (작을 수록 좋음)
	vector<vector<int32>> best(100, vector<int32>(100, INT32_MAX));

	// 부모 추적 용도
	map<POINT_FOR_ASTAR, POINT_FOR_ASTAR> parent;

	// OpenList
	priority_queue<PQ_NODE, vector<PQ_NODE>, greater<PQ_NODE>> pq;

	// 1) 예약(발견) 시스템 구현
	// - 이미 더 좋은 경로를 찾았다면 스킵
	// 2) 뒤늦게 더 좋은 경로가 발견될 수 있음 -> 예외 처리 필수
	// - openList에서 찾아서 제거한다거나.
	// - pq에서 pop한 다음에 무시한다거나.

	// 초기값
	{
		const int32 g = 0;
		const int32 h = 10 * (abs(dIdx.y - sIdx.y) + abs(dIdx.x - sIdx.x));
		const int32 f = g + h;
		pq.emplace(move(PQ_NODE{ {calcSIdx.x, calcSIdx.y}, g, f }));
		best[calcSIdx.y][calcSIdx.x] = f;
		parent[calcSIdx] = { calcSIdx.x, calcSIdx.y };
	}

	while (pq.empty() == false)
	{
		// 제일 좋은 후보를 찾는다
		PQ_NODE calcNode = pq.top();
		pq.pop();

		// 동일한 좌표를 여러 경로로 찾아서
		// 더 빠른 경로로 인해서 이미 방문(closed)된 경우 스킵
		// [선택]
		if (closed[calcNode.calcIdx.y][calcNode.calcIdx.x])
			continue;
		if (best[calcNode.calcIdx.y][calcNode.calcIdx.x] < calcNode.f)
			continue;

		// 방문
		closed[calcNode.calcIdx.y][calcNode.calcIdx.x] = true;

		// 목적지에 도착했으면 바로 종료
		if (POINT_FOR_ASTAR{ (calcNode.calcIdx.x - addition.x) + startTile.x, (calcNode.calcIdx.y - addition.y) + startTile.y } == dIdx)
			break;

		for (int dir = 0; dir < DirectionCount; dir++)
		{
			const POINT_FOR_ASTAR calcNIdx = { calcNode.calcIdx + _directions[dir] };
			const POINT_FOR_ASTAR nIdx = { (calcNIdx.x - addition.x) + startTile.x, (calcNIdx.y - addition.y) + startTile.y };
			if (calcNIdx.x < 0 || calcNIdx.y < 0)
				continue;

			// 갈 수 있는 지역은 맞는지 확인
			if ((*_mapToolPtr)->canGo( move(POINT{ nIdx.x, nIdx.y })) == false)
				continue;
			// [선택] 이미 방문한 곳이면 스킵
			if (closed[calcNIdx.y][calcNIdx.x])
				continue;

			// 비용 계산
			const int32 g = calcNode.g + _directionCosts[dir];
			const int32 h = 10 * (abs(dIdx.y - nIdx.y) + abs(dIdx.x - nIdx.x));
			const int32 f = g + h;

			// 다른 경로에서 더 빠른 길을 찾았으면 스킵
			if (best[calcNIdx.y][calcNIdx.x] <= f)
				continue;

			// 예약 진행
			best[calcNIdx.y][calcNIdx.x] = f;

			pq.emplace( move(PQ_NODE{ calcNIdx, g, f }));
			parent[calcNIdx] = calcNode.calcIdx;
		}
	}

	// 거꾸로 거슬러 올라간다
	vector<POINT> path;
	POINT_FOR_ASTAR revCalcIdx = calcDIdx;
	while (true)
	{		
		path.emplace_back(move(POINT{ (revCalcIdx.x - addition.x) + startTile.x, (revCalcIdx.y - addition.y) + startTile.y }));

		// 시작점은 자신이 곧 부모이다
		if (revCalcIdx == parent[revCalcIdx])
			break;

		revCalcIdx = parent[revCalcIdx];
	}
	//std::reverse(path.begin(), path.end());
	return path;
}