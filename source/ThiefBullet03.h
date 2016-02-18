
#ifndef __THIEFBULLET03_H__
#define	__THIEFBULLET03_H__

#include	"BaseBullet.h"
#include	"ThiefBullet04.h"
//*****************************************************************************
//
//	ThiefBullet03クラス(ハイパーアーツのアーム部分)
//
//*****************************************************************************



class ThiefBullet03 : public BaseBullet
{
private:
	float	growSpeed;	//アームの伸び縮みする速さ
	bool	checkMax, checkMin;	//アームが最大まで伸びたか、最小まで縮んだか
public:
	//	初期化・解放
	ThiefBullet03(void);
	~ThiefBullet03(void){};

	bool Initialize();

	void ControlScale();
	void Update();
};

//*****************************************************************************
#endif // !__THIEF_BULLET03_H__
