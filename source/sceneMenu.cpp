
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
#include	"sceneLoad.h"
#include	"sceneTitle.h"

#include	"sceneMenu.h"

//*******************************************************************************
//
//	sceneMenuクラス
//
//*******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------

	namespace
	{
		namespace MENU_MODE
		{
			enum
			{
				INIT,
				SELECT_PLAYERNUM,
				SELECT_CHARACTER,
				SELECT_STAGE,
				SELECT_CHECK,
				MOVE_MAIN,
				MOVE_TITLE,
				OPTION,
			};
		}
	}

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	sceneMenu::sceneMenu( void )
	{
	
	}

	//	デストラクタ
	sceneMenu::~sceneMenu( void )
	{
		Release();
	}

	//	初期化
	bool	sceneMenu::Initialize( void )
	{
		//	camera
		mainView = new Camera();
		mainView->SetProjection( D3DXToRadian( 10.0f ), 1.0f, 1000.0f );
		mainView->Set( Vector3( 0.0f, 5.2f, -70.0f ), Vector3( 0.0, 5.2f, 0.0f ) );

		//	random
		Random::Initialize();

		//	screen
		screen->Initialize();
		screen->SetScreenMode( SCREEN_MODE::WIPE_IN, 1.5f );

		//	ゲームマネージャ初期化
		OptionInitialize();
		gameManager->Initialize();
		
		//	画像読み込み
		back = make_unique<iex2DObj>( LPSTR( "DATA/UI/back.png" ) );
		frame = make_unique<iex2DObj>( LPSTR( "DATA/UI/frame.png" ) );
		checkBack = make_unique<iex2DObj>( LPSTR( "DATA/UI/selectCheck_Back.png" ) );
		checkCursor = make_unique<iex2DObj>( LPSTR( "DATA/UI/selectCheck_Cursor.png" ) );
		playerNumText = make_unique<iex2DObj>( LPSTR( "DATA/UI/playerNum_Text.png" ) );
		playerNum = make_unique<iex2DObj>( LPSTR( "DATA/UI/playerNum.png" ) );
		face = new iex2DObj( "DATA/UI/chara_emotion.png" );
		cursor = new iex2DObj( "DATA/UI/cursor.png" );


		Oimage = new iex2DObj("DATA/UI/OptionText.png");
		Otime = new iex2DObj("DATA/UI/number.png");
		OCmax = new iex2DObj("DATA/UI/number.png");
		//	モデル読み込み
		//org[CHARACTER_TYPE::KNIGHT] = new iex3DObj( "DATA/CHR/Knight/Knight_Dammy.IEM" );		//	騎士
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Knight/Knight_Dammy.IEM" ) );		//	掃除屋
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Y2009/Y2009.IEM" ) );					//	姫
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>(LPSTR("DATA/CHR/SQUIRREL/SQUIRREL.IEM"));		//	リス
		org[CHARACTER_TYPE::TIGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/ECCMAN/ECCMAN.IEM" ) );				//	トラ

		//	オリジナルモデル情報初期化
		org[CHARACTER_TYPE::SCAVENGER]->SetScale( 0.05f );	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetScale( 0.02f );		//	姫
		org[CHARACTER_TYPE::THIEF]->SetScale(0.04f);		//	リス
		org[CHARACTER_TYPE::TIGER]->SetScale( 0.02f );			//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle( D3DX_PI );	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetAngle( D3DX_PI );	//	姫
		org[CHARACTER_TYPE::THIEF]->SetAngle( D3DX_PI );	//	リス
		org[CHARACTER_TYPE::TIGER]->SetAngle( D3DX_PI );			//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion( 2 );	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 1 );		//	姫
		org[CHARACTER_TYPE::THIEF]->SetMotion( 0 );		//	リス
		org[CHARACTER_TYPE::TIGER]->SetMotion( 0 );			//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->Update();	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->Update();		//	姫
		org[CHARACTER_TYPE::THIEF ]->Update();		//	リス
		org[CHARACTER_TYPE::TIGER]->Update();			//	トラ

		deskStage = make_unique<iexMesh>( LPSTR( "DATA/back/stage.IMO" ) );
		forestStage = make_unique<iexMesh>( LPSTR( "DATA/BG/Forest/model/forest.IMO" ) );

		//	画像構造体初期化
		{
			textImage.obj = new iex2DObj( "DATA/UI/menu-head.png" );
			int x = static_cast<int>( iexSystem::ScreenWidth / 2 );
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.29f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
			ImageInitialize( textImage, x, y, w, h, 512, 0, 512, 256 );
			textImage.angle = D3DXToRadian( 5.0f );
			textImage.renderflag = true;
		}

		//	モード設定
		SetMode( MENU_MODE::INIT );

		//	全体更新
		Update();
		return	true;
	}

	//	解放
	void	sceneMenu::Release( void )
	{
		SafeDelete( mainView );
		SafeDelete( textImage.obj );
		SafeDelete( face );
		SafeDelete( cursor );
		Random::Release();

		//設計中
		SafeDelete(Oimage);
		SafeDelete(Otime);
		SafeDelete(OCmax);
	}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------

	//	更新
	void	sceneMenu::Update( void )
	{
		switch ( mode )
		{
		case MENU_MODE::INIT:
			if ( screen->GetScreenState() )
			{
				SetMode( MENU_MODE::SELECT_PLAYERNUM );
			}
			break;

		case MENU_MODE::SELECT_PLAYERNUM:
			SelectPlayerNumUpdate();
			break;

		case MENU_MODE::SELECT_CHARACTER:
			SelectCharacterUpdate();
			break;

		case MENU_MODE::SELECT_STAGE:
			SelectStageUpdate();
			break;

		case MENU_MODE::SELECT_CHECK:
			SelectCheckUpdate();
			break;

		case MENU_MODE::MOVE_MAIN:
			MoveMainUpdate();
			break;

		case MENU_MODE::MOVE_TITLE:
			MoveTitleUpdate();
			break;

		case MENU_MODE::OPTION:
			OptionUpdate();
			break;
		}
		if (KEY_Get(KEY_B) == 3){
			tempmode = mode+1;
			SetMode(MENU_MODE::OPTION);
		}

		//	スクリーン更新
		screen->Update();
	}

	//	描画
	void	sceneMenu::Render( void )
	{
		//	camera
		mainView->Activate();
		mainView->Clear();

		//	背景( 一番後ろに表示 )
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		switch ( mode )
		{
		case MENU_MODE::SELECT_PLAYERNUM:
			SelectPlayerNumRender();
			break;

		case MENU_MODE::SELECT_CHARACTER:
			SelectCharacterRender();
			break;

		case MENU_MODE::SELECT_STAGE:
			SelectStageRender();
			break;

		case MENU_MODE::SELECT_CHECK:
			SelectCheckRender();
			break;

		case MENU_MODE::MOVE_MAIN:
			MoveMainRender();
			break;
		case MENU_MODE::OPTION:
			OptionRender();
			break;
		}

		//	スクリーン
		screen->Render();
	}

//-------------------------------------------------------------------------------
//	プレイ人数選択動作
//-------------------------------------------------------------------------------

	//	初期化
	void	sceneMenu::SelectPlayerNumInitialize( void )
	{
		playerNumSelectInfo.num = 0;
		playerNumSelectInfo.t = 1.0f;
		playerNumSelectInfo.saveY = 0;
		playerNumSelectInfo.sy = 0;
	}

	//	更新
	void	sceneMenu::SelectPlayerNumUpdate( void )
	{
		//	パラメータ加算
		playerNumSelectInfo.t += 0.08f;
		if ( playerNumSelectInfo.t >= 1.0f )	playerNumSelectInfo.t = 1.0f;	

		//	移動が終わったら
		if ( playerNumSelectInfo.t >= 1.0f )
		{
			//	選択
			if ( input[0]->Get( KEY_DOWN ) == 1 )
			{
				//	元の座標を保存
				playerNumSelectInfo.saveY =  128 * playerNumSelectInfo.num;
				playerNumSelectInfo.num++;
				if ( playerNumSelectInfo.num >= 4 )
				{
					playerNumSelectInfo.num = 3;
					playerNumSelectInfo.t = 1.0f;
				}
				else
				{
					playerNumSelectInfo.t = 0.0f;
				}
			}

			if ( input[0]->Get( KEY_UP ) == 1 )
			{
				//	元の座標を保存
				playerNumSelectInfo.saveY = 128 * playerNumSelectInfo.num;
				playerNumSelectInfo.num--;
				if ( playerNumSelectInfo.num < 0 )
				{
					playerNumSelectInfo.num = 0;
					playerNumSelectInfo.t = 1.0f;
				}
				else
				{
					playerNumSelectInfo.t = 0.0f;
				}
			}

			//	決定
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				gameManager->SetPlayerNum( playerNumSelectInfo.num + 1 );
				SetMode( MENU_MODE::SELECT_CHARACTER );
			}
		}

		if ( input[0]->Get( KEY_B ) == 3 )
		{
			SetMode( MENU_MODE::MOVE_TITLE );
		}

		//	補間
		CubicFunctionInterpolation( playerNumSelectInfo.sy, playerNumSelectInfo.saveY, 128 * playerNumSelectInfo.num, playerNumSelectInfo.t );
	}

	//	描画
	void	sceneMenu::SelectPlayerNumRender( void )
	{
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.28f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.4f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.08f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.15f );
		playerNum->Render( x, y, w, h, 0, playerNumSelectInfo.sy, 128, 128 );

		x = static_cast<int>( iexSystem::ScreenWidth * 0.35f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.41f );
		w = static_cast<int>( iexSystem::ScreenWidth * 0.39f );
		h = static_cast<int>( iexSystem::ScreenHeight * 0.138f );
		playerNumText->Render( x, y, w, h, 0, 0, 512, 128 );
	}

