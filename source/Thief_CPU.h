
#pragma once

//*********************************************************************************
//
//	Thief_CPU�N���X
//
//*********************************************************************************

//	include
#include	"Thief.h"

//	class
class Thief_CPU : public Thief
{
private:

public:
	//	�������E���
	Thief_CPU(void);
	~Thief_CPU(void);

	//	����֐�
	bool	CPU_Move(void);
	//void	AutoAttack( void ) override;
	//void	ControlAI( void )override;
};

