
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

public:
	//	‰Šú‰»E‰ğ•ú
	Scavenger_CPU(void);
	~Scavenger_CPU(void);
	bool	Initialize(int playerNum, Vector3 pos);

	//	“®ìŠÖ”
	void	ControlAI( void ) override;
	void	AutoPickCoin( void )override;
	void	RunAway( void )override;
	bool	QuickArts( void )override;
};

