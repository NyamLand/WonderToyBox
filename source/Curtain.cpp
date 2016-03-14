
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Curtain.h"

//**********************************************************************
//
//	Curtainクラス
//
//**********************************************************************

#define	VERTEX_MAX	4

//-----------------------------------------------------------------------------
//	初期化・開放
//-----------------------------------------------------------------------------

	//	コンストラクタ
	Curtain::Curtain( void ) : mode( CURTAIN_MODE::CLOSE ), speed( 0.001f ), param( 1.0f )
	{
		//	画像読み込み
		Load();

		//	パラメータ初期化
		curtainL.t = param;
		curtainR.t = param;

		//	頂点座標設定
		StartPosInitialize();
		EndPosInitialize();

		//	頂点設定
		FOR( 0, VERTEX_MAX )
		{
			SetVertex( curtainL.tlv[value], 
				startPosL[CLOSE][value],
				( float )( iexSystem::ScreenHeight * ( value / 2 ) ), 
				0.0f, 
				float( value % 2 ), 
				float( value / 2 ), 
				0xFFFFFFFF );

			SetVertex( curtainR.tlv[value], 
				startPosR[CLOSE][value], 
				( float )( iexSystem::ScreenHeight * ( value / 2 ) ), 
				0.0f, 
				float( value % 2 ), 
				float( value / 2 ), 
				0xFFFFFFFF );
		}

		//	更新
		Update();
	}

	//	デストラクタ
	Curtain::~Curtain( void )
	{
		SafeDelete( curtainR.obj );
		SafeDelete( curtainL.obj );
	}

	//	画像読み込み
	void	Curtain::Load( void )
	{
		//	画像読み込み
		curtainL.obj = new iex2DObj( "DATA/UI/title/curtain1.png" );
		curtainR.obj = new iex2DObj( "DATA/UI/title/curtain2.png" );
	}

	//	始点設定
	void	Curtain::StartPosInitialize( void ) 
	{		
		//	Open時初期位置設定
		startPosL[OPEN][0] = 0;																					startPosL[OPEN][1] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
		startPosL[OPEN][2] = 0;																					startPosL[OPEN][3] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
		startPosR[OPEN][0] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );	startPosR[OPEN][1] = static_cast<float>( iexSystem::ScreenWidth );
		startPosR[OPEN][2] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );	startPosR[OPEN][3] = static_cast<float>( iexSystem::ScreenWidth );

		//	Close時初期位置設定
		startPosL[CLOSE][0] = -( static_cast<float>( iexSystem::ScreenWidth * 0.5f ) );		startPosL[CLOSE][1] = 0;
		startPosL[CLOSE][2] = -( static_cast<float>( iexSystem::ScreenWidth * 0.5f ) );		startPosL[CLOSE][3] = 0;
		startPosR[CLOSE][0] = static_cast<float>( iexSystem::ScreenWidth );						startPosR[CLOSE][1] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
		startPosR[CLOSE][2] = static_cast<float>( iexSystem::ScreenWidth );						startPosR[CLOSE][3] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
	}

	//	終点設定
	void	Curtain::EndPosInitialize( void )
	{
		//	Open時初期位置設定
		endPosL[OPEN][0] = -( static_cast<float>( iexSystem::ScreenWidth * 0.5f ) );	endPosL[OPEN][1] = 0;
		endPosL[OPEN][2] = -( static_cast<float>( iexSystem::ScreenWidth * 0.5f ) );	endPosL[OPEN][3] = 0;
		endPosR[OPEN][0] = static_cast<float>( iexSystem::ScreenWidth );					endPosR[OPEN][1] = static_cast<float>( iexSystem::ScreenWidth * 1.5f );
		endPosR[OPEN][2] = static_cast<float>( iexSystem::ScreenWidth );					endPosR[OPEN][3] = static_cast<float>( iexSystem::ScreenWidth * 1.5f );

		//	Close時初期位置設定
		endPosL[CLOSE][0] = 0;																					endPosL[CLOSE][1] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
		endPosL[CLOSE][2] = 0;																					endPosL[CLOSE][3] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );
		endPosR[CLOSE][0] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );		endPosR[CLOSE][1] = static_cast<float>( iexSystem::ScreenWidth );
		endPosR[CLOSE][2] = static_cast<float>( iexSystem::ScreenWidth * 0.5f );		endPosR[CLOSE][3] = static_cast<float>( iexSystem::ScreenWidth );
	}

//-----------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------

	//	更新
	void	Curtain::Update( void )
	{
		//	パラメータ加算
		isEnd = PercentageUpdate( param, speed );
		
		//	パラメータ適用
		curtainL.t = param;
		curtainR.t = param;

		switch ( mode )
		{
		case CURTAIN_MODE::OPEN:
			Open();
			break;
			
		case CURTAIN_MODE::CLOSE:
			Close();
			break;
		}
	}

	//	描画
	void	Curtain::Render( void )
	{
		iexPolygon::Render2D( curtainL.tlv, 2, curtainL.obj, RS_COPY );
		iexPolygon::Render2D( curtainR.tlv, 2, curtainR.obj, RS_COPY );
	}

//-----------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------

	//	開く　各頂点移動
	void	Curtain::Open( void )
	{
		int speedMode = 0;
		FOR( 0, VERTEX_MAX )
		{
			if ( value / 2 == 0 )	speedMode = ePrm_t::eSlow_Lv1;
			else							speedMode = ePrm_t::eSlow_Lv5;
			Lerp( curtainL.tlv[value].sx, startPosL[OPEN][value], endPosL[OPEN][value], GetBezier( speedMode, speedMode, curtainL.t ) );
			Lerp( curtainR.tlv[value].sx, startPosR[OPEN][value], endPosR[OPEN][value], GetBezier(speedMode, speedMode, curtainR.t ) );
		}
	}

	//	閉じる	各頂点移動
	void	Curtain::Close( void )
	{
		FOR( 0, VERTEX_MAX )
		{
			Lerp( curtainL.tlv[value].sx, startPosL[CLOSE][value], endPosL[CLOSE][value], GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtainL.t ) );
			Lerp( curtainR.tlv[value].sx, startPosR[CLOSE][value], endPosR[CLOSE][value], GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtainR.t ) );
		}
	}

//-----------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------

	//	モード設定
	void	Curtain::SetMode( CURTAIN_MODE mode, float speed )
	{
		this->mode = mode;
		this->speed = speed;
		this->param = 0.0f;
		isEnd = false;
	}

//-----------------------------------------------------------------------------
//	情報取得
//-----------------------------------------------------------------------------	

	//	動作終了フラグ取得
	bool	Curtain::GetIsEnd( void )const
	{
		return	isEnd;
	}