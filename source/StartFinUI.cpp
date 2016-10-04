
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"system/System.h"
#include	"Particle.h"
#include	"Camera.h"
#include	"StartFinUI.h"
#include	"Sound.h"


//**********************************************************************
//
//	CoinUIEffectクラス
//	
//**********************************************************************

//---------------------------------------------------------------------------------------
//	グローバル
//---------------------------------------------------------------------------------------
#define	FIN_DELAY	2

//---------------------------------------------------------------------------------------
//	初期化・解放
//---------------------------------------------------------------------------------------
	
	//	コンストラクタ
	StartFinUI::StartFinUI( void ) : count( 0 ), waitTimer( 0 ), s_t( 0 ), s_step( 0 ), changeflag( false )
	{
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.27f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.49f );
		ImageInitialize( s_obj, x, y, w, h, 0, 0, 512, 512 );
		w = static_cast<int>( iexSystem::ScreenWidth * 0.49f );
		h = static_cast<int>( iexSystem::ScreenHeight * 0.27f );
		ImageInitialize( f_obj, x, y, w, h, 0, 512, 1024, 512 );

		s_obj.obj = new iex2DObj( "DATA/UI/bfUI_02.png" );
		f_obj.obj = new iex2DObj( "DATA/UI/bfUI.png" );

		s_pos = Vector3( ( float )s_obj.x, -( ( float )s_obj.h / 2.0f ), 0.0f );
		f_pos = Vector3( ( float )s_obj.x, ( float )s_obj.y, 0.0f );
	}

	//	デストラクタ
	StartFinUI::~StartFinUI( void )
	{
		SafeDelete( s_obj.obj );
		SafeDelete( f_obj.obj );
	}


//---------------------------------------------------------------------------------------
//	更新・描画
//---------------------------------------------------------------------------------------
	void	StartFinUI::StartUpdate( void )
	{
		int round = gameManager->GetRound();
		static int	delay;
		switch ( s_step )
		{
			//	画面上部から移動
		case 0:
			if ( StartMove() )
			{
				SetScaling( s_obj, 4.0f );
				s_step++;
				break;
			}
			break;
			//	着地しながらフェードアウト
		case 1:
			if ( !s_obj.scalingFlag )
			{
				s_obj.alpha -= 1.0f / 60.0f;
				if ( s_obj.alpha <= 0.0f )
				{
					s_obj.sx = 512;
					s_obj.sy = 512;
					SetScaling( s_obj, 3.0f, false );
					s_step++;
					break;
				}
			}
			ScalingLandingUpdate( s_obj, 100 );
			break;
			//	GOがフェードイン
		case 2:
			sound->PlaySE(SE::GAMESTART_SE );
			if ( !s_obj.scalingFlag )
			{
				delay = 90;		//	文字停止フレーム数
				s_step++;
				break;
			}
			ScalingAlphaUpdate( s_obj, 300 );
			break;
			//	文字停止フレーム数後、Goがフェードアウト
		case 3:
			//	タイマーの間はbreak;
			delay--;
			if ( delay >= 0 )	break;

			changeflag = true;
			waitTimer = FIN_DELAY * SECOND;
			break;
		}

		//	FIGHT描画のためスキップ
		if (s_step > 1) return;
		//	読み込み位置・サイズ設定
		switch (round)
		{
		case Round::ROUND1:
			s_obj.sx = 0;
			s_obj.sy = 0;

			break;

		case Round::ROUND2:
			s_obj.sx = 512;
			s_obj.sy = 0;
			break;

		case Round::ROUND_FINAL:
			s_obj.sx = 0;
			s_obj.sy = 512;
			break;
		}

	}

	//	ラウンド文字移動
	bool	StartFinUI::StartMove( void )
	{
		Vector3 pos;

		//	パラメータ加算
		s_t += 1.0f / 30.0f;

		if ( s_t >= 1.0f )
		{
			return true;
		}

		Lerp( pos, s_pos, f_pos, s_t );
		s_obj.x = ( int )pos.x;
		s_obj.y = ( int )pos.y;

		if ( s_t >= 1.0f ) s_t = 1.0f;

		return false;
	}

	//	タイムアップ演出
	void	StartFinUI::FinishUpdate( void )
	{
		waitTimer--;

		if ( waitTimer <= 0 )	changeflag = true;
	}


//---------------------------------------------------------------------------------------
//	メイン描画
//---------------------------------------------------------------------------------------

	//	開始描画
	void	StartFinUI::StartRender( void )
	{
		RenderImage( s_obj, s_obj.sx, s_obj.sy, s_obj.sw, s_obj.sh, IMAGE_MODE::SCALING );
	}

	//	終了描画
	void	StartFinUI::FinishRender( void )
	{
		RenderImage( f_obj, f_obj.sx, f_obj.sy, f_obj.sw, f_obj.sh, IMAGE_MODE::NORMAL );
	}


//------------------------------------------------------------------------------
//	情報設定・取得
//------------------------------------------------------------------------------
	
	//	モード変更フラグ設定
	void	StartFinUI::SetChangeFlag( const bool& flag )
	{
		changeflag = flag;
	}

	//	モード変更フラグ取得
	bool	StartFinUI::GetChangeFlag( void )
	{
		bool	out = this->changeflag;
		return	out;
	}
