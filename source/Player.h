
#ifndef		__PLAYER_H__
#define		__PLAYER_H__

//****************************************************************************************
//
//	Player関連クラス
//
//****************************************************************************************

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

	namespace PLAYER_TYPE
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

	namespace
	{
		//	キャラクター名文字列
		const		LPSTR	characterName[PLAYER_TYPE::MAX] =
		{
			"ナイト",
			"プリンセス",
			"リス",
			"とら",
		};
	}

	namespace
	{
		//	被ダメージ用各色
		const Vector3	DAMAGE_COLOR[] =
		{
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f),
		};
	}
}

class Player
{
protected:
	//	定数
	static const int MIN_INPUT_STATE = 300;	//	スティック判定最小値

protected:
	struct ParameterState
	{
		bool	state;
		int		timer;
	};

protected:
	//	モデル
	iex3DObj*	obj;

	//	コントローラ
	iexInput*		input;

	//	基本情報
	MotionData	motionData;
	Vector3			pos;
	Vector3			move;
	float				resistance;	//	抵抗力・摩擦力
	float				angle;
	float				scale;
	float				speed;
	int					power;
	int					mode;
	int					diffence;
	float				force;		//	与力値
	bool				unrivaled;	//	無敵
	bool				isGround;	//	接地判定
	int					p_num;		//	自分の番号
	int					type;			//	プレイヤータイプ
	int					leanFrame;		//　仰け反り時間

	//	ブースト情報
	float		bSpeed;		//　ブースト中スピード
	int			bPower;		//　ブースト中パワー

	//	シェーダーカラー情報
	Vector3		colorParam;	//	色変更用
	Vector3		passDamageColor;	//	あたえる色
	Vector3		receiveDamageColor;	//	受け取る色
	bool		damageState;		//	ダメージ状態
	bool		CanHyper;		

	//	当たり判定用
	int			attackParam;				//	攻撃種類
	int			knockBackType;			//	吹っ飛ばし種類
	Vector3		knockBackVec;			//	吹っ飛ばし方向
	Vector3		attackPos_top;			//	当たり判定座標上
	Vector3		attackPos_bottom;		//	当たり判定座標下
	Vector3		attackPos;					//	当たり判定座標中心
	float		attack_r;					//	当たり判定半径
	float		attack_t;					//	当たり判定用割合パラメータ

	//	状態
	ParameterState		slip;
	ParameterState		boost;
	ParameterState		outrage;
	ParameterState		attackUp;
	ParameterState		speedUp;
	ParameterState		bomb;
	ParameterState		jump;

	bool		boosting;	//　どんけつ中であるか

	//	上昇値
	//int		plusPower;
	//int		plusSpeed;

protected:
	//	関数
	void	SetMotion( int motion );

public:
	//	初期化・解放
	Player( void );
	~Player( void );
	bool	Initialize( int input, iex3DObj* org, Vector3 pos );
	virtual	void	SetMotionData( void ) = 0;
	void	ParameterStateInitialize( void );
	void	ParameterStateInitialize( ParameterState& ps );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	virtual	void	Render( iexShader* shader, LPSTR technique ) = 0;

	//	動作関数
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
	void	CommonKnockBackWeak(void);
	void	CommonKnockBackLeanBackWard(void);
	void	CommonKnockBack( void );
	void	AddForce( float force );
	void	Wait( void );
	void	Move( void );
	void	Jump( void );
	void	Guard( void );
	void	Attack( int attackKind );
	void	Damage( void );
	
	//	パラメータ状態動作
	void	ParameterStateUpdate( void );
	void	AttackUp( void );

	//	各キャラクターごとに設定
	virtual	bool	QuickArts( void ) = 0;
	virtual	bool	PowerArts( void ) = 0;
	virtual	bool	HyperArts( void ) = 0;
	virtual	void	SetAttackParam( int attackKind ) = 0;

	//	情報設定
	void	SetMode( const int& state );
	void	SetPos( const Vector3& pos );
	void	SetPos( const float& x, const float& y, const float& z );
	void	SetAngle( const float& angle );
	void	SetScale( const float& scale );
	void	SetKnockBackVec( const Vector3& knockBackVec );
	void	SetType( const int& type );
	void	SetLeanFrame( const int& frame );
	void	SetDamageColor( const Vector3& color );
	void	SetReceiveColor( const Vector3& color );
	void	SetPower( const int& power );
	void	SetSpeed( const float& speed );
	void	SetBoosting( const bool& boosting );
	void	SetParameterState( const PARAMETER_STATE::PARAMETERSTATE& parameterState );
	void	SetParameterState( ParameterState& parameterState, const int& time );
	void	SetResistance( const float& resistance );
	void	SetMove( const Vector3& move );


	//	情報取得
	Vector3	GetPos( void );
	Vector3	GetDamageColor( void );
	Vector3	GetFront( void );
	Vector3	GetRight( void );
	Vector3	GetUp( void );
	Vector3	GetMove( void );
	Matrix	GetMatrix( void );
	float	GetAngle( void );
	float	GetSpeed( void );
	float	GetResistance( void );
	bool	GetCanHyper( void );
	bool	GetUnrivaled( void );
	int		GetMode( void );
	int		GetType( void );
	int		GetPower( void );
	int		GetLeanFrame(void);
	

	//	当たり判定用パラメータ取得
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
