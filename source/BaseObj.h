
#ifndef		__BASEOBJ_H__
#define		__BASEOBJ_H__

//****************************************************************************************
//
//	BaseObj�N���X
//
//****************************************************************************************
class BaseObj
{
protected:
	//	�萔
	const float GRAVITY = -0.01f;
	enum STATE
	{
		MOVE,
		ATTACK,
		DAMAGE,
	};

protected:
	//	���f��
	iex3DObj*	obj;

	//	�R���g���[��
	iexInput*		input;

	//	�p�����[�^
	Vector3		pos;
	Vector3		attackPos;
	Vector3		move;
	float			angle;
	float			scale;
	float			speed;
	int				mode;
	int				attackParam;
	bool			isGround;

protected:
	//	�֐�
	void	SetMotion( int motion );
	void	SetMode( int mode );

public:
	//	�������E���
	BaseObj( void );
	~BaseObj( void );
	bool	Initialize( int input, int type, Vector3 pos );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	void	StageCollisionCheck( void );

	//	���ݒ�
	void	SetPos( Vector3 pos );
	void	SetPos( float x, float y, float z );
	void	SetAngle( float angle );
	void	SetScale( float scale );

	//	���擾
	Vector3	GetPos( void );
	Vector3	GetAttackPos( void );
	Matrix	GetMatrix( void );
	float		GetAngle( void );
	int			GetAttackParam( void );
	
};

//****************************************************************************************
#endif // !__BASEOBJ_H__
