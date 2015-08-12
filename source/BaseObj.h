
#ifndef		__BASEOBJ_H__
#define		__BASEOBJ_H__

//****************************************************************************************
//
//	BaseObjクラス
//
//****************************************************************************************

namespace PlayerData
{
	enum PLAYER_TYPE
	{
		KNIGHT,		//	騎士
		PRINCESS,	//	姫
		KING,			//	大王
		BEAR,			//	クマ
		SQUIRREL,	//	リス
		TIGER,			//	トラ
		ANIMA,			//	アニマさん
		CROWS,		//	クロウズさん（綴り合ってる？）
		Y2009,			//	Y姉さん
		ECCMAN,		//	ECCマン
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

class BaseObj
{
private:

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
	Vector3			pos;
	Vector3			move;
	Vector3			knockBackVec;
	float				angle;
	float				scale;
	float				speed;
	float				force;
	bool				unrivaled;	//	無敵
	bool				isGround;
	int					mode;
	int					coinNum;
	int					power;
	int					diffence;

	//	当たり判定用
	int				attackParam;				//	攻撃種類
	int				knockBackType;			//	吹っ飛ばし種類
	Vector3		attackPos_top;			//	当たり判定座標上
	Vector3		attackPos_bottom;		//	当たり判定座標下
	Vector3		attackPos;					//	当たり判定座標中心
	Vector3		attackPos2;
	Vector3		attackPos3;
	Vector3		attackPos4;
	float			attack_r;					//	当たり判定半径
	float			attack_t;					//	当たり判定用割合パラメータ

protected:
	//	関数
	void	SetMotion( int motion );
	void	SetMode( int mode );

public:
	//	初期化・解放
	BaseObj( void );
	~BaseObj( void );
	bool	Initialize( int input, iex3DObj* org, Vector3 pos );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	動作関数
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

	//	情報設定
	void	SetMode( PlayerData::STATE state );
	void	SetPos( Vector3 pos );
	void	SetPos( float x, float y, float z );
	void	SetAngle( float angle );
	void	SetScale( float scale );
	void	SetKnockBackVec( Vector3 knockBackVec );

	//	情報取得
	Vector3	GetPos( void );
	Matrix	GetMatrix( void );
	float		GetAngle( void );
	int			GetCoinNum( void );
	bool		GetUnrivaled( void );
	int			GetMode( void );

	//	当たり判定用パラメータ取得
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
