
#ifndef __THIEF_BULLET02_H__
#define	__THIEF_BULLET02_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	Thief_Bullet02�N���X
//
//*****************************************************************************
class Thief_Bullet02 : public BaseBullet
{
private:
	int time;
	int holdCoinNum;
	float range;	//���x�̌�����

public:
	//	�������E���
	Thief_Bullet02(void);
	~Thief_Bullet02(void){};

	bool Initialize();

	void Move();
	void CollectCoin();
	bool HitCheckVsMyPlayer();	//�ł��o�����v���C���[�Ƃ̔���
	void Update();
};

//*****************************************************************************
#endif // !__THIEF_BULLET02_H__
