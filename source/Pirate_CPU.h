
#pragma once

//*********************************************************************************
//
//	Pirate_CPU�N���X
//
//*********************************************************************************

//	include
#include	"Pirate.h"

//	class
class Pirate_CPU : public Pirate
{
private:

public:
	//	�������E���
	Pirate_CPU(void);
	~Pirate_CPU(void);

	//	����֐�
	bool	CPU_Move(void);
	//void	ControlAI( void )override;
};

