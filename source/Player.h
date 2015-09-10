
#ifndef		__PLAYER_H__
#define		__PLAYER_H__

//****************************************************************************************
//
//	Player�֘A�N���X
//
//****************************************************************************************

namespace PlayerData
{
	const int CHARACTER_MAX = 4;

	//	��_���[�W�p�e�F
	const Vector3	DAMAGE_COLOR[] =
	{
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	
	//	�L�����N�^�[��������
	const		LPSTR	characterName[CHARACTER_MAX] =
	{
		"�i�C�g",
		"�v�����Z�X",
		"���X",
		"�Ƃ�",
	};

	enum PLAYER_TYPE
	{
		KNIGHT,		//	�R�m
		PRINCESS,	//	�P
		SQUIRREL,	//	���X
		TIGER,			//	�g��
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

class Player
{
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
	Vector3		pos;
	Vector3		move;
	float		angle;
	float		scale;
	float		speed;
	float		bSpeed;		//�@�u�[�X�g���X�s�[�h
	int			power;
	int			bPower;		//�@�u�[�X�g���p���[
	int			mode;
	float		force;		//	�^�͒l
	bool		unrivaled;	//	���G
	bool		isGround;	//	�ڒn����
	int			p_num;		//	�����̔ԍ�
	int			diffence;
	int			type;		//	�v���C���[�^�C�v
	Vector3		colorParam;	//	�F�ύX�p
	Vector3		passDamageColor;	//	��������F
	Vector3		receiveDamageColor;	//	�󂯎��F
	bool		damageState;		//	�_���[�W���
	bool		CanHyper;		
	bool		boosting;	//�@�ǂ񂯂��ł��邩

	//	�����蔻��p
	int			attackParam;				//	�U�����
	int			knockBackType;			//	������΂����
	Vector3		knockBackVec;			//	������΂�����
	Vector3		attackPos_top;			//	�����蔻����W��
	Vector3		attackPos_bottom;		//	�����蔻����W��
	Vector3		attackPos;					//	�����蔻����W���S
	float		attack_r;					//	�����蔻�蔼�a
	float		attack_t;					//	�����蔻��p�����p�����[�^

protected:
	//	�֐�
	void	SetMotion( int motion );
	void	SetMode( int mode );

public:
	//	�������E���
	Player( void );
	~Player( void );
	bool	Initialize( int input, iex3DObj* org, Vector3 pos );
	virtual	void	SetMotionData( void ) = 0;

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	virtual	void	Render( iexShader* shader, LPSTR technique ) = 0;

	//	����֐�
	void	ModeManagement( void );
	void	AngleAdjust( float speed );
	void	AngleAdjust( const Vector3& direction, float speed );
	void	StageCollisionCheck( void ); 
	void	CommonUpdate( void );
	void	CommonRender( void );
	void	CommonRender( iexShader* shader, LPSTR technique );
	void	CommonMove( void );
	void	CommonMove( float speed );
	void	CommonJump( void );
	void	CommonGuard( void );
	void	CommonKnockBackStrength( void );
	void	CommonKnockBackMiddle( void );
	void	CommonKnockBackWeak( void );
	void	CommonKnockBack( void );
	void	AddForce( float force );
	void	Move( void );
	void	Jump( void );
	void	Guard( void );
	void	Attack( int attackKind );
	void	Damage( void );

	//	�e�L�����N�^�[���Ƃɐݒ�
	virtual	bool	QuickArts( void ) = 0;
	virtual	bool	PowerArts( void ) = 0;
	virtual	bool	HyperArts( void ) = 0;
	virtual	void	SetAttackParam( int attackKind ) = 0;

	//	���ݒ�
	void	SetMode( PlayerData::STATE state );
	void	SetPos( Vector3 pos );
	void	SetPos( float x, float y, float z );
	void	SetAngle( float angle );
	void	SetScale( float scale );
	void	SetKnockBackVec( Vector3 knockBackVec );
	void	SetType( int type );
	void	SetDamageColor( Vector3 color );
	void	SetReceiveColor( Vector3 color );
	void	SetPower(int power);
	void	SetSpeed(float speed);
	void	SetBoosting(bool  boosting);

	//	���擾
	Vector3	GetPos( void );
	Matrix	GetMatrix( void );
	float	GetAngle( void );
	bool	GetUnrivaled( void );
	int		GetMode( void );
	int		GetType( void );
	Vector3	GetDamageColor( void );
	bool	GetCanHyper( void );
	int		GetPower( void );
	float	GetSpeed( void );


	//	�����蔻��p�p�����[�^�擾
	int		GetAttackParam( void );
	int		GetKnockBackType( void );
	Vector3	GetAttackPos( void );
	Vector3	GetAttackPos_Top( void );
	Vector3	GetAttackPos_Bottom( void );
	float	GetAttack_R( void );
	float	GetAttack_T( void );
	int		GetP_Num( void );
};

//****************************************************************************************
#endif // !__PLAYER_H__
