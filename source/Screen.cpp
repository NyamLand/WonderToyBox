
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Screen.h"
#include	"system\System.h"

//*******************************************************************************
//
//	Screenクラス
//
//*******************************************************************************

//-----------------------------------------------------------------------------------
//	グローバル
//-----------------------------------------------------------------------------------

#define	WIPE_MAX_SIZE	1000.0f
#define	ADVISORY_SPEED	0.01f		//	推奨速度

	namespace
	{
		namespace Color
		{
			const Vector3 WHITE( 1.0f, 1.0f, 1.0f );
			const Vector3 BLACK( 0.0f, 0.0f, 0.0f );
		}
		namespace Size
		{
			const int MAX = 255;
			const int MIN = 0;
		}
	}

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	Screen::Screen( void ) : color( Color::BLACK ),
		alpha( 1.0f ), speed( 1.0f ), wipeSize( 1.0f ), param( 0.0f ),
		screenState( false ),
		mode( SCREEN_MODE::FADE_IN ), size( Size::MIN )
	{
		//	スクリーンサイズを設定
		width = static_cast<float>( iexSystem::ScreenWidth );
		height = static_cast<float>( iexSystem::ScreenHeight );
		widthHalf = width * 0.5f;
		heightHalf = height * 0.5f;

		//	各補間初期値、最終値設定
		startParam[SCREEN_MODE::FADE_IN]		=		startParam[SCREEN_MODE::WHITE_IN]		=		startParam[SCREEN_MODE::WIPE_OUT] = 1.0f;
		startParam[SCREEN_MODE::FADE_OUT]	=		startParam[SCREEN_MODE::WHITE_OUT]	=		startParam[SCREEN_MODE::WIPE_IN] = 0.0f;
		endParam[SCREEN_MODE::FADE_OUT]		=		endParam[SCREEN_MODE::WHITE_OUT]	=		endParam[SCREEN_MODE::WIPE_IN] = 1.0f;
		endParam[SCREEN_MODE::FADE_IN]			=		endParam[SCREEN_MODE::WHITE_IN]		=		endParam[SCREEN_MODE::WIPE_OUT] = 0.0f;

		shader3D->SetValue( "screen_width", width );
		shader3D->SetValue( "screen_height", height );
	}

	//	デストラクタ
	Screen::~Screen( void )
	{

	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	更新
	bool	Screen::Update( void )
	{
		//	補間割合加算
		PercentageUpdate( param, ADVISORY_SPEED * speed );

		//	スクリーン状態初期化
		screenState = false;

		switch ( mode )
		{
		case SCREEN_MODE::FADE_IN:
		case SCREEN_MODE::WHITE_IN:
		case SCREEN_MODE::FADE_OUT:
		case SCREEN_MODE::WHITE_OUT:
			screenState = Fade();
			break;
		case SCREEN_MODE::WIPE_IN:
		case SCREEN_MODE::WIPE_OUT:
			screenState = Wipe();
		}

		return	screenState;
	}

	//	描画
	void	Screen::Render( void )
	{
		switch ( mode )
		{
		case SCREEN_MODE::WIPE_IN:
		case SCREEN_MODE::WIPE_OUT:
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shader3D, "WipeEffect", 0xFFFFFFFF );
			break;

		default:
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, GetColor( color, alpha ) );
			break;
		}
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	フェード処理
	bool	Screen::Fade( void )
	{
		//	補間
		bool	isEnd = CubicFunctionInterpolation( alpha, startParam[mode], endParam[mode], param );

		return	isEnd;
	}

	//	ワイプ処理
	bool	Screen::Wipe( void )
	{
		//	補間
		bool isEnd = CubicFunctionInterpolation( wipeSize, startParam[mode], endParam[mode], param );

		//	シェーダーに値セット
		shader3D->SetValue( "effect_size", WIPE_MAX_SIZE * wipeSize );
		return	isEnd;
	}

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

	//	スクリーンモード
	void	Screen::SetScreenMode( int mode, float speed )
	{
		this->mode = mode;
		this->speed = speed;
		this->screenState = false;
		param = 0.0f;
		switch ( this->mode )
		{
		case SCREEN_MODE::FADE_IN:
		case SCREEN_MODE::FADE_OUT:
			color = Color::BLACK;
			break;

		case SCREEN_MODE::WHITE_IN:
		case SCREEN_MODE::WHITE_OUT:
			color = Color::WHITE;
			break;

		default:
			break;
		}
	}

//-----------------------------------------------------------------------------------
//	情報取得
//-----------------------------------------------------------------------------------

	//	スクリーン幅をfloatで取得
	float	Screen::GetWidthFloat( void )const
	{
		return	width;
	}

	//	スクリーン幅の半分をfloatで取得
	float	Screen::GetWidthHalfFloat( void )const
	{
		return	widthHalf;
	}

	//	スクリーン高さをfloatで取得
	float	Screen::GetHeightFloat( void )const
	{
		return	height;
	}

	//	スクリーン高さの半分をfloatで取得
	float	Screen::GetHeightHalfFloat( void )const
	{
		return	heightHalf;
	}

	//	スクリーン幅をintで取得
	int	Screen::GetWidthInt( void )const
	{
		return	static_cast<int>( width );
	}

	//	スクリーン幅の半分をintで取得
	int	Screen::GetWidthHalfInt( void )const
	{
		return	static_cast<int>( widthHalf );
	}

	//	スクリーン高さをintで取得
	int	Screen::GetHeightInt( void )const
	{
		return	static_cast<int>( height );
	}

	//	スクリーン高さの半分をintで取得
	int	Screen::GetHeightHalfInt( void )const
	{
		return	static_cast<int>( heightHalf );
	}

	//	スクリーン状態取得
	bool	Screen::GetScreenState( void )const
	{
		return	screenState;
	}

	//	実体取得
	Screen*	Screen::GetInstance( void )
	{
		static	Screen	out;
		return	&out;
	}