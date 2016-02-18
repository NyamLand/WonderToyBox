
#ifndef __THIEFBULLET01_H__
#define	__THIEFBULLET01_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	ThiefBullet01�N���X
//
//*****************************************************************************

namespace STEP
{
	enum
	{
		CREATED = 0,	//�g�����v�o��
		WAIT,			//�ҋ@
		TARGETING,		//�Ə������킹��
		FIRING,			//����
		STALK			//�ǐ�
	};
}

class ThiefBullet01 : public BaseBullet
{
private:
	float ToPlayerLength;
	Vector3 ToPlayerVec;
	float frontDot, backDot, upDot, downDot;
	Vector3 cross;
	Vector3 front;
	Vector3 up;

public:
	//	�������E���
	ThiefBullet01(void);
	~ThiefBullet01(void){};

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
