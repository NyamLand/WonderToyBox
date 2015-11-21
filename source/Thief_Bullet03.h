
#ifndef __THIEF_BULLET03_H__
#define	__THIEF_BULLET03_H__

#include	"BaseBullet.h"
#include	"Thief_Bullet04.h"
//*****************************************************************************
//
//	Thief_Bullet03クラス(ハイパーアーツのアーム部分)
//
//*****************************************************************************



class Thief_Bullet03 : public BaseBullet
{
private:
	float	growSpeed;	//アームの伸び縮みする速さ
	bool	checkMax, checkMin;	//アームが最大まで伸びたか、最小まで縮んだか
public:
	//	初期化・解放
	Thief_Bullet03(void);
	~Thief_Bullet03(void){};

	bool Initialize();

	void Move();
	void Update();
};

//*****************************************************************************
#endif // !__THIEF_BULLET03_H__
