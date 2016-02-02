
#ifndef __THIEF_BULLET04_H__
#define	__THIEF_BULLET04_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	Thief_Bullet04クラス(ハイパーアーツの先っぽ)
//
//*****************************************************************************


#include	"Thief_Bullet03.h"

class Thief_Bullet04 : public BaseBullet
{
private:
	int		holdCoinNum;
	float	growSpeed;	//アームの伸び縮みする速さ
	Vector3 startPos;	//発射位置
	bool	checkMax, checkMin;	//アームが最大まで伸びたか、最小まで縮んだか
	bool	isPlayerCheck[4];	//ｎ番目のプレイヤーに触れたかどうか

public:
	//	初期化・解放
	Thief_Bullet04(void);
	~Thief_Bullet04(void){};

	bool Initialize();
	void Update();

	void Move(); 
	bool PlayerCollisionCheck(void);

};

//*****************************************************************************
#endif // !__THIEF_BULLET04_H__
