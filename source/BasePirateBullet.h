#ifndef __BASEPIRATEBULLET01_H__
#define	__BASEPIRATEBULLET01_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	BasePirateBulletクラス
//
//*****************************************************************************
class BasePirateBullet : public BaseBullet
{
protected:
	int	power;
	bool explosion;
	bool isPlayerCheck[4];	//ｎ番目のプレイヤーに触れたかどうか 触れていない = false;
public:
	//	初期化・解放
	BasePirateBullet(void);
	~BasePirateBullet(void){

	};
	bool Initialize();

	virtual void Move() = 0;
	virtual void Update() = 0;


protected:
	virtual void Explode();
	virtual bool PlayerCollisionCheck(void);
};

//*****************************************************************************
#endif // !__PIRATE_BULLET01_H__
