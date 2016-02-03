
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
	int		quickCount;

public:
	//	�������E���
	Scavenger_CPU(void);
	~Scavenger_CPU(void);
	bool	Initialize(int playerNum, Vector3 pos);

	//	����֐�
	void	ControlAI( void ) override;
	void	AutoPickCoin( void )override;
	void	RunAway( void )override;
	bool	QuickArts( void )override;
};

