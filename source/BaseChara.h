
#pragma once

//*******************************************************************************
//
//	BaseChara�N���X
//
//*******************************************************************************

//	include
#include	"ItemManager.h"

//	parameter
namespace
{
	namespace PARAMETER_STATE
	{
		enum
		{
			SLIP,
			BOOST,
			OUTRAGE,
			MAGNET,
			ATTACKUP,
			SPEEDUP,
			BOMB,
			JUMP,
		};
	}

	namespace MODE_STATE
	{
		enum
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
			DAMAGE_FLYUP,
			DAMAGE_LEANBACKWARD
		};
	}

	namespace COLLISION_TYPE
	{
		enum
		{
			SPHEREVSCAPSULE = 1,	//	��VS�J�v�Z��
			SPHEREVSSPHERE,			//	��VS��
			CAPSULEVSCAPSULE,			//	�J�v�Z��VS�J�v�Z��
			SPHEREVSCYRINDER,			//	��VS�~���i�~���̉�]�Ȃ��j
			SPHEREVSPOINT,				//	��VS�_
		};
	}

	namespace KNOCKBACK_TYPE
	{
		enum
		{
			STRENGTH = 1,		//	��
			MIDDLE,				//	��
			WEAK,					//	��
			LEANBACKWARD,			//	�m�b�N�o�b�N�Ȃ��A������
		};
	}

	namespace CHARACTER_TYPE
	{
		enum
		{
			KNIGHT,		//	�R�m
			PRINCESS,	//	�P
			SQUIRREL,	//	���X
			TIGER,			//	�g��
			MAX,
		};
	}
	
	namespace MOTION_NUM
	{
		enum 
		{
			STAND,					//	����
			POSTURE,				//	�\��
			RUN,						//	����
			ATTACK1,				//	�U���P�i�K��
			JUMP,					//	�W�����v
			LANDING,				//	���n
			ATTACK2,				//	�U���Q�i�K��
			ATTACK3,				//	�U���R�i�K��
			GUARD,					//	�K�[�h
		};
	}

	//	�L�����N�^�[��������
	namespace
	{
		//	�L�����N�^�[��������
		const		LPSTR	characterName[CHARACTER_TYPE::MAX] =
		{
			"�i�C�g",
			"�v�����Z�X",
			"���X",
			"�Ƃ�",
		};
	}
}

//	class
class BaseChara
{
private:
	//	���[�V�������
	enum MOTION_DATA
	{
		STAND,					//	����
		RUN,					//	����
		ATTACK1,				//	�U���P�i�K��
		POSTURE,				//	�\��
		JUMP,					//	�W�����v
		LANDING,				//	���n
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		GUARD,					//	�K�[�h
	};

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
		float	r;
		float	t;
	};

	//	�m�b�N�o�b�N���
	struct KNOCKBACK_INFO
	{
		Vector3	vec;
		int			type;
		bool	isUp;
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

protected:
	iex3DObj*	obj;

	//	��{�p�����[�^
	iexInput*	input;
	Vector3		pos;
	Vector3		move;
	float			drag;		//	��R�́E���C��
	float			angle;
	float			moveVec;
	float			scale;
	float			speed;
	float			totalSpeed;
	float			force;
	float			diffence;
	bool			unrivaled;
	bool			isGround;
	bool			canHyper;
	//bool			boosting;	//�@�� PARAMETER_INFO�^�� boost
	bool			isPlayer;
	bool			jumpState;
	bool			checkWall;
	int				mode;
	int				playerNum;
	int				power;
	int				totalPower;
	int				leanFrame;		//	�����莞��
	int				jumpStep;			//	�W�����v����
	int				damageStep;
	int				rank;

	//	�e���\����
	DAMAGECOLOR_INFO		damageColor;
	ATTACK_INFO				attackInfo;
	KNOCKBACK_INFO			knockBackInfo;
	AI_INFO						aiInfo;
	SLIP_INFO						slipInfo;
	PLUS_STATUS_INFO		plusStatusInfo;

	//	���	
	PARAMETER_INFO		slip;
	PARAMETER_INFO		boost;
	PARAMETER_INFO		outrage;
	PARAMETER_INFO		attackUp;
	PARAMETER_INFO		speedUp;
	PARAMETER_INFO		magnet;
	PARAMETER_INFO		bomb;
	PARAMETER_INFO		jump;

private:
	virtual	void	MotionManagement( int motion );
	virtual	void	ModeManagement( void );
	void	SetMotion( int motion );
	void	ParameterInfoInitialize( void );
	void	ParameterInfoInitialize( PARAMETER_INFO& ps );

public:
	//	�������E���
	BaseChara( void );
	~BaseChara( void );
	virtual	bool	Initialize( int playerNum, Vector3 pos, bool isPlayer );
	virtual	bool	Initialize( int playerNum, Vector3 pos );
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
	void	KnockBack(void);
	void	AddKnockBackForce(float force);
	void	KnockBackLeanBackWard( void );
	void	FallCheck( void );
	void	ParameterAdjust( void );

	//	�p�����[�^��ԓ���
	void	ParameterInfoUpdate( void );
	void	AttackUp( void );
	void	EventSlip( void );
	void	ItemMagnet( void );
	void	BoostUp( void );

	//	�q�N���X�Ŏ���
	virtual	bool	QuickArts( void ) = 0;
	virtual	bool	PowerArts( void ) = 0;
	virtual	bool	HyperArts( void ) = 0;
	virtual	void	SetAttackParam( int attackKind ) = 0;

	//	������@��
	virtual	void	ControlAI( void );
	virtual	void	Control( void );

	//	AI����֐�
	void	AutoRun();						//�@�R�C�������ɍs��
	void	AutoAngleAdjust(float speed, Vector3 target);
	//void	AutoAngleAdjust(const Vector3& direction, float speed);
	void	AutoAttack();
	void	RunAway();
	void	AutoGuard();
	void	AutoWait();

	//	���ݒ�
	void	SetMode( int mode );
	void	SetAIMode( int mode );
	void	SetMove( Vector3 move );
	void	SetPos( Vector3 pos );
	void	SetAngle( float angle );
	void	SetScale( float scale );
	void	SetDrag( float param );
	void	SetDamageColor( Vector3 color );
	void	SetPassColor( Vector3 color );
	void	SetLeanFrame( int frame );
	//void	SetBoosting( bool boosting );	//�@
	void	SetKnockBackVec( Vector3 vec );
	void	SetUnrivaled( bool state );
	void	SetParameterState( int parameterState );
	void	SetRank( int rank );
	void	SetParameterState( PARAMETER_INFO& paramterState, int time );
	void	SetForce(float force);

	//	���擾
	Matrix	GetMatrix( void )const;
	Matrix	GetBoneMatrix( int num )const;
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
	float	GetAngle( void )const;
	float	GetScale( void )const;
	float	GetAttack_R( void )const;
	float	GetAttack_T( void )const;
	//float	GetSpeed( void )const;
	float	GetTotalSpeed( void )const;
	bool	GetUnrivaled( void )const;
	bool	GetCanHyper( void )const;
	bool	GetParameterState( int type )const;
	int		GetPower( void )const;
	int		GetTotalPower( void )const;
	int		GetMode( void )const;
	int		GetAIMode( void )const;
	int		GetPlayerNum( void )const;
	int		GetAttackParam( void )const;
	int		GetKnockBackType(void)const;
	int		GetKnockBackIsUp(void)const;
	int		GetLeanFrame( void )const;
	int		GetRank( void )const;
};
