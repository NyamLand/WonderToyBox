
#ifndef __THIEF_BULLET04_H__
#define	__THIEF_BULLET04_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	Thief_Bullet04�N���X(�n�C�p�[�A�[�c�̐����)
//
//*****************************************************************************


#include	"Thief_Bullet03.h"

class Thief_Bullet04 : public BaseBullet
{
private:
	float	growSpeed;	//�A�[���̐L�яk�݂��鑬��
	bool	checkMax, checkMin;	//�A�[�����ő�܂ŐL�т����A�ŏ��܂ŏk�񂾂�

public:
	//	�������E���
	Thief_Bullet04(void);
	~Thief_Bullet04(void){};

	bool Initialize();
	void Update();

	void Move(); 
	bool PlayerCollisionCheck(void);

};

//*****************************************************************************
#endif // !__THIEF_BULLET04_H__
