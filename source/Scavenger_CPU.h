
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
	int		AdjustingCount;

public:
	//	�������E���
	Scavenger_CPU(void);
	~Scavenger_CPU(void);
	bool	Initialize(int playerNum, Vector3 pos);

	//	����֐�
	
	void	AutoPickCoin();
	//void	AutoAttack( void );
	//bool	HyperArts();
	void	RunAway();
	bool	QuickArts();
	void	ControlAI( void ) override;
};

