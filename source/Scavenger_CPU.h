
#pragma once

//*********************************************************************************
//
//	Scavenger_CPU�N���X
//
//*********************************************************************************

//	include
#include	"Scavenger.h"

//	class
class Scavenger_CPU : public Scavenger
{
private:

public:
	//	�������E���
	Scavenger_CPU(void);
	~Scavenger_CPU(void);

	//	����֐�
	bool	CPU_Move(void);
	void	AutoAttack( void ) override;
	//void	ControlAI( void )override;
};

