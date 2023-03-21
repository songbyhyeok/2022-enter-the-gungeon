#pragma once

#include "Monster.h"

class EnemyGigi : public Monster
{
public:
	void			init(FPOINT scale, const POINT& sp, POINT size, POINT currPos, POINT currIdx);
	void			update() override;
	void			render(const HDC& hdc) const override;
	void			release() override;

private:
	void			changeIdleMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX) override;
	void			changeWalkingMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX) override;
	void			changeDeathMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX) override;
	void			changeAttackMotion(const MONSTER_STANDING_POSITION_STATE& selectedStandingState, bool isReverseX) override;
};