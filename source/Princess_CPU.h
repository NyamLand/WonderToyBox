
#pragma once

//*********************************************************************************
//
//	Princess_CPUクラス
//
//*********************************************************************************

//	include
#include	"Princess.h"

//	class
class Princess_CPU : public Princess
{
private:

public:
	//	初期化・解放
	Princess_CPU( void );
	~Princess_CPU( void );

	//	動作関数
	void	ControlAI(void) override;
	void	AutoPickCoin(void)override;
	void	RunAway(void)override;
};

