
#ifndef		__BASEOBJ_H__
#define		__BASEOBJ_H__

//****************************************************************************************
//
//	BaseObj�N���X
//
//****************************************************************************************

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

	enum KNOCKBACK_TYPE
	{
		KNOCKBACK_STRENGTH = 1,		//	��
		KNOCKBACK_MIDDLE,				//	��
		KNOCKBACK_WEAK,					//	��
	};

	enum COLLISION_TYPE
	{
		SPHEREVSCAPSULE = 1,	//	��VS�J�v�Z��
		SPHEREVSSPHERE,			//	��VS��
		CAPSULEVSCAPSULE,			//	�J�v�Z��VS�J�v�Z��
		SPHEREVSCYRINDER,			//	��VS�~���i�~���̉�]�Ȃ��j
		SPHEREVSPOINT,				//	��VS�_
	};

	enum STATE
	{
		MOVE,
		ATTACK,
		POWERARTS,
		QUICKARTS,
		HYPERARTS,
		JUMP,
		GUARD,
		DAMAGE,
		DAMAGE_STRENGTH,
		DAMAGE_MIDDLE,
		DAMAGE_WEAK,
	};
}

class BaseObj
{
private:

protected:
	//	�萔
	const float GRAVITY = -0.01f;
	static const int MIN_INPUT_STATE = 300;	//	�X�e�B�b�N����ŏ��l


protected:
	//	���f��
	iex3DObj*	obj;

	//	�R���g���[��
	iexInput*		input;

	//	�p�����[�^
	MotionData	motionData;
	Vector3			pos;
	Vector3			move;
	Vector3			knockBackVec;
	float				angle;
	float				scale;
	float				speed;
	float				force;
	bool				unrivaled;	//	���G
	bool				isGround;
	int					mode;
	int					coinNum;
	int					power;
	int					diffence;

	//	�����蔻��p
	int				attackParam;				//	�U�����
	int				knockBackType;			//	������΂����
	Vector3		attackPos_top;			//	�����蔻����W��
	Vector3		attackPos_bottom;		//	�����蔻����W��
	Vector3		attackPos;					//	�����蔻����W���S
	Vector3		attackPos2;
	Vector3		attackPos3;
	Vector3		attackPos4;
	float			attack_r;					//	�����蔻�蔼�a
	float			attack_t;					//	�����蔻��p�����p�����[�^

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
	void	CommonMove( void );
	void	CommonMove( float speed );
	void	CommonJump( void );
	void	CommonGuard( void );
	void	CommonKnockBackStrength( void );
	void	CommonKnockBackMiddle( void );
	void	CommonKnockBackWeak( void );
	void	CommonKnockBack( void );
	void	AddForce( float force );

	//	���ݒ�
	void	SetMode( PlayerData::STATE state );
	void	SetPos( Vector3 pos );
	void	SetPos( float x, float y, float z );
	void	SetAngle( float angle );
	void	SetScale( float scale );
	void	SetKnockBackVec( Vector3 knockBackVec );

	//	���擾
	Vector3	GetPos( void );
	Matrix	GetMatrix( void );
	float		GetAngle( void );
	int			GetCoinNum( void );
	bool		GetUnrivaled( void );
	int			GetMode( void );

	//	�����蔻��p�p�����[�^�擾
	int			GetAttackParam( void );
	int			GetKnockBackType(void);
	Vector3	GetAttackPos( void );
	Vector3	GetAttackPos_Top( void );
	Vector3	GetAttackPos_Bottom( void );
	float		GetAttack_R( void );
	float		GetAttack_T( void );
};



//****************************************************************************************
#endif // !__BASEOBJ_H__
