
#ifndef		__BASEOBJ_H__
#define		__BASEOBJ_H__

//****************************************************************************************
//
//	BaseObj�N���X
//
//****************************************************************************************
class BaseObj
{
private:
	enum MotionNum
	{
		STAND,					//	����
		POSTURE,				//	�\��
		RUN,						//	����
		JUMP,					//	�W�����v
		LANDING,				//	���n
		ATTACK1,				//	�U���P�i�K��
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		GUARD,					//	�K�[�h
	};

protected:
	//	�萔
	const float GRAVITY = -0.01f;
	static const int MIN_INPUT_STATE = 300;	//	�X�e�B�b�N����ŏ��l
	enum STATE
	{
		MOVE,
		ATTACK,
		POWERARTS,
		QUICKARTS,
		HYPERARTS,
		DAMAGE,
	};

protected:
	//	���f��
	iex3DObj*	obj;

	//	�R���g���[��
	iexInput*		input;

	//	�p�����[�^
	MotionData	motionData;
	Vector3		pos;
	Vector3		attackPos;
	Vector3		move;
	float			angle;
	float			scale;
	float			speed;
	int				mode;
	int				attackParam;
	bool			isGround;
	int				coinNum;

protected:
	//	�֐�
	void	SetMotion( int motion );
	void	SetMode( int mode );

public:
	//	�������E���
	BaseObj( void );
	~BaseObj( void );
	bool	Initialize( int input, iex3DObj* org, Vector3 pos );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	void	AngleAdjust( float speed );
	void	AngleAdjust( const Vector3& direction, float speed );
	void	StageCollisionCheck( void );
	void	AddCoin( void );
	void	SubCoin( void );
	void	CommonMove( void );		//	���ʓ���
	void	CommonMove( float speed );
	void	CommonJump( void );
	void	CommonGuard( void );

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
	int			GetCoinNum( void );
};

namespace PlayerData
{
	enum PLAYER_TYPE
	{
		KNIGHT,		//	�R�m
		PRINCESS,	//	�P
		KING,			//	�剤
		BEAR,			//	�N�}
		SQUIRREL,	//	���X
		TIGER,			//	�g��
		ANIMA,			//	�A�j�}����
		CROWS,		//	�N���E�Y����i�Ԃ荇���Ă�H�j
		Y2009,			//	Y�o����
		ECCMAN,		//	ECC�}��
	};
}

//****************************************************************************************
#endif // !__BASEOBJ_H__
