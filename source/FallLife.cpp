
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Random.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"FallLife.h"

//**********************************************************************
//
//	FallLifeクラス
//
//**********************************************************************

//--------------------------------------------------------------------------------
//	グローバル
//--------------------------------------------------------------------------------
#define	MAX_LIFE		5
#define	FALL_SPEED		0.03f
#define	PERCENTAGE_MAX	1.0f
#define	MOVE_X	70
#define	MOVE_Y	-15
#define	SUB_MOVE_Y	1

//--------------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------------

	//	コンストラクタ
	FallLife::FallLife( void ) : orgLife( nullptr )
	{
		FOR( 0, PLAYER_MAX )
		{
			lifeImage[value].obj = nullptr;
			damageFlag[value] = false;
			param[value] = PERCENTAGE_MAX;
			fallDirection[value] = LEFT_FALL;
			savePos[value].x = 0;
			savePos[value].y = 0;
			moveY[value] = MOVE_Y;
		}
	}

	//	デストラクタ
	FallLife::~FallLife( void )
	{
		Release();
	}

	//	初期化
	bool	FallLife::Initialize( void )
	{
		//	読み込み
		Load();

		//	画像設定
		FOR( 0, PLAYER_MAX )
		{
			lifeImage[value].obj = orgLife;
			ImageInitialize( lifeImage[value], 0, 0, 75, 75, 0, 0, 64, 64 );
			lifeImage[value].renderflag = false;
			if ( lifeImage[value].obj == nullptr )	return	false;
		}

		return	true;
	}

	//	解放
	void	FallLife::Release( void )
	{
		SafeDelete( orgLife );
	}

	//	読み込み
	void	FallLife::Load( void )
	{
		orgLife = new iex2DObj( "DATA/UI/NLife.png" );
	}

//--------------------------------------------------------------------------------
//	更新・描画
//--------------------------------------------------------------------------------

	//	更新
	void	FallLife::Update( void )
	{
		bool	active = false;

		FOR( 0, PLAYER_MAX )
		{
			//	パラメータ加算( フラグ立ってなかったらとばす )
			active = AddPercentage( value );
			if ( !active )	continue;

			//	落下
			Fall( value );

			//	透明度更新
			AlphaUpdate( value );

			//	向き更新
			AngleUpdate( value );
		}
	}

	//	描画
	void	FallLife::Render( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			//	ダメージ受けてないとき描画しない
			if ( !damageFlag[value] )	continue;

			//	画像描画
			RenderImage( lifeImage[value], 
				lifeImage[value].sx, lifeImage[value].sy,
				lifeImage[value].sw, lifeImage[value].sh,
				IMAGE_MODE::ADOPTPARAM );
		}
	}

//--------------------------------------------------------------------------------
//	動作関数
//--------------------------------------------------------------------------------

	//	割合加算( プレイヤー番号 ) : paramを加算、100%で止める、フラグが立ってなかったらfalse
	bool	FallLife::AddPercentage( int playerNum )
	{
		if ( !damageFlag[playerNum] )		return false;

		//	パラメータ加算
		param[playerNum] += FALL_SPEED;
		if ( param[playerNum] >= PERCENTAGE_MAX )
		{
			param[playerNum] = PERCENTAGE_MAX;
			damageFlag[playerNum] = false;
			lifeImage[playerNum].renderflag = false;
			lifeImage[playerNum].angle = 0.0f;
		}

		return	true;
	}

	//	落下　座標移動
	void	FallLife::Fall( int playerNum )
	{
		//	X座標移動補間
		Lerp( lifeImage[playerNum].x, 
			savePos[playerNum].x, 
			savePos[playerNum].x + MOVE_X * fallDirection[playerNum], 
			GetBezier( ePrm_t::eSlow_Lv3, ePrm_t::eSlow_Lv1, param[playerNum] ) );

		//	Y座標移動値減算
		moveY[playerNum] += SUB_MOVE_Y;

		//	Y座標加算
		lifeImage[playerNum].y += moveY[playerNum];
	}

	//	透明度更新
	void	FallLife::AlphaUpdate( int playerNum )
	{
		//	補間
		Lerp( 
			lifeImage[playerNum].alpha,
			1.0f, 0.0f, 
			GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5,param[playerNum] ) );
	}

	//	向き更新
	void	FallLife::AngleUpdate( int playerNum )
	{
		//	補間
		CubicFunctionInterpolation(
			lifeImage[playerNum].angle,
			0.0f, 
			( D3DX_PI  * 0.25f ) * fallDirection[playerNum],
			param[playerNum] );

		//	回転中心座標設定
		lifeImage[playerNum].p = GetPoint( lifeImage[playerNum].x, lifeImage[playerNum].y );
	}

//--------------------------------------------------------------------------------
//	情報設定
//--------------------------------------------------------------------------------

	//	座標設定( プレイヤー番号、X座標、Y座標 )
	void	FallLife::SetPos( int playerNum, int posX, int posY )
	{
		savePos[playerNum].x = posX;
		savePos[playerNum].y = posY;
	}

	//	読み込み位置設定
	void	FallLife::SetSrcPos( int playerNum, int sx, int sy )
	{
		lifeImage[playerNum].sx = sx;
		lifeImage[playerNum].sy = sy;
	}

	//	ダメージフラグ設定( プレイヤー番号、trueでライフ落下演出リセット、減る前のライフ )
	void	FallLife::SetDamageFlag( int playerNum, bool flag, int culLife )
	{
		damageFlag[playerNum] = flag;
		lifeImage[playerNum].renderflag = true;
		param[playerNum] = 0.0f;
		lifeImage[playerNum].y = savePos[playerNum].y;
		moveY[playerNum] = MOVE_Y;
		
		//	落下向き設定(ランダム)
		if ( Random::PercentageRandom( 0.5f ) )	fallDirection[playerNum] = FALL_DIRECTION::LEFT_FALL;
		else																fallDirection[playerNum] = FALL_DIRECTION::RIGHT_FALL;

		//	読み込み位置設定
		SetSrcPos( 
			playerNum,
			lifeImage[playerNum].sw * ( ( MAX_LIFE - culLife ) % 4 ),
			lifeImage[playerNum].sh * ( ( MAX_LIFE- culLife ) / 4 ) );
	}

//--------------------------------------------------------------------------------
//	情報取得
//--------------------------------------------------------------------------------
