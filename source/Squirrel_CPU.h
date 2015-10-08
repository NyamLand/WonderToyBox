
#pragma once

//*********************************************************************************
//
//	Squirrel_CPUƒNƒ‰ƒX
//
//*********************************************************************************

//	include
#include	"Squirrel.h"

//	class
class Squirrel_CPU : public Squirrel
{
private:

public:
	//	‰Šú‰»E‰ğ•ú
	Squirrel_CPU( void );
	~Squirrel_CPU( void );

	//	“®ìŠÖ”
	bool	CPU_Move( void );
	//void	ControlAI( void )override;
};

