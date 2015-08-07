
#ifndef		__BASEOBJ_H__
#define		__BASEOBJ_H__

//****************************************************************************************
//
//	BaseObjクラス
//
//****************************************************************************************
class BaseObj
{
private:
	enum MotionNum
	{
		STAND,					//	立ち
		POSTURE,				//	構え
		RUN,						//	走り
		JUMP,					//	ジャンプ
		LANDING,				//	着地
		ATTACK1,				//	攻撃１段階目
		ATTACK2,				//	攻撃２段階目
		ATTACK3,				//	攻撃３段階目
		GUARD,					//	ガード
	};

protected:
	//	定数
	const float GRAVITY = -0.01f;
	static const int MIN_INPUT_STATE = 300;	//	スティック判定最小値
	enum STATE
	{
		MOVE,
		ATTACK,
		POWERARTS,
		QUICKARTS,
		HYPERARTS,
		DAMAGE,
	};

protected:
	//	モデル
	iex3DObj*	obj;

	//	コントローラ
	iexInput*		input;

	//	パラメータ
	MotionData	motionData;
	Vector3		pos;
	Vector3		attackPos;
	Vector3		move;
	float			angle;
	float			scale;
	float			speed;
	int				mode;
	int				attackParam;
	bool			isGround;
	int				coinNum;

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
	void	CommonMove( void );		//	共通動作
	void	CommonMove( float speed );
	void	CommonJump( void );
	void	CommonGuard( void );

	//	情報設定
	void	SetPos( Vector3 pos );
	void	SetPos( float x, float y, float z );
	void	SetAngle( float angle );
	void	SetScale( float scale );

	//	情報取得
	Vector3	GetPos( void );
	Vector3	GetAttackPos( void );
	Matrix	GetMatrix( void );
	float		GetAngle( void );
	int			GetAttackParam( void );
	int			GetCoinNum( void );
};

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
}

//****************************************************************************************
#endif // !__BASEOBJ_H__
