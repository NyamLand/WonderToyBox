
#pragma once

//*********************************************************************************
//
//	Squirrel_CPUクラス
//
//*********************************************************************************

//	include
#include	"Squirrel.h"

//	class
class Squirrel_CPU : public Squirrel
{
private:

public:
	//	初期化・解放
	Squirrel_CPU( void );
	~Squirrel_CPU( void );

	//	動作関数
	bool	CPU_Move( void );
	//void	ControlAI( void )override;
};

