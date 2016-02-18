
#ifndef __THIEFBULLET01_H__
#define	__THIEFBULLET01_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	ThiefBullet01クラス
//
//*****************************************************************************

namespace STEP
{
	enum
	{
		CREATED = 0,	//トランプ出現
		WAIT,			//待機
		TARGETING,		//照準を合わせる
		FIRING,			//発射
		STALK			//追跡
	};
}

class ThiefBullet01 : public BaseBullet
{
private:
	float ToPlayerLength;
	Vector3 ToPlayerVec;
	float frontDot, backDot, upDot, downDot;
	Vector3 cross;
	Vector3 front;
	Vector3 up;

public:
	//	初期化・解放
	ThiefBullet01(void);
	~ThiefBullet01(void){};

	bool Initialize();
	void Update();



	bool	PlayerCollisionCheck(void);
	void Wait();
	void Targeting();
	void Firing();
	void Move();
	void Stalk();
};

//*****************************************************************************
#endif // !__THIEF_BULLET01_H__
