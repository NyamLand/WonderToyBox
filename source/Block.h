
#ifndef __BLOCK_H__
#define	__BLOCK_H__

//*********************************************************************************
//
//	Block�N���X
//
//*********************************************************************************

class Block
{
private:
	iexMesh*	box;

	//	�p�����[�^
	Vector3		pos;
	float			scale;
	float			angle;
	//	�R���g���[��
	iexInput*		input;

	//	�p�����[�^
	Vector3		attackPos;
	Vector3		move;
	float			speed;
	int				mode;
	int				attackParam;

public:
	//	�������E���
	Block( void );
	~Block( void );
	bool	Initialize( int input, int type, Vector3 pos, float scale );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	Move( void );
	void	Move( float speed );

	//	�p�x�␳�֐�
	void	AngleAdjust(float speed);
	void	AngleAdjust(const Vector3& direction, float speed);

	//	���ݒ�
	void	SetPos(Vector3 pos);
	void	SetPos(float x, float y, float z);
	void	SetAngle(float angle);
	void	SetScale(float scale);

	//	���擾
	Vector3	GetPos(void);
	Vector3	GetAttackPos(void);
	Matrix	GetMatrix(void);
	float		GetAngle(void);
	int			GetAttackParam(void);

};
extern	Block*	m_Block;

//*********************************************************************************
#endif // !__BLOCK_H__
