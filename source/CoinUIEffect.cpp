
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"system/System.h"
#include	"Particle.h"
#include	"Camera.h"
#include	"CoinUIEffect.h"

//**********************************************************************
//
//	CoinUIEffectクラス
//	
//**********************************************************************

//---------------------------------------------------------------------------------------
//	グローバル
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//	初期化・解放
//---------------------------------------------------------------------------------------

	//	コンストラクタ
	CoinUIEffect::CoinUIEffect( void ) : targetPar( nullptr ), camera( nullptr ),
		parPos( 0.0f, 0.0f, 0.0f ), posY( 0 ), scale( 0 )
	{
		FOR( 0, PLAYER_MAX )
		{
			coinFlag[value] = false;
			coinTimer[value] = 0;
			posX[value] = 0;
		}
	}

	//	デストラクタ
	CoinUIEffect::~CoinUIEffect( void )
	{
		Release();
	}

	//	初期化
	bool	CoinUIEffect::Initialize( void )
	{
		//	パーティクル用バッファ
		parPos = Vector3( 100.0f, 100.0f, 100.0f );
		targetPar = new iex2DObj( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET );
		camera = new Camera();
		camera->SetPos( Vector3( 0.0f, 10.0f, -10.0f ) + parPos );
		return	true;
	}

	//	解放
	void	CoinUIEffect::Release( void )
	{
		SafeDelete( camera );
		SafeDelete( targetPar );
	}

//---------------------------------------------------------------------------------------
//	更新・描画
//---------------------------------------------------------------------------------------

	//	更新
	void	CoinUIEffect::Update( void )
	{
		camera->Update( VIEW_MODE::FIX, parPos );
		particle->Update();

		FOR( 0, PLAYER_MAX )
		{
			//	パーティクルを30フレーム間だけ描画許可する
			if ( coinFlag[value] )
			{
				coinTimer[value]++;
				if ( coinTimer[value] > 30 )
				{
					coinFlag[value] = false;
					coinTimer[value] = 0;
				}
			}
		}
	}

	//	描画
	void	CoinUIEffect::Render( int player )
	{
		if ( !coinFlag[player] )	return;

		targetPar->Render( 
			static_cast<int>( posX[player] - scale * 1.5f  ), 
			static_cast<int>( posY - scale * 1.5f ),
			scale * 3, scale * 2,
			0, 0, 
			iexSystem::ScreenWidth, iexSystem::ScreenHeight,
			shader2D, "add" );
	}

	//	バックバッファに描画
	void	CoinUIEffect::RenderToBackBaffer( void )
	{
		//	レンダーターゲットにバックバッファを指定
		targetPar->RenderTarget( 0 );

		//	画面クリア
		camera->Activate();
		camera->Clear();

		//	パーティクル描画
		particle->Render();
	}

//---------------------------------------------------------------------------------------
//	情報設定
//---------------------------------------------------------------------------------------

	//	エフェクト描画フラグ設定( プレイヤー番号、trueで描画 )
	void	CoinUIEffect::SetEffectFlag( int playerNum, bool state )
	{
		coinFlag[playerNum] = true;
		particle->CoinGet( parPos, 0.5f );
	}

	//	座標設定
	void	CoinUIEffect::SetPos( int playerNum, int x, int y )
	{
		posX[playerNum] = x;
		posY = y;
	}

	//	スケール設定
	void	CoinUIEffect::SetScale( int scale )
	{
		this->scale = scale;
	}