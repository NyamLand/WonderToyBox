
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

	//	初期化
	bool	Screen::Initialize( void )
	{
		mode = SCREEN_MODE::FADE_IN;
		color = Color::BLACK;
		screenState = false;
		alpha = 1.0f;
		speed = 1.0f;
		size = Size::MIN;
		wipeSize = 1.0f;
		param = 0.0f;

		shader3D->SetValue( "screen_width", static_cast<float>( iexSystem::ScreenWidth ) );
		shader3D->SetValue( "screen_height", static_cast<float>( iexSystem::ScreenHeight ) );

		return	false;
	}

	//	解放
	void	Screen::Release( void )
	{

	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	更新
	bool	Screen::Update( void )
	{
		screenState = false;

		switch ( mode )
		{
		case SCREEN_MODE::FADE_IN:
		case SCREEN_MODE::WHITE_IN:
			screenState = FadeIn();
			break;

		case SCREEN_MODE::FADE_OUT:
		case SCREEN_MODE::WHITE_OUT:
			screenState = FadeOut();
			break;

		case SCREEN_MODE::WIPE_IN:
			screenState = WipeIn();
			break;

		case SCREEN_MODE::WIPE_OUT:
			screenState = WipeOut();
			break;
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

	//	フェードイン
	bool	Screen::FadeIn( void )
	{
		//	パラメータ更新
		param += 0.01f * speed;
		if ( param >= 1.0f )		param = 1.0f;

		//	補間
		bool isEnd = CubicFunctionInterpolation( alpha, 1.0f, 0.0f, param );
	
		//	終了処理
		if ( isEnd )
		{
			screenState = true;
			return true;
		}
		return	false;
	}

	//	フェードアウト
	bool	Screen::FadeOut( void )
	{
		//	パラメータ更新
		param += 0.01f * speed;
		if ( param >= 1.0f )	param = 1.0f;

		//	補間
		bool isEnd = CubicFunctionInterpolation( alpha, 0.0f, 1.0f, param );

		//	終了処理
		if ( isEnd )
		{
			screenState = true;
			return true;
		}
		return	false;
	}

	//	ワイプアウト
	bool	Screen::WipeOut( void )
	{
		//	パラメータ更新
		param += 0.01f * speed;
		if ( param >= 1.0f )	param = 1.0f;

		//	補間
		bool isEnd = CubicFunctionInterpolation( wipeSize, 1.0f, 0.0f, param );
		shader3D->SetValue( "effect_size", 1000.0f * wipeSize );

		//	終了処理
		if ( isEnd )
		{
			screenState = true;
			return true;
		}
		return	false;
	}

	//	ワイプイン
	bool	Screen::WipeIn( void )
	{
		//	パラメータ更新
		param += 0.01f * speed;
		if ( param >= 1.0f )	param = 1.0f;

		//	補間
		bool isEnd = CubicFunctionInterpolation( wipeSize, 0.0f, 1.0f, param );
		shader3D->SetValue( "effect_size", 1000.0f * wipeSize );

		//	終了処理
		if ( isEnd )
		{
			screenState = true;
			return true;
		}
		return	false;
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

		case SCREEN_MODE::WIPE_OUT:
		case SCREEN_MODE::WIPE_IN:
			break;
		}
	}

//-----------------------------------------------------------------------------------
//	情報取得
//-----------------------------------------------------------------------------------

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