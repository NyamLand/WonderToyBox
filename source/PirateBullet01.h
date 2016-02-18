#ifndef __PIRATEBULLET01_H__
#define	__PIRATEBULLET01_H__

#include	"BasePirateBullet.h"
//*****************************************************************************
//
//	PirateBullet01ÉNÉâÉX
//
//*****************************************************************************
class PirateBullet01 : public BasePirateBullet
{
private:
public:
	//	èâä˙âªÅEâï˙
	PirateBullet01(void);
	~PirateBullet01(void){
	
	};
	bool Initialize();

	void Move();
	void Update();

	//virtual void Explode();

protected:
	//virtual bool PlayerCollisionCheck(void);
};

//*****************************************************************************
#endif // !__PIRATE_BULLET01_H__
