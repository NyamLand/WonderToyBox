
#pragma once

//*********************************************************************************
//
//	Princess_CPUƒNƒ‰ƒX
//
//*********************************************************************************

//	include
#include	"Princess.h"

//	class
class Princess_CPU : public Princess
{
private:

public:
	//	‰Šú‰»E‰ğ•ú
	Princess_CPU( void );
	~Princess_CPU( void );

	//	“®ìŠÖ”
	bool	CPU_Move( void );
	//void	ControlAI( void )override;
};