//-------------------------------------------------------------------------------
//	キャラクター選択動作
//-------------------------------------------------------------------------------

	//	キャラクター選択初期化
	void	sceneMenu::SelectCharacterInitialize( void )
	{
		//	モデル、選択情報初期化
		FOR( 0, PLAYER_MAX )
		{
			//	モデル登録、初期化
			obj[value] = org[value]->Clone();
			obj[value]->SetPos( -7.0f + ( 14.0f / 3.0f * value ), 0.0f, 0.0f );
			obj[value]->Update();

			//	選択情報初期化
			characterSelectInfo.character[value] = value;
			characterSelectInfo.select[value] = false;
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.04f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.07f );
			ImageInitialize( cursorImage[value], 0, 0, w, h, 0, 0, 0, 0 );
			cursorImage[value].obj = cursor;

			if ( value >= gameManager->GetPlayerNum() )
			{
				cursorImage[value].renderflag = false;
			}
		}

		//	変数初期化
		characterSelectInfo.imagePos = 0;
		characterSelectInfo.step = 0;
		characterSelectInfo.timer = 0;
		characterSelectInfo.playerNum = gameManager->GetPlayerNum();

		//	画像位置初期化
		textImage.angle = D3DXToRadian( 5.0f );
		textImage.renderflag = true;

		//	顔画像初期化
		FOR( 0, CHARACTER_TYPE::MAX )
		{
			int left = static_cast<int>( iexSystem::ScreenWidth / 5 );
			int x = left + left * value;
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.45f );
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.155f );
			ImageInitialize( faceImage[value], x, y, w, h, 0, 256 * value, 256, 256 );
			faceImage[value].obj = face;
		}
	}

	//	キャラクター選択
	void	sceneMenu::SelectCharacterUpdate( void )
	{
		//	各プレイヤー動作
		FOR( 0, characterSelectInfo.playerNum )
		{
			//	キャンセル
			if ( input[value]->Get( KEY_B ) == 3 )		characterSelectInfo.select[value] = false;

			//	選択済みだったらキャンセルのみ受付け
			if ( characterSelectInfo.select[value] )	continue;

			//	カーソル移動
			if ( input[value]->Get( KEY_RIGHT ) == 3 )	characterSelectInfo.character[value]++;
			if ( input[value]->Get( KEY_LEFT ) == 3 )		characterSelectInfo.character[value]--;

			//	上限・下限設定
			if ( characterSelectInfo.character[value] < 0 )	characterSelectInfo.character[value] = CHARACTER_TYPE::MAX - 1;
			if ( characterSelectInfo.character[value] >= CHARACTER_TYPE::MAX )	characterSelectInfo.character[value] = 0;

			//	決定
			if ( input[value]->Get( KEY_SPACE ) == 3 || input[value]->Get( KEY_A ) == 3 )
			{
				gameManager->SetCharacterType( value, characterSelectInfo.character[value] );
				characterSelectInfo.select[value] = true;
			}

			//	モデル差し替え
			obj[value] = org[characterSelectInfo.character[value]]->Clone();
		}

		//	選択済み人数をカウント
		int		selectCheck = 0;
		FOR( 0, gameManager->GetPlayerNum() )
		{
			if ( characterSelectInfo.select[value] )	selectCheck++;
		}

		//	全員未選択時にキャンセルボタンを押すとプレイ人数選択へ移行
		if ( selectCheck == 0 )
		{
			if ( KEY( KEY_B ) == 3 )
			{
				SetMode( MENU_MODE::SELECT_PLAYERNUM );
			}
		}

		//	全員選択済み
		if ( selectCheck >= gameManager->GetPlayerNum() )
		{
			//	コンピュータ分を適当に設定
			FOR( gameManager->GetPlayerNum(), PLAYER_MAX )
			{
				characterSelectInfo.character[value] = Random::GetInt( 0, CHARACTER_TYPE::MAX - 1 );
				gameManager->SetCharacterType( value, characterSelectInfo.character[value] );
			
				//	モデル差し替え
				obj[value] = org[characterSelectInfo.character[value]]->Clone();
			}

			//	ステージ選択へ
			SetMode( MENU_MODE::SELECT_STAGE );
		}

		//	モデルデータ更新
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->Animation();
			obj[value]->SetPos( -7.0f + ( 14.0f / 3.0f * value ), 0.0f, 0.0f );
			obj[value]->Update();
		}
	}

	//	キャラクター選択描画
	void	sceneMenu::SelectCharacterRender( void )
	{
		//	テキスト画像
		RenderImage( textImage, 512, 0, 512, 256, IMAGE_MODE::ADOPTPARAM );

		//	モデル・顔・カーソル描画
		FOR( 0, PLAYER_MAX )
		{
			//	モデル描画
			obj[value]->Render( shader3D, "toon" );
		}
		
		FOR( 0, CHARACTER_TYPE::MAX )
		{
			//	顔描画
			RenderImage( faceImage[value], 0, 256 * value, 256, 256, IMAGE_MODE::NORMAL );
		}
			//	カーソル描画
		FOR( 0, PLAYER_MAX )
		{
			cursorImage[value].x = faceImage[characterSelectInfo.character[value]].x;
			cursorImage[value].y = faceImage[characterSelectInfo.character[value]].y - faceImage[characterSelectInfo.character[value]].h / 2;
			RenderImage( cursorImage[value], 128 * ( value % 2 ), 128 * ( value / 2 ), 128, 128, IMAGE_MODE::NORMAL );
		}

	}

