
#ifndef __THIEFBULLET03_H__
#define	__THIEFBULLET03_H__

#include	"BaseBullet.h"
#include	"ThiefBullet04.h"
//*****************************************************************************
//
//	ThiefBullet03�N���X(�n�C�p�[�A�[�c�̃A�[������)
//
//*****************************************************************************



class ThiefBullet03 : public BaseBullet
{
private:
	float	growSpeed;	//�A�[���̐L�яk�݂��鑬��
	bool	checkMax, checkMin;	//�A�[�����ő�܂ŐL�т����A�ŏ��܂ŏk�񂾂�
public:
	//	�������E���
	ThiefBullet03(void);
	~ThiefBullet03(void){};

	bool Initialize();

	void ControlScale();
	void Update();
};

//*****************************************************************************
#endif // !__THIEF_BULLET03_H__
