
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
	bool	CPU_Move(void);
	void	AutoAttack( void ) override;
	//void	ControlAI( void )override;
};

