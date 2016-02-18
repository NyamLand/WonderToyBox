
#ifndef __THIEFBULLET02_H__
#define	__THIEFBULLET02_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	ThiefBullet02クラス
//
//*****************************************************************************



class ThiefBullet02 : public BaseBullet
{
private:
	int holdCoinNum;
	float range;	//速度の減衰率

public:
	//	初期化・解放
	ThiefBullet02(void);
	~ThiefBullet02(void){};

	bool Initialize();

	void Move();
	void CollectCoin();
	bool HitCheckVsMyPlayer();	//打ち出したプレイヤーとの判定
	void Update();
};

//*****************************************************************************
#endif // !__THIEF_BULLET02_H__
