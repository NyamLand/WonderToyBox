#ifndef __PIRATEBULLET02_H__
#define	__PIRATEBULLET02_H__

#include	"BasePirateBullet.h"
//*****************************************************************************
//
//	PirateBullet02�N���X
//
//*****************************************************************************
class PirateBullet02 : public BasePirateBullet
{
private:
public:
	//	�������E���
	PirateBullet02(void);
	~PirateBullet02(void){};

	void Move();
	void Update();
};

//*****************************************************************************
#endif // !__PIRATE_BULLET01_H__
