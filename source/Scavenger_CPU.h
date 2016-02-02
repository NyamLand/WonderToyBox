
#pragma once

//*********************************************************************************
//
//	Scavenger_CPUクラス
//
//*********************************************************************************

//	include
#include	"Scavenger.h"

//	class
class Scavenger_CPU : public Scavenger
{
private:

public:
	//	初期化・解放
	Scavenger_CPU(void);
	~Scavenger_CPU(void);

	//	動作関数
	bool	CPU_Move(void);
	//void	AutoAttack( void );
	void	ControlAI( void ) override;
};