//-------------------------------------------------------------------------------
//	ステージ選択動作
//-------------------------------------------------------------------------------

	//	ステージ選択初期化
	void	sceneMenu::SelectStageInitialize( void )
	{
		//	机モデル初期化
		deskStage->SetPos( -5.0f, 3.0f, 0.0f );
		deskStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
		deskStage->SetScale( 0.1f );
		deskStage->Update();

		//	森モデル初期化
		forestStage->SetPos( 5.0f, 4.0f, 0.0f );
		forestStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
		forestStage->SetScale( 0.04f );
		forestStage->Update();

		//	パラメータ初期化
		stageSelectInfo.angle = D3DX_PI;
		stageSelectInfo.stage = 0;
	}

	//	ステージ選択
	void	sceneMenu::SelectStageUpdate( void )
	{
		//	ステージ回転
		stageSelectInfo.angle += 0.01f;

		//	ステージの向きを設定(選択中回転)
		switch ( stageSelectInfo.stage )
		{
		case 0:
			deskStage->SetAngle( D3DXToRadian( 30.0f ), stageSelectInfo.angle, 0.0f );
			forestStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			break;

		case 1:
			deskStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			forestStage->SetAngle( D3DXToRadian( 30.0f ), stageSelectInfo.angle, 0.0f );
			break;
		}

		//	左右で選択
		{
			if ( KEY( KEY_RIGHT ) == 3 )
			{
				stageSelectInfo.angle = D3DX_PI;
				stageSelectInfo.stage++;
			}

			if ( KEY( KEY_LEFT ) == 3 )
			{
				stageSelectInfo.angle = D3DX_PI;
				stageSelectInfo.stage--;
			}

			//	上限・下限設定
			if ( stageSelectInfo.stage >= 2 )	stageSelectInfo.stage = 0;
			if ( stageSelectInfo.stage < 0 )		stageSelectInfo.stage = 1;
		}

		//	決定
		if ( KEY( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
		{
			//	マネージャーに情報をセット
			gameManager->SetStageType( stageSelectInfo.stage );

			//	モデルの角度を戻す
			forestStage->SetAngle( D3DXToRadian( 5.0f ), D3DX_PI, 0.0f );
			deskStage->SetAngle( D3DXToRadian( 5.0f ), D3DX_PI, 0.0f );

			//	次のモードへ
			SetMode( MENU_MODE::SELECT_CHECK );
		}

		//	キャンセル
		if ( KEY( KEY_B ) == 3 )
		{
			SetMode( MENU_MODE::SELECT_CHARACTER );
		}

		//	ステージ更新
		deskStage->Update();
		forestStage->Update();
	}

	//	ステージ選択描画
	void	sceneMenu::SelectStageRender( void )
	{
		//	テキスト画像描画(背面に描画)
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		RenderImage( textImage, 0, 256, 512, 256, IMAGE_MODE::ADOPTPARAM );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		deskStage->Render();
		forestStage->Render();
	}

//-------------------------------------------------------------------------------
//	最終チェック動作
//-------------------------------------------------------------------------------

	//	最終確認初期化
	void	sceneMenu::SelectCheckInitialize( void )
	{
		//	キャラ別モーション設定
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 2 );
		
		//	各プレイヤーモデル初期化
		FOR( 0, PLAYER_MAX )
		{
			obj[value] = org[gameManager->GetCharacterType( value )]->Clone();
			obj[value]->SetPos( -7.0f + ( 14.0f / 3.0f * value ), 0.0f, 0.0f );
			obj[value]->Update();
		}

		//	構造体初期化
		{
			checkSelectInfo.check = false;
			checkSelectInfo.select = false;
			checkSelectInfo.step = 0;
		}

		//	ステージ座標、向き設定
		{
			deskStage->SetPos( 0.0f, 5.0f, 5.0f );
			deskStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			deskStage->SetScale( 0.08f );
			deskStage->Update();
			forestStage->SetPos( 0.0f, 6.0f, 5.0f );
			forestStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			forestStage->SetScale( 0.03f );
			forestStage->Update();
		}
	}

	//	最終確認
	void	sceneMenu::SelectCheckUpdate( void )
	{
		//	決定（はい：メインへ、いいえ：キャラ選択へ）
		if ( input[0]->Get( KEY_A ) == 3 || input[0]->Get( KEY_SPACE ) == 3 )
		{
			if ( !checkSelectInfo.check )	checkSelectInfo.check = true;
			else
			{
				if ( !checkSelectInfo.select )
				{
					SetMode( MENU_MODE::MOVE_MAIN );
				}
				else
				{
					SetMode( MENU_MODE::SELECT_CHARACTER );
				}
			}
		}

		//	選択中動作(カーソル移動, キャンセル)
		if ( checkSelectInfo.check )
		{
			if ( input[0]->Get( KEY_RIGHT ) == 3 || input[0]->Get( KEY_LEFT ) == 3 )
			{
				checkSelectInfo.select = !checkSelectInfo.select;
			}

			if ( input[0]->Get( KEY_B ) == 3 )
			{
				checkSelectInfo.check = false;
				checkSelectInfo.select = false;
			}
		}
		else
		{
			//	キャンセルでステージ選択へ
			if ( input[0]->Get( KEY_B ) == 3 )
			{
				SetMode( MENU_MODE::SELECT_STAGE );
			}
		}

		//	モデル更新
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->Animation();
			obj[value]->Update();
		}
	}

	//	最終確認描画
	void	sceneMenu::SelectCheckRender( void )
	{
		//	ステージ描画
		switch ( gameManager->GetStageType() )
		{
		case 0:		deskStage->Render();		break;
		case 1:		forestStage->Render();		break;
		}

		//	プレイヤー描画
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->Render( shader3D, "toon" );
		}

		//	テキスト画像描画
		RenderImage( textImage, 0, 512, 512, 256, IMAGE_MODE::ADOPTPARAM );
		
		//	チェック項目描画
		if ( checkSelectInfo.check )
		{
			int x = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.17f );
			int w = static_cast<int>( iexSystem::ScreenWidth *  0.39f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.7f );
			checkBack->Render( x, y, w, h, 0, 0, 512, 512 );

			x = static_cast<int>( iexSystem::ScreenWidth * 0.34f );
			y = static_cast<int>( iexSystem::ScreenHeight * 0.62f );
			w = static_cast<int>( iexSystem::ScreenWidth *  0.31f );
			h = static_cast<int>( iexSystem::ScreenHeight * 0.13f );
			checkCursor->Render( x, y, w, h, 0, checkSelectInfo.select * 128, 512, 128 );
		}
	}

