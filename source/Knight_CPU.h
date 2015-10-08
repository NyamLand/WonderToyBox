
#pragma once

//*********************************************************************************
//
//	Knight_CPUƒNƒ‰ƒX
//
//*********************************************************************************

//	include
#include	"Knight.h"

//	class
class Knight_CPU : public Knight
{
private:

public:
	//	‰Šú‰»E‰ğ•ú
	Knight_CPU( void );
	~Knight_CPU( void );

	//	“®ìŠÖ”
	bool	CPU_Move( void );
	//void	ControlAI( void )override;
};

