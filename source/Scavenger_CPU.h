
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
	int		quickCount;
	int		AdjustingCount;

public:
	//	初期化・解放
	Scavenger_CPU(void);
	~Scavenger_CPU(void);
	bool	Initialize(int playerNum, Vector3 pos);

	//	動作関数
	
	void	AutoPickCoin();
	//void	AutoAttack( void );
	//bool	HyperArts();
	void	RunAway();
	bool	QuickArts();
	void	ControlAI( void ) override;
};

