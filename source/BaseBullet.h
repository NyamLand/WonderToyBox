
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
	Vector3			scale;
	Vector3			angle;
	bool			state;		//���݂��邩�ǂ���
	bool			activate;	//�����蔻��̗L��
	float			radius;		//���̂̔��a(�����蔻��Ŏg�p))
	int				judgeTimer;	//	����L������
	int				number;		//	�ԍ����蓖�ėp
	int				leanpower; //	�����莞�ԗp
	int				playerNum;	//���Ԗڂ̃v���C���[�̒e��
	int liveTime;	//��������Ă���̎���
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
	virtual bool	PlayerCollisionCheck(void);

	//	���擾�E�ݒ�
	void	SetPos(Vector3 pos);
	void	SetAngle(Vector3 angle);
	void	SetScale(Vector3 scale);
	Vector3	GetPos(void);
	Vector3		GetAngle(void);
	Vector3		GetRight();
	Vector3		GetUp();
	Vector3		GetFront();
	float		GetDegreeAngle(Vector3 vec1, Vector3 vec2);	//2�x�N�g���Ԃ̃x�N�g���擾(�x��)

protected:
	friend	class BulletManager;
};

//*****************************************************************************
#endif // !__BULLET_H__
