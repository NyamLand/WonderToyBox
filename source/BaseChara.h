
#pragma once

//*******************************************************************************
//
//	BaseChara�N���X
//
//*******************************************************************************

//	include

//	parameter
namespace
{
	namespace PARAMETER_STATE
	{
		enum
		{
			UNRIVALED,
			SLIP,
			BOOST,
			OUTRAGE,
			MAGNET,
			ATTACKUP,
			SPEEDUP,
			BOMB,
			JUMP,
			CONFUSION,
			RESPAWN,
			UNRIVALEDITEM,
		};
	}

	namespace MODE_STATE
	{
		enum
		{
			WAIT,
			MOVE,
			ATTACK,
			QUICKARTS,
			POWERARTS,
			HYPERARTS,
			JUMP,
			GUARD,
			DAMAGE,
			DAMAGE_LEANBACKWARD,
			KNOCKBACK,
			DEATH,
		};
	}

	namespace AI_MODE_STATE
	{
		enum
		{
			WAIT,
			MOVE,
			ATTACK,
			QUICKARTS,
			POWERARTS,
			HYPERARTS,
			JUMP,
			GUARD,
			DAMAGE,
			DAMAGE_LEANBACKWARD,
			KNOCKBACK,
			DEATH,
			
			RUNAWAY,
			STOP
		};
	}

	namespace KNOCKBACK_TYPE
	{
		enum
		{
			NONE = -1,
			STRENGTH,		//	��
			MIDDLE,				//	��
			WEAK,					//	��
			LEANBACKWARD,			//	�m�b�N�o�b�N�Ȃ��A������
		};
	}

	namespace CHARACTER_TYPE
	{
		enum
		{
			SCAVENGER,		//�|����
			PRINCESS,	//	�P
			THIEF,		//�@����
			PIRATE,
			MAX,
		};
	}

	//�R�C���̗��Ƃ���
	namespace DROP_TYPE
	{
		enum
		{
			DROP,
			SUCTION,
		};
	}

	//	�L�����N�^�[��������
	namespace
	{
		//	�L�����N�^�[��������
		const		LPSTR	characterName[CHARACTER_TYPE::MAX] =
		{
			"�|����"
			"�v�����Z�X",
			"����",
			"�C��"
		};
	}

	//�@CPU���R�C����T���悤�ɂȂ邽�߂́A��ɑ��݂���R�C�������̊
	const int CPU_SERCH_COIN_MIN = 10;
}

//	class
class BaseChara
{
private:
protected:
	//	�_���[�W���F���
	struct DAMAGECOLOR_INFO
	{
		Vector3	param;	//	�F�ύX�p
		Vector3	passColor;	//	��������F
		Vector3	catchColor;	//	�󂯎��F
	};

	//	�U�����
	struct ATTACK_INFO
	{
		int		type;
		Vector3	pos;
		Vector3	top;
		Vector3	bottom;
		int		power;		//�U����
		int		dropPower;	//�R�C���𗎂Ƃ��́i���Ƃ������j
		int		addParam;	//�ǉ�����
		int		Interval;	//�U���Ԋu
		float	r;
		float	t;
		int		coinDropType;
	};

	//	�m�b�N�o�b�N���
	struct KNOCKBACK_INFO
	{
		Vector3	vec;
		int		type;
	};
	
	//	�p�����[�^���
	struct PARAMETER_INFO
	{
		bool	state;
		int		timer;
	};

	//	AI���
	struct AI_INFO
	{
		int		mode;
		int		param;
		bool	act_flag;		//�@�s�������itrue�F�s�����j
		int		runStraightCount;
		int		AdjustingCount;	//�@�p�x��������

		int		step_autorun;
		int		count_wait;		//�@�ҋ@���ԁi�P�b�����j
		int		count_run;		//	�������ԁi�Q�`�S�b�j
		int		count_runaway;	//�@��������
		int		count_attack;	//�@�U�����ԁi�L�����Ɉˑ��j
		int		count_guard;	
	};

