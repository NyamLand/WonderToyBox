
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
		sound->AllStop();
	}
	
	//	初期化
	bool	sceneTitle::Initialize( void )
	{
		//	環境設定
		iexLight::SetAmbient( 0x404040 );
		iexLight::SetFog( 800, 1000, 0 );

		//	ライト設定
		Vector3 dir( 1.0f, -1.0f, -0.5f );
		dir.Normalize();
		iexLight::DirLight( shader3D, 0, &dir, 0.8f, 0.8f, 0.8f );

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

		//	UI初期化
		ui = new UI();
		ui->Initialize( UI_MODE::TITLE );
		
		//	ステージ
		stage = new iexMesh( "DATA/BG/title_map.IMO" );

		//	乱数初期化
		Random::Initialize();		

		//	各モード初期化
		TitleInitialize();
		MenuInitialize();

		return	true;
	}

	//	タイトル初期化
	void	sceneTitle::TitleInitialize( void )
	{
		//	画像読み込み
		titleInfo.curtainL.obj = new iex2DObj( "DATA/curtain1.png" );
		titleInfo.curtainR.obj = new iex2DObj( "DATA/curtain2.png" );
		titleInfo.titleImage.obj = new iex2DObj( "DATA/UI/title.png" );
		titleInfo.pressSpace.obj = new iex2DObj( "DATA/UI/pressspace.png" );

		//	画像構造体初期化
		ImageInitialize( titleInfo.pressSpace, 640, 560, 300, 100, 0, 0, 256, 128 );
		ImageInitialize( titleInfo.titleImage, 640, 300, 500, 500, 0, 0, 512, 512 );

		//	カーテン用構造体初期化
		{
			//	パラメータ初期化
			titleInfo.curtainL.t = 0.0f;
			titleInfo.curtainR.t = 0.0f;

			//	頂点設定
			SetVertex( titleInfo.curtainL.tlv[0], 0, 0, 0, 0, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[1], 640, 0, 0, 1, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[2], 0, 720, 0, 0, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[3], 640, 720, 0, 1, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[0], 640, 0, 0, 0, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[1], 1280, 0, 0, 1, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[2], 640, 720, 0, 0, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[3], 1280, 720, 0, 1, 1, 0xFFFFFFFF );
		}

		//	変数初期化
		titleInfo.titleImage.renderflag = true;
		titleInfo.step = 0;
	}

	//	メニュー初期化
	void	sceneTitle::MenuInitialize( void )
	{
		menuInfo.menu_num = 0;
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
		stage->Render();

		//	パーティクル描画
		particle->Render();

		switch (mode)
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

		//	デバッグ
		mainView->Render();



		//char	str[256];
		Matrix	mat = mainView->GetMatrix();
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();
		//sprintf_s( str, "t.x = %f\nt.y = %f\nt.z = %f\n", eff_pos.x, eff_pos.y, eff_pos.z );
		//DrawString( str, 50, 500 );
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
				//	カメラ更新
				mainView->Update( VIEW_MODE::SETUP );

				//	点滅更新
				FlashingUpdate( titleInfo.pressSpace, D3DX_PI / 180 * 4.0f );

				//	SPACEキーで選択
				if ( KEY( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
				{
					//	タイトル画像を隠す
					titleInfo.titleImage.renderflag = false;

					//	画面制御
					screen->SetScreenMode( SCREEN_MODE::WHITE_OUT, speed );

					//	pressspace波紋
					static	float	wavespeed = 1.5f;
					SetWave( titleInfo.pressSpace, wavespeed );
					titleInfo.step++;
				}
				break;

			case 1:
				//	カメラ更新
				mainView->Update( VIEW_MODE::TITLE );

				//	波紋更新
				WaveUpdate( titleInfo.pressSpace );

				//	点滅更新
				FlashingUpdate( titleInfo.pressSpace, D3DX_PI / 180.0f * 10.0f );

				//	パラメータ加算
				titleInfo.curtainL.t += D3DX_PI / 180 * speed;
				titleInfo.curtainR.t += D3DX_PI / 180 * speed;

				//	パラメータ上限設定
				if ( titleInfo.curtainL.t >= 1.0f )	titleInfo.curtainL.t = 1.0f;
				if ( titleInfo.curtainR.t >= 1.0f )	titleInfo.curtainR.t = 1.0f;

				//	各頂点移動
				curtainStateL = Lerp( titleInfo.curtainL.tlv[0].sx,    0, -640, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[1].sx,  640,    0, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[2].sx,    0, -640, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[3].sx,  640,    0, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[0].sx,  640, 1280, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[1].sx, 1280, 1920, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[2].sx,  640, 1280, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[3].sx, 1280, 1920, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );

				//	動作済みで次のステップへ
				if ( curtainStateL && curtainStateR )	titleInfo.step++;
				break;

			case 2:
				changeflag = false;
				curtainStateL = false;
				curtainStateR = false;
				static	float	screenSpeed = 1.0f;
				screen->SetScreenMode( SCREEN_MODE::WHITE_IN, screenSpeed );
				mode = TITLE_MODE::MENU;
				break;
			}
		}

		//	描画
		void	sceneTitle::TitleRender( void )
		{
			//	背景描画
			iexPolygon::Rect( 0, 0, 1280, 720, RS_COPY, 0xFFFFFFFF );

			//	幕描画
			iexPolygon::Render2D( titleInfo.curtainL.tlv, 2, titleInfo.curtainL.obj, RS_COPY );
			iexPolygon::Render2D( titleInfo.curtainR.tlv, 2, titleInfo.curtainL.obj, RS_COPY );

			//	タイトル画像描画
			RenderImage( titleInfo.titleImage, 0, 0, 512, 512, IMAGE_MODE::NORMAL );

			//	pressSpace描画
			RenderImage( titleInfo.pressSpace, 0, 0, 256, 128, IMAGE_MODE::FLASH );
			RenderImage( titleInfo.pressSpace, 0, 0, 256, 128, IMAGE_MODE::WAVE );
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
							screen->SetScreenMode( SCREEN_MODE::WIPE_OUT, 1.0f );
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

			if (screen->GetScreenState())
			{
				MainFrame->ChangeScene(new sceneLoad(new sceneMenu()));
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
			ui->Update( mode );
			mainView->Update( VIEW_MODE::TITLE );
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
		void	sceneTitle::CreditRender( void )
		{
			DrawString( "つくった戦士たちの紹介だよ", 50, 50 );
			DrawString( "[SPACE]：メニューへ", 300, 400, 0xFFFFFF00 );
		}