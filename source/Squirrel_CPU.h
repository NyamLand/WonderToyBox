
#pragma once

//*********************************************************************************
//
//	Squirrel_CPU�N���X
//
//*********************************************************************************

//	include
#include	"Squirrel.h"

//	class
class Squirrel_CPU : public Squirrel
{
private:

public:
	//	�������E���
	Squirrel_CPU( void );
	~Squirrel_CPU( void );

	//	����֐�
	bool	CPU_Move( void );
	//void	ControlAI( void )override;
};

