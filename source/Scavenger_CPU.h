
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

public:
	//	‰Šú‰»E‰ğ•ú
	Scavenger_CPU(void);
	~Scavenger_CPU(void);

	//	“®ìŠÖ”
	bool	CPU_Move(void);
	void	AutoAttack( void ) override;
	//void	ControlAI( void )override;
};

