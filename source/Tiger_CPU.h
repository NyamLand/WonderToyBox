
#pragma once

//*********************************************************************************
//
//	Tiger_CPU�N���X
//
//*********************************************************************************

//	include
#include	"Tiger.h"

//	class
class Tiger_CPU : public Tiger
{
private:

public:
	//	�������E���
	Tiger_CPU( void );
	~Tiger_CPU( void );

	//	����֐�
	bool	CPU_Move( void );
	void	ControlAI( void )override;
};

