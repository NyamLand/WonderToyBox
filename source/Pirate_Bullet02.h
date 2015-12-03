#ifndef __PIRATE_BULLET02_H__
#define	__PIRATE_BULLET02_H__

#include	"Pirate_Bullet01.h"
//*****************************************************************************
//
//	Pirate_Bullet01ÉNÉâÉX
//
//*****************************************************************************
class Pirate_Bullet02 : public Pirate_Bullet01
{
private:
public:
	//	èâä˙âªÅEâï˙
	Pirate_Bullet02(void);
	~Pirate_Bullet02(void){};

	void Move();
	void Update();
};

//*****************************************************************************
#endif // !__PIRATE_BULLET01_H__
