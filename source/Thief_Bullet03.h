
#ifndef __THIEF_BULLET03_H__
#define	__THIEF_BULLET03_H__

#include	"BaseBullet.h"
#include	"Thief_Bullet04.h"
//*****************************************************************************
//
//	Thief_Bullet03�N���X(�n�C�p�[�A�[�c�̃A�[������)
//
//*****************************************************************************



class Thief_Bullet03 : public BaseBullet
{
private:
	float	growSpeed;	//�A�[���̐L�яk�݂��鑬��
	bool	checkMax, checkMin;	//�A�[�����ő�܂ŐL�т����A�ŏ��܂ŏk�񂾂�
public:
	//	�������E���
	Thief_Bullet03(void);
	~Thief_Bullet03(void){};

	bool Initialize();

	void Move();
	void Update();
};

//*****************************************************************************
#endif // !__THIEF_BULLET03_H__
