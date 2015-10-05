
#pragma once

//*********************************************************************************
//
//	Tiger_CPUƒNƒ‰ƒX
//
//*********************************************************************************

//	include
#include	"Tiger.h"

//	class
class Tiger_CPU : public Tiger
{
private:

public:
	//	‰Šú‰»E‰ğ•ú
	Tiger_CPU( void );
	~Tiger_CPU( void );

	//	“®ìŠÖ”
	bool	CPU_Move( void );
	void	ControlAI( void )override;
};

