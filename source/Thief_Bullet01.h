
#ifndef __THIEF_BULLET01_H__
#define	__THIEF_BULLET01_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	Thief_Bullet01�N���X
//
//*****************************************************************************
class Thief_Bullet01 : public BaseBullet
{
private:
	int time;
	int step;

public:
	//	�������E���
	Thief_Bullet01(void);
	~Thief_Bullet01(void){};

	bool Initialize();

	void Move();
	void Stalk();
	void Update();
};

//*****************************************************************************
#endif // !__THIEF_BULLET01_H__