	//	slip���
	struct SLIP_INFO
	{
		float	speed;
		float	drag;
	};


	//	plusStatus���
	struct PLUS_STATUS_INFO
	{
		int		power;
		float	speed;
		int		boostPower;		//�@���q�N���X�Ŋe�X�̒l������������
		float	boostSpeed;		//�@���q�N���X�Ŋe�X�̒l������������
	};

	struct SHADOW_INFO
	{
		iex2DObj*	obj;
		Vector3		pos;
		LVERTEX	v[4];
		float			scale;
	};

protected:
	iex3DObj*	obj;

	//	��{�p�����[�^
	iexInput*	input;
	Vector3		pos;
	Vector3		move;
	Vector3		angle;
	Vector3		objectMove;
	float				drag;		//	��R�́E���C��
	float				moveVec;
	float				scale;
	float				speed;
	float				totalSpeed;
	float				force;
	float				diffence;
	float				jumpPower;
	float				dt;
	float				param;			//	����
	float				moveAngle;
	bool				isGround;
	bool				canHyper;
	bool				inUseHyper;
	bool				isPlayer;
	bool				jumpState;
	bool				checkWall;
	bool				renderflag;
	bool				coinUnrivaled;
	bool				initflag;
	int				mode;
	int				playerNum;
	int				totalPower;
	int				leanFrame;		//	�����莞��
	int				jumpStep;			//	�W�����v����
	int				damageStep;
	int				rank;
	int				life;
	int				branktime;
	int				checkWallCount;

	//	�e���\����
	DAMAGECOLOR_INFO		damageColor;
	ATTACK_INFO				attackInfo;
	KNOCKBACK_INFO			knockBackInfo;
	AI_INFO						aiInfo;
	SLIP_INFO					slipInfo;
	PLUS_STATUS_INFO		plusStatusInfo;
	SHADOW_INFO				shadow;

	//	���	
	PARAMETER_INFO		unrivaled;
	PARAMETER_INFO		slip;
	PARAMETER_INFO		boost;
	PARAMETER_INFO		outrage;
	PARAMETER_INFO		attackUp;
	PARAMETER_INFO		speedUp;
	PARAMETER_INFO		magnet;
	PARAMETER_INFO		bomb;
	PARAMETER_INFO		jump;
	PARAMETER_INFO		confusion;
	PARAMETER_INFO		respawn;
	PARAMETER_INFO		unrivaledItem;

private:
	//virtual	void	MotionManagement( int motion );
	virtual	void	ModeManagement( void );
	void	ParameterInfoInitialize( void );
	void	ParameterInfoInitialize( PARAMETER_INFO& ps );

public:
	//	�������E���
	BaseChara( void );
	~BaseChara( void );
	virtual	bool	Initialize( int playerNum, Vector3 pos, bool isPlayer );
	virtual	bool	Initialize( int playerNum, Vector3 pos );
	virtual	void	ShadowInitialize( void );
	virtual	void	AttackParamInitialize(void);
	void	Release( void );

