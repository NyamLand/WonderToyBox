
#pragma once

//*********************************************************************************
//
//	Knight_CPU�N���X
//
//*********************************************************************************

//	include
#include	"Knight.h"

//	class
class Knight_CPU : public Knight
{
private:

public:
	//	�������E���
	Knight_CPU( void );
	~Knight_CPU( void );

	//	����֐�
	bool	CPU_Move( void );
	//void	ControlAI( void )override;
};

