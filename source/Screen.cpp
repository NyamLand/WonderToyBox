
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
			iexPolygon::Rect( 0, 0, 1280, 720, RS_COPY, GetColor( color, alpha ) );
			break;
		}
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	フェードイン
	bool	Screen::FadeIn( void )
	{
		alpha -= D3DX_PI / 180.0f * speed;
		
		if ( alpha <= 0.0f ){
			alpha = 0.0f;
			screenState = true;
			return true;
		}
		
		return	false;
	}

	//	フェードアウト
	bool	Screen::FadeOut( void )
	{
		if ( alpha >= 1.0f ){
			alpha = 1.0f;
			screenState = true;
			return true;
		}

		alpha += D3DX_PI / 180.0f * speed;

		return	false;
	}

	//	ワイプアウト
	bool	Screen::WipeOut( void )
	{
		if ( wipeSize < 0.0f ){
			wipeSize = 0.0f;
			screenState = true;
			return true;
		}

		wipeSize -= D3DX_PI / 180.0f * speed;
		shader3D->SetValue( "effect_size", 1000.0f * wipeSize );
		return false;
	}

	//	ワイプイン
	bool	Screen::WipeIn( void )
	{
		if ( wipeSize >= 1.0f ){
			wipeSize = 1.0f;
			screenState = true;
			return true;
		}

		wipeSize += D3DX_PI / 180.0f * speed;
		shader3D->SetValue( "effect_size", 1000.0f * wipeSize );
		return false;
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

		switch ( this->mode )
		{
		case SCREEN_MODE::FADE_IN:
			color = Color::BLACK;
			alpha = 1.0f;
			break;

		case SCREEN_MODE::FADE_OUT:
			color = Color::BLACK;
			alpha = 0.0f;
			break;

		case SCREEN_MODE::WHITE_IN:
			color = Color::WHITE;
			alpha = 1.0f;
			break;

		case SCREEN_MODE::WHITE_OUT:
			color = Color::WHITE;
			alpha = 0.0f;
			break;

		case SCREEN_MODE::WIPE_OUT:
			color = Color::BLACK;
			alpha = 1.0f;
			wipeSize = 1.0f;
			break;

		case SCREEN_MODE::WIPE_IN:
			color = Color::BLACK;
			alpha = 1.0f;
			wipeSize = 0.0f;
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