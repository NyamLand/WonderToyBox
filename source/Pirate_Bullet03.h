#ifndef __PIRATE_BULLET03_H__
#define	__PIRATE_BULLET03_H__

#include	"Pirate_Bullet01.h"
//*****************************************************************************
//
//	Pirate_Bullet03�N���X
//
//*****************************************************************************
class Pirate_Bullet03 : public Pirate_Bullet01
{
private:
public:
	//	�������E���
	Pirate_Bullet03(void);
	~Pirate_Bullet03(void){};

	void Move();
	void Update();
};

//*****************************************************************************
#endif // !__PIRATE_BULLET01_H__
