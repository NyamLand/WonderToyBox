
#ifndef __BULLET_H__
#define	__BULLET_H__

//*****************************************************************************
//
//	Bullet�N���X
//
//*****************************************************************************


class BaseBullet
{
protected:
	int				limitTimer;
protected:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			state;
	bool			activate;
	int				judgeTimer;	//	����L������
	int				number;		//	�ԍ����蓖�ėp
	int				leanpower; //	�����莞�ԗp
	int				playerNum;	//���Ԗڂ̃v���C���[�̒e��
	int time;	//��������Ă���̎���
	int step;

public:
	//	�������E���
	BaseBullet(void);
	~BaseBullet(void);
	virtual bool	Initialize(void) = 0;

	//	�X�V�E�`��
	virtual void	Update(void) = 0;
	void	Render(void);
	void	Render(iexShader* shader, LPSTR technique);

	//	����֐�
	bool	StageCollisionCheck(void);
	bool	PlayerCollisionCheck(void);

	//	���擾�E�ݒ�
	void	SetPos(Vector3 pos);
	void	SetAngle(float angle);
	void	SetScale(float angle);
	Vector3	GetPos(void);
	float		GetAngle(void);

protected:
	friend	class BulletManager;
};

//*****************************************************************************
#endif // !__BULLET_H__
