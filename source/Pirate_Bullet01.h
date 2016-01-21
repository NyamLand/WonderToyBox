#ifndef __PIRATE_BULLET01_H__
#define	__PIRATE_BULLET01_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	Pirate_Bullet01�N���X
//
//*****************************************************************************
class Pirate_Bullet01 : public BaseBullet
{
protected:
	int	power;
	bool explosion;
	bool isPlayerCheck[4];	//���Ԗڂ̃v���C���[�ɐG�ꂽ���ǂ��� = false;
public:
	//	�������E���
	Pirate_Bullet01(void);
	~Pirate_Bullet01(void){};
	bool Initialize();

	void Move();
	void Update();

	void Explode();

protected:
	virtual bool PlayerCollisionCheck(void);
};

//*****************************************************************************
#endif // !__PIRATE_BULLET01_H__
