
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Sound.h"
#include	"Screen.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"Random.h"
#include	"BaseChara.h"
#include	"CharacterManager.h"
#include	"Curtain.h"
#include	"sceneMain.h"
#include	"sceneMenu.h"
#include	"sceneLoad.h"

#include	"sceneTitle.h"

//*****************************************************************************
//
//	sceneTitleクラス
//
//*****************************************************************************

//-----------------------------------------------------------------------------------
//	グローバル
//-----------------------------------------------------------------------------------

namespace
{
	namespace TITLE_STEP
	{
		enum
		{
			WAIT_SCREEN,
			WAIT_INPUT,
			WAIT_OPEN,
			NEXT_MODE,
		};
	}
}

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	sceneTitle::sceneTitle( void )	: mode( TITLE_MODE::TITLE )
	{

	}
	
	//	デストラクタ
	sceneTitle::~sceneTitle( void )
	{
		Release();
	}
	
	//	初期化
	bool	sceneTitle::Initialize( void )
	{
		//	環境設定
		iexLight::SetAmbient( 0x404040 );
		iexLight::SetFog( 800, 1000, 0 );

		//	ライト設定
		Vector3 dir( 0.0f, -1.0f, -0.0f );
		dir.Normalize();
		iexLight::DirLight( shader3D, 0, &dir, 1.0f, 1.0f, 1.0f );

		//	読み込み
		Load();

		//	カメラ設定
		mainView = new Camera();

		//	カーテン初期化
		curtain = new Curtain();

		//	マネージャー初期化
		gameManager->Initialize();

		//	パーティクル初期化
		particle->Initialize();

		//	スクリーン初期化
		screen->SetScreenMode( SCREEN_MODE::FADE_IN, 1.0f );

		//	ステージ	
		stage->SetAngle( D3DX_PI );
		stage->SetScale( 0.1f );
		stage->Update();

		//	乱数初期化
		Random::Initialize();		

		//	各モード初期化
		TitleInitialize();
		MenuInitialize();
		CreditInitialize();

		//	BGM設定再生
		sound->PlayBGM( BGM::TITLE_BGM );
		return	true;
	}

	//	タイトル初期化
	void	sceneTitle::TitleInitialize( void )
	{
		//	画像構造体初期化
		ImageInitialize( titleInfo.pressSpace, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.82f ), static_cast<int>( iexSystem::ScreenWidth * 0.2f ), static_cast<int>( iexSystem::ScreenHeight * 0.15f ), 0, 0, 256, 128 );
		ImageInitialize( titleInfo.titleImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.42f ), static_cast<int>( iexSystem::ScreenWidth * 0.4f ), static_cast<int>( iexSystem::ScreenHeight * 0.7f ), 0, 0, 512, 512 );
		ImageInitialize( titleInfo.gameStartImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.80f ), static_cast<int>( iexSystem::ScreenWidth * 0.15f ), static_cast<int>( iexSystem::ScreenHeight * 0.07f ), 0, 0, 512, 128 );
		ImageInitialize( titleInfo.creditTextImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.87f ), static_cast<int>( iexSystem::ScreenWidth * 0.1f ), static_cast<int>( iexSystem::ScreenHeight * 0.07f ), 0, 128, 512, 128 );
		titleInfo.gameStartImage.obj = titleInfo.titleText;
		titleInfo.creditTextImage.obj = titleInfo.titleText;

		//	カメラ情報初期化
		cameraInfo.state = 0;

		//	変数初期化
		titleInfo.titleImage.renderflag = true;
		titleInfo.step = 0;
		titleInfo.nextMode = 0;
		titleInfo.changeflag = false;
		titleInfo.curtainState = false;
		titleInfo.speed = 0.5f;
	}

	//	メニュー初期化
	void	sceneTitle::MenuInitialize( void )
	{
		menuInfo.menu_num = 0;
	}

	//	クレジット初期化
	void	sceneTitle::CreditInitialize( void )
	{		
		creditInfo.sy = 0;
		creditInfo.t = 0.0f;
	}

	//	読み込み
	bool	sceneTitle::Load( void )
	{
		//	ステージ
		PointerNew( stage, "DATA/BG/title_stageV2/title_stagev2.IMO" );

		//	タイトル画像
		PointerNew( titleInfo.titleImage.obj, "DATA/UI/title.png" );
		PointerNew( titleInfo.pressSpace.obj, "DATA/UI/pressspace.png" );
		PointerNew( titleInfo.titleText, "DATA/UI/title/titleText.png" );

		//	クレジット用画像
		PointerNew( creditInfo.credit, "DATA/UI/title/credit.png" );

		return	true;
	}

	//	解放
	void	sceneTitle::Release( void )
	{
		SafeDelete( curtain );
		SafeDelete( mainView );
		SafeDelete( titleInfo.titleImage.obj );
		SafeDelete( titleInfo.pressSpace.obj );
		SafeDelete( stage );
		SafeDelete( titleInfo.titleText );
		SafeDelete( creditInfo.credit );
		sound->AllStop();
	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	更新
	void	sceneTitle::Update( void )
	{
		curtain->Update();

		//	各モード動作
		switch ( mode )
		{
		case TITLE_MODE::TITLE:
			TitleUpdate();
			break;
		
		case TITLE_MODE::MENU:
			MenuUpdate();
			break;

		case TITLE_MODE::PLAY:
			MoveSceneUpdate();
			break;

		case TITLE_MODE::CREDIT:
			CreditUpdate();
			break;
		}

		//	ターゲット位置にパーティクル配置
		for ( int i = 0; i < TITLE_TARGET::END; i++)	particle->BlueFlame( TITLE_MOVE_INFO::target[i] );
		particle->Update();
		
		//	スクリーン更新
		screen->Update();
	}

	//	描画
	void	sceneTitle::Render( void )
	{
		//	画面クリア
		mainView->Activate();
		mainView->Clear( 0xFF02B4FE );

		//	ステージ描画
		stage->Render();

		//	カーテン描画
		curtain->Render();

		//	パーティクル描画
		//particle->Render();

		switch ( mode )
		{
		case TITLE_MODE::TITLE:
			TitleRender();
			break;

		case TITLE_MODE::MENU:
			MenuRender();
			break;

		case TITLE_MODE::PLAY:
			MoveSceneRender();
			break;

		case TITLE_MODE::CREDIT:
			CreditRender();
			break;
		}

		//	スクリーン描画
		screen->Render();
	}

//******************************************************************
//　各画面のメソッド
//******************************************************************

	//--------------------------------------------------------
	//	タイトル
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::TitleUpdate( void )
		{
			static	float flashingSpeed = D3DX_PI / 180 * 10.0f;
			switch ( titleInfo.step )
			{
			case TITLE_STEP::WAIT_SCREEN:
				//	フェード処理終了まで待つ
				if ( screen->GetScreenState() )	titleInfo.step++;
				break;

			case TITLE_STEP::WAIT_INPUT:
				//	カメラ更新
				mainView->Update( VIEW_MODE::SETUP );

				//	点滅更新
				FlashingUpdate( titleInfo.pressSpace, D3DX_PI / 180 * 4.0f );

				//	カーソル移動
				if ( input[0]->Get( KEY_UP ) == 3 )
				{
					titleInfo.nextMode--;
					if ( titleInfo.nextMode < 0 )	titleInfo.nextMode = 1;
				}
				if ( input[0]->Get( KEY_DOWN ) == 3 )
				{
					titleInfo.nextMode++;
					if ( titleInfo.nextMode >= 2 )	titleInfo.nextMode = 0;
				}

				//	点滅更新
				switch ( titleInfo.nextMode )
				{
				case 0:
					FlashingUpdate( titleInfo.gameStartImage, D3DX_PI / 180 * 4.0f );
					titleInfo.creditTextImage.flashingAlpha = 1.0f;
					break;

				case 1:
					FlashingUpdate( titleInfo.creditTextImage, D3DX_PI / 180 * 4.0f );
					titleInfo.gameStartImage.flashingAlpha = 1.0f;
					break;
				}
				
				//	SPACEキーで選択
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
				{
					//	タイトル画像を隠す
					titleInfo.titleImage.renderflag = false;

					//	決定音鳴らす
					sound->PlaySE( SE::DECIDE_SE );

					//	画面制御
					screen->SetScreenMode( SCREEN_MODE::WHITE_OUT, 1.0f );

					//	カーテンモード設定
					curtain->SetMode( Curtain::OPEN );

					//	pressspace波紋
					static	float	wavespeed = 1.5f;
					switch ( titleInfo.nextMode )
					{
					case 0:	SetWave( titleInfo.gameStartImage, wavespeed );
						break;
					case 1:	SetWave( titleInfo.creditTextImage, wavespeed );
						break;
					}

					//	つぎのステップ
					titleInfo.step++;
				}
				break;

			case TITLE_STEP::WAIT_OPEN:
				//	カメラ更新
				mainView->Update( VIEW_MODE::TITLE );

				//	波紋更新
				WaveUpdate( titleInfo.gameStartImage );
				WaveUpdate( titleInfo.creditTextImage );

				//	点滅更新
				switch ( titleInfo.nextMode )
				{
				case 0:
					FlashingUpdate( titleInfo.gameStartImage, flashingSpeed );
					titleInfo.creditTextImage.flashingAlpha = 1.0f;
					break;

				case 1:
					FlashingUpdate( titleInfo.creditTextImage, flashingSpeed );
					titleInfo.gameStartImage.flashingAlpha = 1.0f;
					break;
				}

				//	動作済みで次のステップへ
				if ( curtain->GetIsEnd() )	titleInfo.step++;
				break;

			case TITLE_STEP::NEXT_MODE:
				titleInfo.changeflag = false;
				titleInfo.curtainState = false;
				static	float	screenSpeed = 1.0f;
				screen->SetScreenMode( SCREEN_MODE::WHITE_IN, screenSpeed );

				switch ( titleInfo.nextMode )
				{
				case 0:
					mode = TITLE_MODE::MENU;
					break;

				case 1:
					mode = TITLE_MODE::CREDIT;
					break;
				}
				break;
			}
		}

		//	描画
		void	sceneTitle::TitleRender( void )
		{
			//	背景描画
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight , RS_COPY, 0xFFFFFFFF );

			//	幕描画
			curtain->Render();

			//	タイトル画像描画
			RenderImage( titleInfo.titleImage, 0, 0, 512, 512, IMAGE_MODE::NORMAL );

			//	テキスト描画
			RenderImage( titleInfo.gameStartImage, 0, 0, 512, 128, IMAGE_MODE::FLASH );
			RenderImage( titleInfo.creditTextImage, 0, 128, 512, 128, IMAGE_MODE::FLASH );
			RenderImage( titleInfo.pressSpace, 0, 0, 256, 128, IMAGE_MODE::WAVE );
			RenderImage( titleInfo.gameStartImage, 0, 0, 512, 128, IMAGE_MODE::WAVE );
			RenderImage( titleInfo.creditTextImage, 0, 128, 512, 128, IMAGE_MODE::WAVE );
		}

	//--------------------------------------------------------
	//	メニュー
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::MenuUpdate( void )
		{
			//	移動先をかえる
			if ( mainView->GetMoveState() )	//	移動が終了してたら
			{
				//	決定
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
				{
					//	決定音
					sound->PlaySE( SE::DECIDE_SE );

					//	カメラ移動先、注視点設定
					if ( menuInfo.menu_num == TITLE_TARGET::PLAY )
					{
						mode = TITLE_MODE::PLAY;
						if ( mainView->GetMoveState() )
						{
							mainView->SetNextPoint( TITLE_TARGET::MOVE_MENU_UP, 0.008f );
						}
					}
				}
			}

			//	カメラ更新
			mainView->Update( VIEW_MODE::TITLE );
		}

		//	描画
		void	sceneTitle::MenuRender( void )
		{

		}

	//--------------------------------------------------------
	//	選択画面へ
	//--------------------------------------------------------
		
		//　更新
		void	sceneTitle::MoveSceneUpdate( void )
		{			
			//	カメラ更新
			mainView->Update( VIEW_MODE::TITLE );

			switch ( cameraInfo.state )
			{
			case 0:
				if ( mainView->GetMoveState() )
				{
					mainView->SetNextPoint( TITLE_TARGET::MOVE_MENU_IN, 0.01f );
					screen->SetScreenMode( SCREEN_MODE::WIPE_OUT, 1.5f );
					cameraInfo.state++;
				}
				break;

			case 1:
				if ( screen->GetScreenState() )
				{
					MainFrame->ChangeScene( new sceneMenu() );
					return;
				}
				break;
			}
		}

		//　描画
		void	sceneTitle::MoveSceneRender( void )
		{

		}

	//--------------------------------------------------------
	//	クレジット
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::CreditUpdate( void )
		{
			//	タイトルへ戻る
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3  || input[0]->Get( KEY_B ) == 3 )
			{
				mode = TITLE_MODE::TITLE;
				MainFrame->ChangeScene( new sceneTitle() );
				return;
			}
			
			//	スクリーン切り替え修了後
			if ( screen->GetScreenState() )
			{
				creditInfo.t += 0.003f;
				if ( creditInfo.t >= 1.0f )	creditInfo.t = 1.0f;
			}

			//	補間
			Lerp( creditInfo.sy, 0, 800, creditInfo.t );
		}

		//	描画
		void	sceneTitle::CreditRender( void )
		{
			creditInfo.credit->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, creditInfo.sy, 512, 512 );
		}