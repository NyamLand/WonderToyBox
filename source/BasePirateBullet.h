#ifndef __BASEPIRATEBULLET01_H__
#define	__BASEPIRATEBULLET01_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	BasePirateBullet�N���X
//
//*****************************************************************************
class BasePirateBullet : public BaseBullet
{
protected:
	int	power;
	bool explosion;
	bool isPlayerCheck[4];	//���Ԗڂ̃v���C���[�ɐG�ꂽ���ǂ��� �G��Ă��Ȃ� = false;
public:
	//	�������E���
	BasePirateBullet(void);
	~BasePirateBullet(void){

	};
	bool Initialize();

	virtual void Move() = 0;
	virtual void Update() = 0;


protected:
	virtual void Explode();
	virtual bool PlayerCollisionCheck(void);
};

//*****************************************************************************
#endif // !__PIRATE_BULLET01_H__
