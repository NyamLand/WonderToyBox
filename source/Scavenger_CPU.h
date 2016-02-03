
#pragma once

//*********************************************************************************
//
//	Scavenger_CPUƒNƒ‰ƒX
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
	//	‰Šú‰»E‰ğ•ú
	Scavenger_CPU(void);
	~Scavenger_CPU(void);
	bool	Initialize(int playerNum, Vector3 pos);

	//	“®ìŠÖ”
	
	void	AutoPickCoin();
	//void	AutoAttack( void );
	//bool	HyperArts();
	void	RunAway();
	bool	QuickArts();
	void	ControlAI( void ) override;
};

