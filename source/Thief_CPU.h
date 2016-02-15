
#pragma once

//*********************************************************************************
//
//	Thief_CPUƒNƒ‰ƒX
//
//*********************************************************************************

//	include
#include	"Thief.h"

//	class
class Thief_CPU : public Thief
{
private:

public:
	//	‰Šú‰»E‰ğ•ú
	Thief_CPU(void);
	~Thief_CPU(void);

	//	“®ìŠÖ”
	void	ControlAI(void) override;
	void	AutoPickCoin(void)override;
	void	RunAway(void)override;
	bool	QuickArts(void)override;
};

