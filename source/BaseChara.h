
#pragma once

//*******************************************************************************
//
//	BaseCharaクラス
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
			STRENGTH,		//	強
			MIDDLE,				//	中
			WEAK,					//	弱
			LEANBACKWARD,			//	ノックバックなし、仰け反り
		};
	}

	namespace CHARACTER_TYPE
	{
		enum
		{
			SCAVENGER,		//掃除屋
			PRINCESS,	//	姫
			THIEF,		//　怪盗
			PIRATE,
			MAX,
		};
	}

	//コインの落とし方
	namespace DROP_TYPE
	{
		enum
		{
			DROP,
			SUCTION,
		};
	}

	//	キャラクター名文字列
	namespace
	{
		//	キャラクター名文字列
		const		LPSTR	characterName[CHARACTER_TYPE::MAX] =
		{
			"掃除屋"
			"プリンセス",
			"怪盗",
			"海賊"
		};
	}

	//　CPUがコインを探すようになるための、場に存在するコイン枚数の基準
	const int CPU_SERCH_COIN_MIN = 10;
}

//	class
class BaseChara
{
private:
protected:
	//	ダメージ時色情報
	struct DAMAGECOLOR_INFO
	{
		Vector3	param;	//	色変更用
		Vector3	passColor;	//	あたえる色
		Vector3	catchColor;	//	受け取る色
	};

	//	攻撃情報
	struct ATTACK_INFO
	{
		int		type;
		Vector3	pos;
		Vector3	top;
		Vector3	bottom;
		int		power;		//攻撃力
		int		dropPower;	//コインを落とす力（落とす枚数）
		int		addParam;	//追加効果
		int		Interval;	//攻撃間隔
		float	r;
		float	t;
		int		coinDropType;
	};

	//	ノックバック情報
	struct KNOCKBACK_INFO
	{
		Vector3	vec;
		int		type;
	};
	
	//	パラメータ情報
	struct PARAMETER_INFO
	{
		bool	state;
		int		timer;
	};

	//	AI情報
	struct AI_INFO
	{
		int		mode;
		int		param;
		bool	act_flag;		//　行動中か（true：行動中）
		int		runStraightCount;
		int		AdjustingCount;	//　角度調整時間

		int		step_autorun;
		int		count_wait;		//　待機時間（１秒未満）
		int		count_run;		//	歩く時間（２～４秒）
		int		count_runaway;	//　逃げ時間
		int		count_attack;	//　攻撃時間（キャラに依存）
		int		count_guard;	
	};

	//	slip情報
	struct SLIP_INFO
	{
		float	speed;
		float	drag;
	};


	//	plusStatus情報
	struct PLUS_STATUS_INFO
	{
		int		power;
		float	speed;
		int		boostPower;		//　→子クラスで各々の値を初期化して
		float	boostSpeed;		//　→子クラスで各々の値を初期化して
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

	//	基本パラメータ
	iexInput*	input;
	Vector3		pos;
	Vector3		move;
	Vector3		angle;
	Vector3		objectMove;
	float				drag;		//	抵抗力・摩擦力
	float				moveVec;
	float				scale;
	float				speed;
	float				totalSpeed;
	float				force;
	float				diffence;
	float				jumpPower;
	float				dt;
	float				param;			//	割合
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
	int				leanFrame;		//	仰け反り時間
	int				jumpStep;			//	ジャンプ動作
	int				damageStep;
	int				rank;
	int				life;
	int				branktime;
	int				checkWallCount;

	//	各情報構造体
	DAMAGECOLOR_INFO		damageColor;
	ATTACK_INFO				attackInfo;
	KNOCKBACK_INFO			knockBackInfo;
	AI_INFO						aiInfo;
	SLIP_INFO					slipInfo;
	PLUS_STATUS_INFO		plusStatusInfo;
	SHADOW_INFO				shadow;

	//	状態	
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
	//	初期化・解放
	BaseChara( void );
	~BaseChara( void );
	virtual	bool	Initialize( int playerNum, Vector3 pos, bool isPlayer );
	virtual	bool	Initialize( int playerNum, Vector3 pos );
	virtual	void	ShadowInitialize( void );
	virtual	void	AttackParamInitialize(void);
	void	Release( void );

	//	更新・描画
	virtual	void	Update( void );
	virtual	void	Render(iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	動作関数
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

	//	パラメータ状態動作
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

	//	子クラスで実装
	virtual	void	CPU_ModeManagement( void );
	virtual	bool	QuickArts( void ) = 0;
	virtual	bool	PowerArts( void ) = 0;
	virtual	bool	HyperArts( void ) = 0;
	virtual	void	SetAttackParam( int attackKind ) = 0;

	//	操作方法別
	virtual	void	ControlAI( void );
	virtual	void	Control( void );

	//	AI動作関数
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
	
	//	情報設定
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
	//void	SetBoosting( bool boosting );	//　
	void	SetKnockBackVec( Vector3 vec );
	void	SetUnrivaled( bool state );
	void	SetCoinUnrivaled( bool state );
	void	SetParameterState( int parameterState );
	void	SetRank( int rank );
	void	SetParameterState( PARAMETER_INFO& paramterState, int time );
	void	SetForce(float force);
	void	SetMotion( int motion );
	void	SetLife( int life );

	//	情報取得
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
