
#pragma once

//*********************************************************************************
//
//	Knight_CPUクラス
//
//*********************************************************************************

//	include
#include	"Knight.h"

//	class
class Knight_CPU : public Knight
{
private:

public:
	//	初期化・解放
	Knight_CPU( void );
	~Knight_CPU( void );

	//	動作関数
	bool	CPU_Move( void );
	//void	ControlAI( void )override;
};