//-------------------------------------------------------------------------------
//	メイン移動動作
//-------------------------------------------------------------------------------

	//	メイン移動初期化
	void	sceneMenu::MoveMainInitialize( void )
	{
		screen->SetScreenMode( SCREEN_MODE::FADE_OUT, 1.5f );
	}

	//	メインへ
	void	sceneMenu::MoveMainUpdate( void )
	{
		if ( screen->GetScreenState() )
		{
			MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
			return;
		}
	}

	//	メイン移動描画
	void	sceneMenu::MoveMainRender( void )
	{

	}

//-------------------------------------------------------------------------------
//	タイトル移動動作
//-------------------------------------------------------------------------------

	//	タイトル移動初期化
	void	sceneMenu::MoveTitleInitialize( void )
	{
		screen->SetScreenMode( SCREEN_MODE::FADE_OUT, 1.0f );
	}

	//	タイトルへ
	void	sceneMenu::MoveTitleUpdate( void )
	{
		if ( screen->GetScreenState() )
		{
			MainFrame->ChangeScene( new sceneLoad( new sceneTitle() ) );
			return;
		}
	}

	//	タイトル移動描画
	void	sceneMenu::MoveTitleRender( void )
	{

	}

//-------------------------------------------------------------------------------
//	オプション関数
//-------------------------------------------------------------------------------
	//	オプション
	void	sceneMenu::OptionInitialize( void )
	{
		//　構造体初期化
		optionInfo.itemflg = true;
		optionInfo.coinMAX = 200;
		optionInfo.minute = 1;
		optionInfo.second = 30;
		optionInfo.step = 0;
		gameManager->SetItemFlg(optionInfo.itemflg);
		gameManager->SetCoinMax(optionInfo.coinMAX);
		gameManager->SetTime(optionInfo.minute, optionInfo.second);
	}

	void	sceneMenu::OptionUpdate( void )
	{
		if (KEY_Get(KEY_DOWN) == 3){
			if (optionInfo.step<3)
			optionInfo.step++;
		}
		if (KEY_Get(KEY_UP) == 3){
			if (optionInfo.step>0){
				optionInfo.step--;
			}
		}

		switch (optionInfo.step){
		case 0:
			if (KEY_Get(KEY_RIGHT) == 3 || KEY_Get(KEY_LEFT) == 3){
				if (optionInfo.itemflg == false){
					optionInfo.itemflg = true;
				}
				else{
					optionInfo.itemflg = false;
				}
			}
			break;
		case 1:
			if (KEY_Get(KEY_RIGHT) == 3 ){
				if (optionInfo.coinMAX<500){
					optionInfo.coinMAX += 50;
				}
			}
			else if (KEY_Get(KEY_LEFT) == 3){
				if (optionInfo.coinMAX>200){
					optionInfo.coinMAX -= 50;
				}
			}
			break;
		case 2:
			if (KEY_Get(KEY_RIGHT) == 3){
				if (optionInfo.minute<5){
					optionInfo.minute++;
				}
			}
			else if (KEY_Get(KEY_LEFT) == 3){
				if (optionInfo.minute>1){
					optionInfo.minute--;
				}
			}
			break;
		case 3:
			if (KEY_Get(KEY_RIGHT) == 3){
				optionInfo.second = 30;
			}
			else if (KEY_Get(KEY_LEFT) == 3){
				optionInfo.second = 0;
			}
			break;
		}
			gameManager->SetItemFlg(optionInfo.itemflg);
			gameManager->SetCoinMax(optionInfo.coinMAX);
			gameManager->SetTime(optionInfo.minute,optionInfo.second);
		if (KEY_Get(KEY_A) == 3){
			SetMode(tempmode);
		}
	}

	void	sceneMenu::OptionRender( void )
	{
		Oimage->Render(300, 150, 512, 128, 0, 128*2, 512, 128);
		Oimage->Render(300, 350, 512, 128, 0, 128*0, 512, 128);
		Oimage->Render(400, 550, 256, 128, 0, 128*1, 256, 128);

		//アイテムの有無描画
		if (optionInfo.itemflg){
			Oimage->Render(950, 150, 256, 128, 0, 128 * 3, 256, 128);
		}
		else{
			Oimage->Render(950, 150, 256, 128, 256, 128 * 3, 256, 128);
		}

		OCmax->Render(950,			350, 128, 128,			(optionInfo.coinMAX/100)*64, 128 * 0, 64, 64);
		OCmax->Render(950+128,		350, 128, 128,	((optionInfo.coinMAX / 10)%10) * 64, 128 * 0, 64, 64);
		OCmax->Render(950+(128*2),	350, 128, 128,									  0, 128 * 0, 64, 64);

		TimerRender();
	}
	void	sceneMenu::TimerRender(void)
	{
		//Oimage->Render(970, 550, 256, 128, 256, 128 * 1, 256, 128);
		OCmax->Render(950 , 550, 128, 128, optionInfo.minute*64, 128 * 0, 64, 64);
		OCmax->Render(950 + 128, 550, 128, 128, ((optionInfo.second / 10) % 10) * 64, 128 * 0, 64, 64);
		OCmax->Render(950 + (128 * 2), 550, 128, 128, 0, 0, 64, 64);

	}
//-------------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------------

	//	モード切り替え
	void	sceneMenu::SetMode( int mode )
	{
		//	初期化する
		switch ( mode )
		{
		case MENU_MODE::SELECT_PLAYERNUM:
			SelectPlayerNumInitialize();
			break;

		case MENU_MODE::SELECT_CHARACTER:
			SelectCharacterInitialize();
			break;

		case MENU_MODE::SELECT_STAGE:
			SelectStageInitialize();
			break;

		case MENU_MODE::SELECT_CHECK:
			SelectCheckInitialize();
			break;

		case MENU_MODE::MOVE_MAIN:
			MoveMainInitialize();
			break;
		case MENU_MODE::OPTION:
			OptionInitialize();
			break;
		}

		//	モード切替
		this->mode = mode;
	}

//-------------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------------