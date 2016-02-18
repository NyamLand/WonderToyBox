
#ifndef __THIEFBULLET04_H__
#define	__THIEFBULLET04_H__

#include	"BaseBullet.h"
//*****************************************************************************
//
//	ThiefBullet04�N���X(�n�C�p�[�A�[�c�̐����)
//
//*****************************************************************************


#include	"ThiefBullet03.h"

class ThiefBullet04 : public BaseBullet
{
private:
	int		holdCoinNum;
	float	growSpeed;	//�A�[���̐L�яk�݂��鑬��
	Vector3 startPos;	//���ˈʒu
	bool	checkMax, checkMin;	//�A�[�����ő�܂ŐL�т����A�ŏ��܂ŏk�񂾂�
	bool	isPlayerCheck[4];	//���Ԗڂ̃v���C���[�ɐG�ꂽ���ǂ���

public:
	//	�������E���
	ThiefBullet04(void);
	~ThiefBullet04(void){};

	bool Initialize();
	void Update();

	void Move(); 
	bool PlayerCollisionCheck(void);

};

//*****************************************************************************
#endif // !__THIEF_BULLET04_H__
