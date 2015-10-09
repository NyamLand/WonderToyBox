
#pragma once

//*******************************************************************************
//
//	BaseCharaクラス
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
			DAMAGE_STRENGTH,
			DAMAGE_MIDDLE,
			DAMAGE_WEAK,
			DAMAGE_LEANBACKWARD,
		};
	}

	namespace COLLISION_TYPE
	{
		enum
		{
			SPHEREVSCAPSULE = 1,	//	球VSカプセル
			SPHEREVSSPHERE,			//	球VS球
			CAPSULEVSCAPSULE,			//	カプセルVSカプセル
			SPHEREVSCYRINDER,			//	球VS円柱（円柱の回転なし）
			SPHEREVSPOINT,				//	球VS点
		};
	}

	namespace KNOCKBACK_TYPE
	{
		enum
		{
			STRENGTH = 1,		//	強
			MIDDLE,				//	中
			WEAK,					//	弱
			LEANBACKWARD,			//	ノックバックなし、仰け反り
		};
	}

	namespace CHARACTER_TYPE
	{
		enum
		{
			KNIGHT,		//	騎士
			PRINCESS,	//	姫
			SQUIRREL,	//	リス
			TIGER,			//	トラ
			MAX,
		};
	}
	
	namespace MOTION_NUM
	{
		enum 
		{
			STAND,					//	立ち
			POSTURE,				//	構え
			RUN,						//	走り
			ATTACK1,				//	攻撃１段階目
			JUMP,					//	ジャンプ
			LANDING,				//	着地
			ATTACK2,				//	攻撃２段階目
			ATTACK3,				//	攻撃３段階目
			GUARD,					//	ガード
		};
	}



	//	キャラクター名文字列
	namespace
	{
		//	キャラクター名文字列
		const		LPSTR	characterName[CHARACTER_TYPE::MAX] =
		{
			"ナイト",
			"プリンセス",
			"リス",
			"とら",
		};
	}
}

//	class
class BaseChara
{
private:
	//	モーション情報
	enum MOTION_DATA
	{
		STAND,					//	立ち
		RUN,						//	走り
		ATTACK1,				//	攻撃１段階目
		POSTURE,				//	構え
		JUMP,					//	ジャンプ
		LANDING,				//	着地
		ATTACK2,				//	攻撃２段階目
		ATTACK3,				//	攻撃３段階目
		GUARD,					//	ガード
	};

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
		float	r;
		float	t;
	};

	//	ノックバック情報
	struct KNOCKBACK_INFO
	{
		Vector3	vec;
		int			type;
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
		int		step_autorun;
		int		count_walk;		//	歩く時間（２～４秒）
		int		count_wait;		//　待機時間（１秒未満）
	};

protected:
	iex3DObj*	obj;

	//	基本パラメータ
	iexInput*	input;
	Vector3		pos;
	Vector3		move;
	float			drag;		//	抵抗力・摩擦力
	float			angle;
	float			scale;
	float			speed;
	float			force;
	float			diffence;
	bool			unrivaled;
	bool			isGround;
	bool			canHyper;
	bool			boosting;
	bool			isPlayer;
	bool			jumpState;
	int				mode;
	int				playerNum;
	int				power;
	int				leanFrame;		//	仰け反り時間
	int				jumpStep;			//	ジャンプ動作

	//	各情報構造体
	DAMAGECOLOR_INFO		damageColor;
	ATTACK_INFO				attackInfo;
	KNOCKBACK_INFO			knockBackInfo;
	AI_INFO						aiInfo;

	//	状態	
	PARAMETER_INFO		slip;
	PARAMETER_INFO		boost;
	PARAMETER_INFO		outrage;
	PARAMETER_INFO		attackUp;
	PARAMETER_INFO		speedUp;
	PARAMETER_INFO		bomb;
	PARAMETER_INFO		jump;

private:
	virtual	void	MotionManagement( int motion );
	virtual	void	ModeManagement( void );
	void	SetMotion( int motion );
	void	ParameterInfoInitialize( PARAMETER_INFO& ps );

public:
	//	初期化・解放
	BaseChara( void );
	~BaseChara( void );
	virtual	bool	Initialize( int playerNum, iex3DObj* org, Vector3 pos, bool isPlayer );
	virtual	bool	Initialize( int playerNum, iex3DObj* org, Vector3 pos );
	void	Release( void );

	//	更新・描画
	virtual	void	Update( void );
	virtual	void	Render(iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	動作関数
	void	CalcDrag( void );
	void	AddMove( void );
	void	StageCollisionCheck( void );
	void	CalcColoParameter( void );
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
	void	KnockBackStrength( void );
	void	KnockBackMiddle( void );
	void	KnockBackWeak( void );
	void	KnockBackLeanBackWard( void );

	//	パラメータ状態動作
	void	ParameterStateUpdate( void );
	void	AttackUp( void );

	//	子クラスで実装
	virtual	bool	QuickArts( void ) = 0;
	virtual	bool	PowerArts( void ) = 0;
	virtual	bool	HyperArts( void ) = 0;
	virtual	void	SetAttackParam( int attackKind ) = 0;

	//	操作方法別
	virtual	void	ControlAI( void );
	virtual	void	Control( void );

	//	AI動作関数
	int 	SearchCoin();					//　コイン探す
	bool	CheckSearchedCoin(int num);		//　見つけたコインが存在するか
	void	AutoRun();						//　コインを取りに行く
	void	AutoAngleAdjust(float speed, Vector3 target);
	//void	AutoAngleAdjust(const Vector3& direction, float speed);

	//	情報設定
	void	SetMode( int mode );
	void	SetMove( Vector3 move );
	void	SetPos( Vector3 pos );
	void	SetAngle( float angle );
	void	SetScale( float scale );
	void	SetDrag( float param );
	void	SetDamageColor( Vector3 color );
	void	SetPassColor( Vector3 color );
	void	SetLeanFrame( int frame );
	void	SetBoosting( bool boosting );
	void	SetKnockBackVec( Vector3 vec );
	void	SetUnrivaled( bool state );

	//	情報取得
	Matrix	GetMatrix( void )const;
	Vector3	GetPos( void )const;
	Vector3	GetMove( void )const;
	Vector3	GetFront( void )const;
	Vector3	GetRight( void )const;
	Vector3	GetUp( void )const;
	Vector3	GetDamageColor( void )const;
	Vector3	GetAttackPos( void )const;
	Vector3	GetAttackPos_Top( void )const;
	Vector3	GetAttackPos_Bottom( void )const;
	float		GetAngle( void )const;
	float		GetScale( void )const;
	float		GetAttack_R( void )const;
	float		GetAttack_T( void )const;
	bool		GetUnrivaled( void )const;
	bool		GetCanHyper( void )const;
	int			GetPower( void )const;
	int			GetMode( void )const;
	int			GetPlayerNum( void )const;
	int			GetAttackParam( void )const;
	int			GetKnockBackType( void )const;
	int			GetLeanFrame( void )const;
};
