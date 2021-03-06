
#pragma once

//*********************************************************************************
//
//	Thief_CPUクラス
//
//*********************************************************************************

//	include
#include	"Thief.h"

//	class
class Thief_CPU : public Thief
{
private:

public:
	//	初期化・解放
	Thief_CPU(void);
	~Thief_CPU(void);

	//	動作関数
	void	ControlAI(void) override;
	void	AutoPickCoin(void)override;
	void	RunAway(void)override;
	bool	QuickArts(void)override;
};

