
#pragma once

//*********************************************************************************
//
//	Pirate_CPUクラス
//
//*********************************************************************************

//	include
#include	"Pirate.h"

//	class
class Pirate_CPU : public Pirate
{
private:

public:
	//	初期化・解放
	Pirate_CPU(void);
	~Pirate_CPU(void);

	//	動作関数
	void	ControlAI( void ) override;
	void	AutoPickCoin( void ) override;
	void	RunAway( void ) override;
	bool	PowerArts( void ) override;
};

