
#ifndef __BULLET_H__
#define	__BULLET_H__

//*****************************************************************************
//
//	Bullet�N���X
//
//*****************************************************************************
class Bullet
{
private:
	int				limitTimer;
private:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			state;
	bool			activate;
	int				judgeTimer;	//	����L������
	int				number;		//	�ԍ����蓖�ėp

public:
	//	�������E���
	Bullet(void);
	~Bullet(void);
	bool	Initialize(void);

	//	�X�V�E�`��
	void	Update(void);
	void	Render(void);
	void	Render(iexShader* shader, LPSTR technique);

	//	����֐�
	void	StageCollisionCheck(void);
	void	PlayerCollisionCheck(void);
	void	Move(void);

	//	���擾�E�ݒ�
	void	SetPos(Vector3 pos);
	void	SetAngle(float angle);
	void	SetScale(float angle);
	Vector3	GetPos(void);
	float		GetAngle(void);

private:
	friend	class BulletManager;
};

//*****************************************************************************
#endif // !__BULLET_H__
