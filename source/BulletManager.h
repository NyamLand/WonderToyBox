
#ifndef		__BULLET_MANAGER_H__
#define		__BULLET_MANAGER_H__

#include	"GameManager.h"
#include	"BaseBullet.h"
#include	"Squirrel_Bullet01.h"
#include	"Thief_Bullet01.h"
#include	"Thief_Bullet02.h"
#include	"Thief_Bullet03.h"
#include	"Thief_Bullet04.h"
#include	"Pirate_Bullet01.h"
#include	"Pirate_Bullet02.h"
//******************************************************************************
//
//	BulletManager�N���X
//
//******************************************************************************
namespace BULLET_TYPE
{
	enum
	{
		SQUIRREL_01 = 0,
		THIEF_01,
		THIEF_02,
		THIEF_03,
		THIEF_04,
		PIRATE_01,
		PIRATE_02,
	};
}

namespace
{
	//------------------------------------------------
	//����L�����Ԃ���Ŏ��Ԃ����Ɍ��܂��Ă��Ȃ��ꍇ��
	//�Ƃ肠����100�b�ɂ��Ă����܂�
	//-------------------------------------------------

	////����L������
	//int BULLET_JUDGETIMER[] =
	//{
	//	100 * SECOND,		//���X1
	//	100 * SECOND,		//����1
	//	100 * SECOND,		//����2
	//	100 * SECOND,		//����3�̃A�[��
	//	100 * SECOND,		//����3�̎蕔��
	//	100 * SECOND			//�C��1
	//};

	//���ł���܂ł̎���
	int BULLET_LIMITTIMER[] =
	{
		100 * SECOND,		//���X1
		3  * SECOND,		//����1
		100 * SECOND,		//����2
		100 * SECOND,		//����3�̃A�[��
		100 * SECOND,		//����3�̎蕔��
		3  * SECOND,		//�C��1
		100 * SECOND,		//�C��2
	};

	//�����蔻�肪�~��łȂ����̂Ɋւ��Ă�NULL
	const float BULLET_RADIUS[] =
	{
		10.0f,		//���X1
		10.0f,		//����1
		10.0f,		//����2
		NULL,		//����3�̃A�[��
		100.0f,		//����3�̎蕔��
		0.5f,		//�C��1
		0.5f		//�C��2
	};
}

class BulletManager
{
public:
	//	�萔
	static const int BULLET_MAX = 201;		//	�e�ő吔
	static const int BULLET_KIND_MAX = 10;
private:


	//	�I�u�W�F�N�g
	iexMesh*	org[BULLET_KIND_MAX];			//	�I���W�i�����f��
	BaseBullet*	c_Bullet[BULLET_MAX];

	//	�ϐ�
	int				bullet_num;	//	�e��
	int				limitTimer;

public:
	//	�������E���
	BulletManager(void);
	~BulletManager(void);
	bool	Initialize(void);

	//	�X�V�E�`��
	void	Update(void);
	void	Render(void);
	void	Render(iexShader* shader, LPSTR technique);

	//	����֐�
	void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, float speed, int playerNum);
	void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, const Vector3 angle, float speed, int playerNum);
	//void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, const float& scale, float speed);
	//void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, float speed, int leanpower);
	void	DistCheck(int n);
};

extern	BulletManager*	m_BulletManager;

//******************************************************************************
#endif // !__BULLET_MANAGER_H__
