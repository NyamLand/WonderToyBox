
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
		Vector3( 1.0f, 1.0f, 1.0f ),
		Vector3( 1.0f, 1.0f, 1.0f ),
		Vector3( 1.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 1.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 1.0f ),
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
		WAIT,
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

namespace
{
	namespace PARAMETER_STATE
	{
		enum PARAMETERSTATE
		{
			SLIP,
			BOOST,
			OUTRAGE,
			ATTACKUP,
			SPEEDUP,
			BOMB,
			JUMP,
		};
	}
}

class Player
{
protected:

protected:
	struct ParameterState
	{
		bool	state;
		int		timer;
	};

protected:
	//	���f��
	iex3DObj*	obj;

	//	�R���g���[��
	iexInput*		input;

	//	��{���
	MotionData	motionData;
	Vector3			pos;
	Vector3			move;
	float				resistance;	//	��R�́E���C��
	float				angle;
	float				scale;
	float				speed;
	int					power;
	int					mode;
	int					diffence;
	float				force;		//	�^�͒l
	bool				unrivaled;	//	���G
	bool				isGround;	//	�ڒn����
	int					p_num;		//	�����̔ԍ�
	int					type;			//	�v���C���[�^�C�v

	//	�u�[�X�g���
	float		bSpeed;		//�@�u�[�X�g���X�s�[�h
	int			bPower;		//�@�u�[�X�g���p���[

	//	�V�F�[�_�[�J���[���
	Vector3		colorParam;	//	�F�ύX�p
	Vector3		passDamageColor;	//	��������F
	Vector3		receiveDamageColor;	//	�󂯎��F
	bool		damageState;		//	�_���[�W���
	bool		CanHyper;		

	//	�����蔻��p
	int			attackParam;				//	�U�����
	int			knockBackType;			//	������΂����
	Vector3		knockBackVec;			//	������΂�����
	Vector3		attackPos_top;			//	�����蔻����W��
	Vector3		attackPos_bottom;		//	�����蔻����W��
	Vector3		attackPos;					//	�����蔻����W���S
	float		attack_r;					//	�����蔻�蔼�a
	float		attack_t;					//	�����蔻��p�����p�����[�^

	//	���
	ParameterState		slip;
	ParameterState		boost;
	ParameterState		outrage;
	ParameterState		attackUp;
	ParameterState		speedUp;
	ParameterState		bomb;
	ParameterState		jump;

	bool		boosting;	//�@�ǂ񂯂��ł��邩

	//	�㏸�l
	//int		plusPower;
	//int		plusSpeed;

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
	void	ParameterStateInitialize( void );
	void	ParameterStateInitialize( ParameterState& ps );

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
	void	Wait( void );
	void	Move( void );
	void	Jump( void );
	void	Guard( void );
	void	Attack( int attackKind );
	void	Damage( void );
	
	//	�p�����[�^��ԓ���
	void	ParameterStateUpdate( void );
	void	AttackUp( void );

	//	�e�L�����N�^�[���Ƃɐݒ�
	virtual	bool	QuickArts( void ) = 0;
	virtual	bool	PowerArts( void ) = 0;
	virtual	bool	HyperArts( void ) = 0;
	virtual	void	SetAttackParam( int attackKind ) = 0;

	//	���ݒ�
	void	SetMode( const PlayerData::STATE& state );
	void	SetPos( const Vector3& pos );
	void	SetPos( const float& x, const float& y, const float& z );
	void	SetAngle( const float& angle );
	void	SetScale( const float& scale );
	void	SetKnockBackVec( const Vector3& knockBackVec );
	void	SetType( const int& type );
	void	SetDamageColor( const Vector3& color );
	void	SetReceiveColor( const Vector3& color );
	void	SetPower( const int& power );
	void	SetSpeed( const float& speed );
	void	SetBoosting( const bool& boosting );
	void	SetParameterState( const PARAMETER_STATE::PARAMETERSTATE& parameterState );
	void	SetParameterState( ParameterState& parameterState, const int& time );
	void	SetResistance( const float& resistance );
	void	SetMove( const Vector3& move );


	//	���擾
	Vector3	GetPos( void )const;
	Vector3	GetDamageColor( void )const;
	Vector3	GetFront( void )const;
	Vector3	GetRight( void )const;
	Vector3	GetUp( void )const;
	Vector3	GetMove( void )const;
	Matrix	GetMatrix( void )const;
	float	GetAngle( void )const;
	float	GetSpeed( void )const;
	float	GetResistance( void )const;
	bool	GetCanHyper( void )const;
	bool	GetUnrivaled( void )const;
	int		GetMode( void )const;
	int		GetType( void )const;
	int		GetPower( void )const;

	//	�����蔻��p�p�����[�^�擾
	int		GetAttackParam( void )const;
	int		GetKnockBackType( void )const;
	Vector3	GetAttackPos( void )const;
	Vector3	GetAttackPos_Top( void )const;
	Vector3	GetAttackPos_Bottom( void )const;
	float	GetAttack_R( void )const;
	float	GetAttack_T( void )const;
	int		GetP_Num( void )const;
};

//****************************************************************************************
#endif // !__PLAYER_H__
