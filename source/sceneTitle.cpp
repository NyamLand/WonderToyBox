
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
#include	"UI.h"
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
	//	タイトルメニュー用情報
	namespace Title_Info
	{
		const		LPSTR	characterName[] =
		{
			"ナイト",
			"プリンセス",
			"リス",
			"Yねえさん"
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
		SafeDelete( mainView );
		SafeDelete( titleInfo.curtainL.obj );
		SafeDelete( titleInfo.curtainR.obj );
		SafeDelete( titleInfo.titleImage.obj );
		SafeDelete( titleInfo.pressSpace.obj );
		SafeDelete( stage );
		SafeDelete( titleInfo.titleText );
		SafeDelete( creditInfo.credit );
		sound->AllStop();
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

		//	カメラ設定
		mainView = new Camera();

		//	マネージャー初期化
		gameManager->Initialize();

		//	パーティクル初期化
		particle->Initialize();

		//	音登録
		sound->Initialize();

		//	スクリーン初期化
		screen->Initialize();
		screen->SetScreenMode( SCREEN_MODE::FADE_IN, 1.0f );

		//	UI初期化
		ui = new UI();
		ui->Initialize( UI_MODE::TITLE );
		
		//	ステージ
		stage = new iexMesh( "DATA/BG/titlestage/title_map.IMO" );

		//	乱数初期化
		Random::Initialize();		

		//	各モード初期化
		TitleInitialize();
		MenuInitialize();
		CreditInitialize();

		return	true;
	}

	//	タイトル初期化
	void	sceneTitle::TitleInitialize( void )
	{
		//	画像読み込み
		titleInfo.curtainL.obj = new iex2DObj( "DATA/UI/title/curtain1.png" );
		titleInfo.curtainR.obj = new iex2DObj( "DATA/UI/title/curtain2.png" );
		titleInfo.titleImage.obj = new iex2DObj( "DATA/UI/title.png" );
		titleInfo.pressSpace.obj = new iex2DObj( "DATA/UI/pressspace.png" );
		titleInfo.titleText = new iex2DObj( "DATA/UI/title/titleText.png" );

		//	画像構造体初期化
		ImageInitialize( titleInfo.pressSpace, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.82f ), static_cast<int>( iexSystem::ScreenWidth * 0.2f ), static_cast<int>( iexSystem::ScreenHeight * 0.15f ), 0, 0, 256, 128 );
		ImageInitialize( titleInfo.titleImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.42f ), static_cast<int>( iexSystem::ScreenWidth * 0.4f ), static_cast<int>( iexSystem::ScreenHeight * 0.7f ), 0, 0, 512, 512 );
		ImageInitialize( titleInfo.gameStartImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.80f ), static_cast<int>( iexSystem::ScreenWidth * 0.15f ), static_cast<int>( iexSystem::ScreenHeight * 0.07f ), 0, 0, 512, 128 );
		ImageInitialize( titleInfo.creditTextImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.87f ), static_cast<int>( iexSystem::ScreenWidth * 0.1f ), static_cast<int>( iexSystem::ScreenHeight * 0.07f ), 0, 128, 512, 128 );
		titleInfo.gameStartImage.obj = titleInfo.titleText;
		titleInfo.creditTextImage.obj = titleInfo.titleText;

		//	カーテン用構造体初期化
		{
			//	パラメータ初期化
			titleInfo.curtainL.t = 0.0f;
			titleInfo.curtainR.t = 0.0f;

			//	頂点設定
			SetVertex( titleInfo.curtainL.tlv[0], 0, 0, 0, 0, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[1], static_cast<float>( iexSystem::ScreenWidth / 2 ), 0, 0, 1, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[2], 0, static_cast<float>( iexSystem::ScreenHeight ), 0, 0, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[3], static_cast<float>( iexSystem::ScreenWidth / 2 ), static_cast<float>( iexSystem::ScreenHeight ), 0, 1, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[0], static_cast<float>( iexSystem::ScreenWidth / 2 ), 0, 0, 0, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[1], static_cast<float>( iexSystem::ScreenWidth ), 0, 0, 1, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[2], static_cast<float>( iexSystem::ScreenWidth / 2 ), static_cast<float>( iexSystem::ScreenHeight ), 0, 0, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[3], static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight ), 0, 1, 1, 0xFFFFFFFF );
		}

		//	変数初期化
		titleInfo.titleImage.renderflag = true;
		titleInfo.step = 0;
		titleInfo.nextMode = 0;
	}

	//	メニュー初期化
	void	sceneTitle::MenuInitialize( void )
	{
		menuInfo.menu_num = 0;
	}

	//	クレジット初期化
	void	sceneTitle::CreditInitialize( void )
	{
		creditInfo.credit = new iex2DObj( "DATA/UI/title/credit.png" );
		creditInfo.sy = 0;
		creditInfo.t = 0.0f;
	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	更新
	void	sceneTitle::Update( void )
	{
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
			MoveSelectUpdate();
			break;

		case TITLE_MODE::OPTION:
			OptionUpdate();
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
		mainView->Clear();

		//	ステージ描画
		stage->Render( shader3D, "full" );

		//	パーティクル描画
		particle->Render();

		switch ( mode )
		{
		case TITLE_MODE::TITLE:
			TitleRender();
			break;

		case TITLE_MODE::MENU:
			MenuRender();
			break;

		case TITLE_MODE::PLAY:
			MoveSelectRender();
			break;

		case TITLE_MODE::OPTION:
			OptionRender();
			break;

		case TITLE_MODE::CREDIT:
			CreditRender();
			break;
		}

		//	UI描画
		ui->Render( mode );

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
			//	パラメータ
			static	bool	changeflag = false;
			static	bool	curtainStateL = false;
			static	bool	curtainStateR = false;
			static	float	speed = 0.5f;

			switch ( titleInfo.step )
			{
			case 0:
				//	フェード処理終了まで待つ
				if ( screen->GetScreenState() )	titleInfo.step++;
				break;

			case 1:
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
				if ( KEY( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
				{
					//	タイトル画像を隠す
					titleInfo.titleImage.renderflag = false;

					//	画面制御
					screen->SetScreenMode( SCREEN_MODE::WHITE_OUT, 1.0f );

					//	pressspace波紋
					static	float	wavespeed = 1.5f;
					
					switch ( titleInfo.nextMode )
					{
					case 0:
						SetWave( titleInfo.gameStartImage, wavespeed );
						break;

					case 1:
						SetWave( titleInfo.creditTextImage, wavespeed );
						break;
					}
					titleInfo.step++;
				}
				break;

			case 2:
				//	カメラ更新
				mainView->Update( VIEW_MODE::TITLE );

				//	波紋更新
				//WaveUpdate( titleInfo.pressSpace );
				WaveUpdate( titleInfo.gameStartImage );
				WaveUpdate( titleInfo.creditTextImage );

				//	点滅更新
				switch (titleInfo.nextMode)
				{
				case 0:
					FlashingUpdate( titleInfo.gameStartImage, D3DX_PI / 180 * 10.0f );
					titleInfo.creditTextImage.flashingAlpha = 1.0f;
					break;

				case 1:
					FlashingUpdate( titleInfo.creditTextImage, D3DX_PI / 180 * 10.0f );
					titleInfo.gameStartImage.flashingAlpha = 1.0f;
					break;
				}
				//	パラメータ加算
				titleInfo.curtainL.t += D3DX_PI / 180 * speed;
				titleInfo.curtainR.t += D3DX_PI / 180 * speed;

				//	パラメータ上限設定
				if ( titleInfo.curtainL.t >= 1.0f )	titleInfo.curtainL.t = 1.0f;
				if ( titleInfo.curtainR.t >= 1.0f )	titleInfo.curtainR.t = 1.0f;

				//	各頂点移動
				curtainStateL = Lerp( titleInfo.curtainL.tlv[0].sx, 0, -( static_cast<int>( iexSystem::ScreenWidth / 2 ) ), GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[1].sx, static_cast<int>( iexSystem::ScreenWidth / 2 ), 0, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[2].sx, 0, -( static_cast<int>( iexSystem::ScreenWidth / 2 ) ), GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[3].sx, static_cast<int>( iexSystem::ScreenWidth / 2 ), 0, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[0].sx, static_cast<int>( iexSystem::ScreenWidth / 2 ), static_cast<int>( iexSystem::ScreenWidth ), GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[1].sx, static_cast<int>( iexSystem::ScreenWidth ), static_cast<int>( iexSystem::ScreenWidth * 1.5f ), GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[2].sx, static_cast<int>( iexSystem::ScreenWidth / 2 ), static_cast<int>( iexSystem::ScreenWidth ), GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[3].sx, static_cast<int>( iexSystem::ScreenWidth ), static_cast<int>( iexSystem::ScreenWidth * 1.5f ), GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );

				//	動作済みで次のステップへ
				if ( curtainStateL && curtainStateR )	titleInfo.step++;
				break;

			case 3:
				changeflag = false;
				curtainStateL = false;
				curtainStateR = false;
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
			iexPolygon::Rect(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight , RS_COPY, 0xFFFFFFFF);

			//	幕描画
			iexPolygon::Render2D( titleInfo.curtainL.tlv, 2, titleInfo.curtainL.obj, RS_COPY );
			iexPolygon::Render2D( titleInfo.curtainR.tlv, 2, titleInfo.curtainL.obj, RS_COPY );

			//	タイトル画像描画
			RenderImage( titleInfo.titleImage, 0, 0, 512, 512, IMAGE_MODE::NORMAL );

			//	pressSpace描画
			//RenderImage( titleInfo.pressSpace, 0, 0, 256, 128, IMAGE_MODE::FLASH );
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
				//	C,Vキーで選択
				if ( KEY( KEY_LEFT ) == 3 )
				{
					menuInfo.menu_num--;
					if ( menuInfo.menu_num < 0 )		menuInfo.menu_num = TITLE_TARGET::CREDIT;
					mainView->SetNextPoint( menuInfo.menu_num, 0.01f );
				}
				if ( KEY( KEY_RIGHT ) == 3 )
				{
					menuInfo.menu_num++;
					if ( menuInfo.menu_num > TITLE_TARGET::CREDIT )	menuInfo.menu_num = 0;
					mainView->SetNextPoint( menuInfo.menu_num, 0.01f );
				}

				//	決定
				if ( KEY( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
				{
					switch ( menuInfo.menu_num )
					{
					case TITLE_TARGET::PLAY:
						mode = TITLE_MODE::PLAY;
						if (mainView->GetMoveState())
						{
							mainView->SetNextPoint(TITLE_TARGET::MOVE_MAIN, 0.005f);
							//sound->PlaySE(SE::DECIDE_SE);
							screen->SetScreenMode( SCREEN_MODE::WIPE_OUT, 1.5f );
						}
						break;

					case TITLE_TARGET::OPTION:
						mode = TITLE_MODE::OPTION;
						break;

					case TITLE_TARGET::CREDIT:
						mode = TITLE_MODE::CREDIT;
						break;
					}	
					sound->PlaySE( SE::DECIDE_SE );
				}
			}

			//	カメラ更新
			mainView->Update( VIEW_MODE::TITLE );

			//	UI更新
			switch ( menuInfo.menu_num )
			{
			case TITLE_TARGET::PLAY:
				ui->Update(TITLE_MODE::MENU);
				break;

			case TITLE_TARGET::OPTION:
				ui->Update(TITLE_MODE::OPTION);
				break;

			case TITLE_TARGET::CREDIT:
				ui->Update(TITLE_MODE::CREDIT);
				break;
			}
		}

		//	描画
		void	sceneTitle::MenuRender( void )
		{
			DrawString( "メニュー画面だよ", 50, 50 );
			DrawString( "C・Vでどれか選んでね", 300, 400, 0xFFFFFF00 );

			switch ( menuInfo.menu_num )
			{
			case TITLE_TARGET::PLAY:
				DrawString( "みんなであそぶモード！", 50, 100 );
				DrawString( "[SPACE]：キャラ選択へ", 300, 200, 0xFFFFFF00 );
				break;

			case TITLE_TARGET::OPTION:
				DrawString( "オプション", 50, 100 );
				DrawString( "[SPACE]：オプションへ", 300, 200, 0xFFFFFF00 );
				break;

			case TITLE_TARGET::CREDIT:
				DrawString( "つくった戦士たち！", 50, 100 );
				DrawString( "[SPACE]：クレジットへ", 300, 200, 0xFFFFFF00 );
				break;
			}
			ui->Render( 0 );
		}

	//--------------------------------------------------------
	//	選択画面へ
	//--------------------------------------------------------
		
		//　更新
		void	sceneTitle::MoveSelectUpdate( void )
		{			
			//	カメラ更新
			mainView->Update(VIEW_MODE::TITLE);

			//	UI更新
			ui->Update( TITLE_MODE::MOVE_MAIN );

			if ( screen->GetScreenState() )
			{
				MainFrame->ChangeScene(new sceneMenu());
				return;
			}
		}

		//　描画
		void	sceneTitle::MoveSelectRender( void )
		{

		}

	//--------------------------------------------------------
	//	オプション
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::OptionUpdate( void )
		{
			mainView->Update( VIEW_MODE::TITLE );
			ui->Update( mode );
			if ( !mainView->GetMoveState() )	return;
			if ( KEY( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				sound->PlaySE( SE::DECIDE_SE );
				mainView->SetNextPoint( TITLE_TARGET::PLAY, 0.01f );
			}
			mainView->SetNextPoint(menuInfo.menu_num, 0.01f);
		}

		//	描画
		void	sceneTitle::OptionRender( void )
		{
			DrawString( "オプションだよ。設定いじってね", 50, 50 );
			DrawString( "[SPACE]：メニューへ", 300, 400, 0xFFFFFF00 );
		}

	//--------------------------------------------------------
	//	クレジット
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::CreditUpdate( void )
		{
			if ( input[0]->Get( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::TITLE;
				creditInfo.t = 0.0f;
			}
			
			if (screen->GetScreenState())
			{
				creditInfo.t += 0.005f;
				if ( creditInfo.t >= 1.0f )	creditInfo.t = 1.0f;
			}

			Lerp( creditInfo.sy, 0, 1536, creditInfo.t );
		}

		//	描画
		void	sceneTitle::CreditRender( void )
		{
			creditInfo.credit->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, creditInfo.sy, 512, 512 );
		}