
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
	void	ControlAI( void ) override;
	void	AutoPickCoin( void ) override;
	void	RunAway( void ) override;
	bool	PowerArts( void ) override;
};

