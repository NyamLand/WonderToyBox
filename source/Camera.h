
#ifndef		__CAMERA_H__
#define		__CAMERA_H__

//****************************************************************************************
//
//	Cameraクラス
//
//****************************************************************************************

//	param
namespace
{
	namespace VIEW_MODE
	{
		enum
		{
			SETUP,
			FIX,
			SLERP,
			CHASE,		//	※このモードの場合はSetPlayerInfoで先にPlayer情報を入れなければいけない
			RESULT,
			TITLE,
			INDIVIDUAL,
		};
	}

	namespace TITLE_MOVE_INFO
	{
		float	height = 5.0f;
		Vector3	pos[] =
		{
			Vector3( 8.0f, height, 0.0f ),		//門番
			Vector3( 45.0f, height, -14.0f ),	//	オプション
			Vector3( -41.0f, height, -20.0f ),	//	クレジット		
			Vector3( 46.0f, height, 200.0f ),	//	メインへ
		};

		//	上と同じ
		Vector3	target[] =
		{
			Vector3( 8.0f, 3.0f, 13.0f ),
			Vector3( 53.0f, 3.0f, -5.0f ),
			Vector3( -50.0f, 3.0f, -14.0f ),
			Vector3( 46.0f, 4.0f, 200.0f ),
		};
	}
	
	namespace TITLE_TARGET
	{
		enum
		{
			PLAY,
			OPTION,
			CREDIT,
			MOVE_MAIN,
			END,
		};
	}
}

//	クラス前方宣言
class Rubber;

//	class
class Camera : public iexView
{
private:
	//	定数
	static	const		int		MAX = 10;		//	カメラからターゲットの距離の最大
	static	const		int		MIN = 4;		//								最少
	static	const		int		PLAYER_NUM = 4;	//	プレイヤーの数

private:
	Vector3	playerPos[PLAYER_NUM];
	Vector3 target;
	Vector3	nextPoint;
	Vector3	nextTarget;
	Vector3	startPos;
	Vector3 pos;
	float		length;
	float		speed;
	float		t;
	float		playerAngle[PLAYER_NUM];
	bool		moveState;
	
	//	ゴムパラメータ
	Rubber*		q;
	
	//	振動用パラメータ
	Vector3 adjust;
	bool		shakeflag;
	int			shakeTimer;
	float		wide;

	//	球面線形補間用
	D3DXQUATERNION	orientation;

public:
	//	初期化・解放
	Camera( void );
	~Camera( void );

	//	更新・描画
	void	Update( int viewmode, Vector3 target = Vector3( 0.0f, 0.0f, 0.0f ) );
	void	Render( void );

	//	各モード動作
	void	ModeFix( Vector3 target );
	void	ModeSlerp( Vector3 target );
	void	ModeIndividualSurveillance( Vector3 target );
	void	ModeChase( void );
	void	ModeResult( void );
	void	ModeTitle();

	//	動作関数
	void	Slerp( Vector3 target, float speed ) ;
	void	Shake( void );
	void	ShakeSet( float wide, int timer );
	void	SetNextPoint( int num, float speed );
	void	SpringMove( Vector3 position );

	//	情報取得
	Vector3	GetPos( void ){ return pos; }
	Vector3	GetTarget( void ){ return target; }
	Matrix	GetMatrix( void ){ return matView; }
	bool	GetMoveState( void )const{ return moveState; }
	float	GetPlayerAngle( int p_num ){ return playerAngle[p_num]; }
	
	//	数値計算
	void	CalcCameraPos( void );
	Vector3	CalcCenterPos( void );
	float	CalcMaxDist( void );
	Vector3	CheckPosLength( Vector3 position );
	void	CheckViewAngle( void );

	
	//	情報設定
	void	SetPos( Vector3 pos ){ this->pos = pos; }
	void	SetPos( float posx, float posy, float posz ){ this->pos = Vector3( posx, posy, posz); }
	void	SetPlayerInfo( const Vector3& p_1, const Vector3& p_2, const Vector3& p_3, const Vector3& p_4 );
};

//	バネ
class Rubber
{
public:
	//	質点パラメータ
	float mass;					//	質量
	Vector3 position;			//	位置
	Vector3 velocity;			//	速度
	bool	init_flag;

private:
	Vector3 acceleration;		//	加速度
	Vector3 resultant;			//	力

public:
	//	コンストラクタ・デストラクタ
	Rubber( void );
	~Rubber( void );

	//	更新
	void	Update( void );
	void	Integrate( float dt );

	//	情報設定・情報取得
	void	AddForce( const Vector3 &force ){	resultant += force;	}
	const	Vector3 &GetAcceleration() const{	return acceleration; }
};

//	メインカメラ
extern	Camera*		mainView;

//****************************************************************************************
#endif // !__CAMERA_H__
