
#ifndef __THIEF_BULLET01_H__
#define	__THIEF_BULLET01_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	Thief_Bullet01クラス
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

class Thief_Bullet01 : public BaseBullet
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
	Thief_Bullet01(void);
	~Thief_Bullet01(void){};

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
