
#pragma once

//*********************************************************************************
//
//	Princess_CPU�N���X
//
//*********************************************************************************

//	include
#include	"Princess.h"

//	class
class Princess_CPU : public Princess
{
private:

public:
	//	�������E���
	Princess_CPU( void );
	~Princess_CPU( void );

	//	����֐�
	bool	CPU_Move( void );
	//void	ControlAI( void )override;
};

