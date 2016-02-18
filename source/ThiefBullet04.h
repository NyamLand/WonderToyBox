
#ifndef __THIEFBULLET04_H__
#define	__THIEFBULLET04_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	ThiefBullet04クラス(ハイパーアーツの先っぽ)
//
//*****************************************************************************


#include	"ThiefBullet03.h"

class ThiefBullet04 : public BaseBullet
{
private:
	int		holdCoinNum;
	float	growSpeed;	//アームの伸び縮みする速さ
	Vector3 startPos;	//発射位置
	bool	checkMax, checkMin;	//アームが最大まで伸びたか、最小まで縮んだか
	bool	isPlayerCheck[4];	//ｎ番目のプレイヤーに触れたかどうか

public:
	//	初期化・解放
	ThiefBullet04(void);
	~ThiefBullet04(void){};

	bool Initialize();
	void Update();

	void Move(); 
	bool PlayerCollisionCheck(void);

};

//*****************************************************************************
#endif // !__THIEF_BULLET04_H__
