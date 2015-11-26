#ifndef __PIRATE_BULLET01_H__
#define	__PIRATE_BULLET01_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	Pirate_Bullet01クラス
//
//*****************************************************************************
class Pirate_Bullet01 : public BaseBullet
{
private:
	 bool explosion = false;
public:
	//	初期化・解放
	Pirate_Bullet01(void);
	~Pirate_Bullet01(void){};
	bool Initialize();

	void Move();
	void Update();

	bool PlayerCollisionCheck(void);
};

//*****************************************************************************
#endif // !__PIRATE_BULLET01_H__
