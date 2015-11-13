
#ifndef __THIEF_BULLET02_H__
#define	__THIEF_BULLET02_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	Thief_Bullet02クラス
//
//*****************************************************************************
class Thief_Bullet02 : public BaseBullet
{
private:
	int time;
	int holdCoinNum;
	float range;	//速度の減衰率

public:
	//	初期化・解放
	Thief_Bullet02(void);
	~Thief_Bullet02(void){};

	bool Initialize();

	void Move();
	void CollectCoin();
	bool HitCheckVsMyPlayer();	//打ち出したプレイヤーとの判定
	void Update();
};

//*****************************************************************************
#endif // !__THIEF_BULLET02_H__
