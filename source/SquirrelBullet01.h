
#ifndef __SQUIRRELBULLET01_H__
#define	__SQUIRRELBULLET01_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	SquirrelBullet01クラス
//
//*****************************************************************************
class SquirrelBullet01 : public BaseBullet
{

public:
	//	初期化・解放
	SquirrelBullet01(void);
	~SquirrelBullet01(void){};
	bool Initialize();

	void Move();
	void Update();
};

//*****************************************************************************
#endif // !__BULLET01_H__
