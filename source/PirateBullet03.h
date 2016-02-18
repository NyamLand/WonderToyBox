#ifndef __PIRATEBULLET03_H__
#define	__PIRATEBULLET03_H__

#include	"BasePirateBullet.h"
//*****************************************************************************
//
//	PirateBullet03ÉNÉâÉX
//
//*****************************************************************************
class PirateBullet03 : public BasePirateBullet
{
private:
public:
	//	èâä˙âªÅEâï˙
	PirateBullet03(void);
	~PirateBullet03(void){};

	void Move();
	void Update();

	void PirateBullet03::Explode(void);

	bool PlayerCollisionCheck(void);
};

//*****************************************************************************
#endif // !__PIRATE_BULLET01_H__
