
#ifndef		__PLAYER_H__
#define		__PLAYER_H__

//****************************************************************************************
//
//	Player関連クラス
//
//****************************************************************************************

namespace PlayerData
{
	const int CHARACTER_MAX = 4;

	//	被ダメージ用各色
	const Vector3	DAMAGE_COLOR[] =
	{
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	
	//	キャラクター名文字列
	const		LPSTR	characterName[CHARACTER_MAX] =
	{
		"ナイト",
		"プリンセス",
		"リス",
		"とら",
	};

	enum PLAYER_TYPE
	{
		KNIGHT,		//	騎士
		PRINCESS,	//	姫
		SQUIRREL,	//	リス
		TIGER,			//	トラ
	};

	enum KNOCKBACK_TYPE
	{
		KNOCKBACK_STRENGTH = 1,		//	強
		KNOCKBACK_MIDDLE,				//	中
		KNOCKBACK_WEAK,					//	弱
	};

	enum COLLISION_TYPE
	{
		SPHEREVSCAPSULE = 1,	//	球VSカプセル
		SPHEREVSSPHERE,			//	球VS球
		CAPSULEVSCAPSULE,			//	カプセルVSカプセル
		SPHEREVSCYRINDER,			//	球VS円柱（円柱の回転なし）
		SPHEREVSPOINT,				//	球VS点
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
	//	定数
	const float GRAVITY = -0.01f;
	static const int MIN_INPUT_STATE = 300;	//	スティック判定最小値

protected:
	//	モデル
	iex3DObj*	obj;

	//	コントローラ
	iexInput*		input;

	//	パラメータ
	MotionData	motionData;
	Vector3		pos;
	Vector3		move;
	float		angle;
	float		scale;
	float		speed;
	float		bSpeed;		//　ブースト中スピード
	int			power;
	int			bPower;		//　ブースト中パワー
	int			mode;
	float		force;		//	与力値
	bool		unrivaled;	//	無敵
	bool		isGround;	//	接地判定
	int			p_num;		//	自分の番号
	int			diffence;
	int			type;		//	プレイヤータイプ
	Vector3		colorParam;	//	色変更用
	Vector3		passDamageColor;	//	あたえる色
	Vector3		receiveDamageColor;	//	受け取る色
	bool		damageState;		//	ダメージ状態
	bool		CanHyper;		
	bool		boosting;	//　どんけつ中であるか

	//	当たり判定用
	int			attackParam;				//	攻撃種類
	int			knockBackType;			//	吹っ飛ばし種類
	Vector3		knockBackVec;			//	吹っ飛ばし方向
	Vector3		attackPos_top;			//	当たり判定座標上
	Vector3		attackPos_bottom;		//	当たり判定座標下
	Vector3		attackPos;					//	当たり判定座標中心
	float		attack_r;					//	当たり判定半径
	float		attack_t;					//	当たり判定用割合パラメータ

protected:
	//	関数
	void	SetMotion( int motion );
	void	SetMode( int mode );

public:
	//	初期化・解放
	Player( void );
	~Player( void );
	bool	Initialize( int input, iex3DObj* org, Vector3 pos );
	virtual	void	SetMotionData( void ) = 0;

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
	void	CommonKnockBackWeak( void );
	void	CommonKnockBack( void );
	void	AddForce( float force );
	void	Move( void );
	void	Jump( void );
	void	Guard( void );
	void	Attack( int attackKind );
	void	Damage( void );

	//	各キャラクターごとに設定
	virtual	bool	QuickArts( void ) = 0;
	virtual	bool	PowerArts( void ) = 0;
	virtual	bool	HyperArts( void ) = 0;
	virtual	void	SetAttackParam( int attackKind ) = 0;

	//	情報設定
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

	//	情報取得
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
