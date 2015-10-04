
#pragma once

//*********************************************************************************
//
//	Tiger_CPUクラス
//
//*********************************************************************************

//	include
#include	"Tiger.h"

//	class
class Tiger_CPU : public Tiger
{
private:

public:
	//	初期化・解放
	Tiger_CPU( void );
	~Tiger_CPU( void );

	//	動作関数
	bool	CPU_Move( void );
	void	ControlAI( void )override;
};

