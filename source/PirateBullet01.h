#ifndef __PIRATEBULLET01_H__
#define	__PIRATEBULLET01_H__

#include	"BasePirateBullet.h"
//*****************************************************************************
//
//	PirateBullet01�N���X
//
//*****************************************************************************
class PirateBullet01 : public BasePirateBullet
{
private:
public:
	//	�������E���
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
