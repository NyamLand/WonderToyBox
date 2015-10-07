
#ifndef		__CAMERA_H__
#define		__CAMERA_H__

//****************************************************************************************
//
//	Cameraクラス
//
//****************************************************************************************

namespace
{
	namespace VIEW_MODE
	{
		enum
		{
			SETUP,
			FIX,
			SLERP,
			CHASE,		//	※このモードの場合はSetTargetでtargetに値を与える必要があります。
			RESULT,
			TITLE,
		};
	}

	namespace TITLE_MOVE_INFO
	{
		float	height = 5.0f;
		Vector3	pos[6] =
		{
			Vector3( 8.0f, height, 0.0f ),		//門番
			Vector3( 45.0f, height, -14.0f ),	//	オプション
			Vector3( -41.0f, height, -20.0f ),	//	クレジット
			Vector3( -4.5f, height, 46.0f ),	//	キャラ選択
			Vector3( 49.0f, height, 69.0f ),	//	ステージ選択
			Vector3( 45.0f, height, 73.0f ),	//	確認
		};

		//	上に同じ
		Vector3	target[6] =
		{
			Vector3( 8.0f, 3.0f, 13.0f ),
			Vector3( 53.0f, 3.0f, -5.0f ),
			Vector3( -50.0f, 3.0f, -14.0f ),
			Vector3( -9.0f, 3.0f, 59.0f ),
			Vector3( 52.0f, 3.0f, 60.0f ),
			Vector3( 46.0f, 3.0f, 80.0f ),
		};
	}
	
	namespace TITLE_TARGET
	{
		enum
		{
			PLAYERNUMBER,
			OPTION,
			CREDIT,
			SELECTCHARACTER,
			SELECTSTAGE,
			CHECK,
			END,
		};
	}
}

class Camera : public iexView
{
private:
	//	定数
	static	const		int		MAX = 20;		//	カメラからターゲットの距離の最大
	static	const		int		MIN = 8;		//								最少

private:
	Vector3	playerPos[4];
	Vector3 target;
	Vector3	nextPoint;
	Vector3	nextTarget;
	Vector3	startPos;
	Vector3 pos;
	float		length;
	float		speed;
	float		t;
	bool		moveState;
	
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

	//	動作関数
	void	ModeFix( Vector3 target );
	void	ModeSlerp( Vector3 target );
	void	ModeChase( void );
	void	ModeResult( void );
	void	ModeTitle();
	void	Slerp( Vector3 target, float speed ) ;
	void	Shake( void );
	void	ShakeSet( float wide, int timer );
	void	SetNextPoint( int num, float speed );

	//	情報取得
	Vector3	GetPos( void ){ return pos; }
	Vector3	GetTarget( void ){ return target; }
	Matrix	GetMatrix( void ){ return matView; }
	bool		GetMoveState( void )const{ return moveState; }
	
	//	数値計算
	void	CalcCameraPos( void );
	Vector3	CalcCenterPos( void );
	float	CalcMaxDist( void );
	
	//	情報設定
	void	SetPos( Vector3 pos ){ this->pos = pos; }
	void	SetPos( float posx, float posy, float posz ){ this->pos = Vector3( posx, posy, posz); }
	void	SetPlayerInfo( const Vector3& p_1, const Vector3& p_2, const Vector3& p_3, const Vector3& p_4 );
};

extern	Camera*		m_Camera;

//****************************************************************************************
#endif // !__CAMERA_H__
