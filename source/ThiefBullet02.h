
#ifndef __THIEFBULLET02_H__
#define	__THIEFBULLET02_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	ThiefBullet02�N���X
//
//*****************************************************************************



class ThiefBullet02 : public BaseBullet
{
private:
	int holdCoinNum;
	float range;	//���x�̌�����

public:
	//	�������E���
	ThiefBullet02(void);
	~ThiefBullet02(void){};

	bool Initialize();

	void Move();
	void CollectCoin();
	bool HitCheckVsMyPlayer();	//�ł��o�����v���C���[�Ƃ̔���
	void Update();
};

//*****************************************************************************
#endif // !__THIEF_BULLET02_H__
