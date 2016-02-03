
#pragma once

//*********************************************************************************
//
//	Pirate_CPUƒNƒ‰ƒX
//
//*********************************************************************************

//	include
#include	"Pirate.h"

//	class
class Pirate_CPU : public Pirate
{
private:

public:
	//	‰Šú‰»E‰ğ•ú
	Pirate_CPU(void);
	~Pirate_CPU(void);

	//	“®ìŠÖ”
	void	ControlAI( void ) override;
	void	AutoPickCoin( void ) override;
	void	RunAway( void ) override;
	bool	PowerArts( void ) override;
};