	//	�X�V�E�`��
	virtual	void	Update( void );
	virtual	void	Render(iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	����֐�
	void	CalcDrag( void );
	void	AddMove( void );
	void	StageCollisionCheck( void );
	void	CalcColorParameter( void );
	void	AngleAdjust( float speed );
	void	AngleAdjust( const Vector3& direction, float speed );
	void	Move( void );
	void	Move( float length );
	void	Run( void );
	void	Attack( int attackKind );
	void	Wait( void );
	void	Jump( void );
	void	Guard( void );
	void	Damage( void );
	void	KnockBack( void );
	void	Death( void );
	void	AddKnockBackForce(float force);
	void	KnockBackLeanBackWard( void );
	void	FallCheck( void );
	void	ParameterAdjust( void );
	void	ShadowUpdate( void );
	void	AddLife( void );
	void	SubLife( void );

	//	�p�����[�^��ԓ���
	void	Unrivaled(void);
	void	ParameterInfoUpdate( void );
	void	AttackUp( void );
	void	EventSlip( void );
	void	ItemMagnet( void );
	void	BoostUp( void );
	void	Confusion( void );
	void	Respawn( void );
	void	Magnet( void );
	void	SpeedUp( void );
	void	ItemUnrivaled( void );

	//	�q�N���X�Ŏ���
	virtual	void	CPU_ModeManagement( void );
	virtual	bool	QuickArts( void ) = 0;
	virtual	bool	PowerArts( void ) = 0;
	virtual	bool	HyperArts( void ) = 0;
	virtual	void	SetAttackParam( int attackKind ) = 0;

	//	������@��
	virtual	void	ControlAI( void );
	virtual	void	Control( void );

	//	AI����֐�
	virtual void	AutoMove( void );		
	virtual void	AutoWait();
	virtual void	AutoAttack( int attackKind );
	virtual void	AutoGuard();
	virtual void	AutoDamage(void);
	virtual void	AutoKnockBack(void);
	virtual void	AutoAddKnockBackForce(float force);
	virtual void	AutoKnockBackLeanBackWard(void);
	virtual void	AutoDeath(void);
	virtual void	RunAway(void);
	virtual void	Stop(void);

	virtual void	AutoAngleAdjust(float speed, Vector3 target);
	virtual void	AutoJump(void);
	virtual void	AutoPickCoin(void);
	
	//	���ݒ�
	void	SetMode( int mode );
	void	SetAIMode( int mode );
	void	SetMove( Vector3 move );
	void	SetPos( Vector3 pos );
	void	SetAngle( Vector3 angle );
	void	SetAngle( float angle );
	void	SetScale( float scale );
	void	SetDrag( float param );
	void	SetDamageColor( Vector3 color );
	void	SetPassColor( Vector3 color );
	void	SetLeanFrame( int frame );
	//void	SetBoosting( bool boosting );	//�@
	void	SetKnockBackVec( Vector3 vec );
	void	SetUnrivaled( bool state );
	void	SetCoinUnrivaled( bool state );
	void	SetParameterState( int parameterState );
	void	SetRank( int rank );
	void	SetParameterState( PARAMETER_INFO& paramterState, int time );
	void	SetForce(float force);
	void	SetMotion( int motion );
	void	SetLife( int life );

	//	���擾
	Matrix		GetMatrix( void )const;
	Matrix		GetBoneMatrix( int num )const;
	Vector3	GetPos( void )const;
	Vector3	GetMove( void )const;
	Vector3	GetFront( void )const;
	Vector3	GetRight( void )const;
	Vector3	GetUp( void )const;
	Vector3	GetDamageColor( void )const;
	Vector3	GetAttackPos( void )const;
	Vector3	GetAttackPos_Top( void )const;
	Vector3	GetAttackPos_Bottom( void )const;
	Vector3	GetBonePos( int num )const;
	Vector3	GetBoneFront( int num )const;
	Vector3	GetBoneRight( int num )const;
	Vector3	GetBoneUp( int num )const;
	float		GetAngle( void )const;
	float		GetAngle( Vector3 vec1, Vector3 vec2 )const;
	float		GetScale( void )const;
	int		GetAttack_addParam(void)const;
	float		GetAttack_R( void )const;
	float		GetAttack_T( void )const;
	float		GetTotalSpeed( void )const;
	bool		GetCanHyper( void )const;
	bool		GetParameterState( int type )const;
	bool		GetJumpFlag( void )const;
	bool		GetCoinUnrivaled( void )const;
	bool		GetIsPlayer( void )const;
	int		GetPower(void)const;
	int		GetDropPower(void)const;
	int		GetTotalPower( void )const;
	int		GetMode( void )const;
	int		GetAIMode( void )const;
	int		GetPlayerNum( void )const;
	int		GetAttackParam( void )const;
	int		GetKnockBackType(void)const;
	int		GetLeanFrame( void )const;
	int		GetRank( void )const;
	int		GetLife(void)const;
	int		GetDropType(void)const;
};
