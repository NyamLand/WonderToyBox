
#ifndef __BULLET01_H__
#define	__BULLET01_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	Squirrel_Bullet01クラス
//
//*****************************************************************************
class Squirrel_Bullet01 : public BaseBullet
{

public:
	//	初期化・解放
	Squirrel_Bullet01(void);
	~Squirrel_Bullet01(void){};
	bool Initialize();

	void Move();
	void Update();
};

//*****************************************************************************
#endif // !__BULLET01_H__
