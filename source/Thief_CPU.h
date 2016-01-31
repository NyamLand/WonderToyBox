
#pragma once

//*********************************************************************************
//
//	Thief_CPUƒNƒ‰ƒX
//
//*********************************************************************************

//	include
#include	"Thief.h"

//	class
class Thief_CPU : public Thief
{
private:

public:
	//	‰Šú‰»E‰ğ•ú
	Thief_CPU(void);
	~Thief_CPU(void);

	//	“®ìŠÖ”
	bool	CPU_Move(void);
	//void	AutoAttack( void ) override;
	//void	ControlAI( void )override;
};

