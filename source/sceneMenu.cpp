
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
		screen->SetScreenMode( SCREEN_MODE::WHITE_IN, 1.0f );

		//	画像読み込み
		back = make_unique<iex2DObj>( LPSTR( "DATA/UI/back.png" ) );
		face = make_unique<iex2DObj>( LPSTR( "DATA/UI/chara_emotion.png" ) );
		cursor = make_unique<iex2DObj>( LPSTR( "DATA/UI/cursor.png" ) );
		frame = make_unique<iex2DObj>( LPSTR( "DATA/UI/frame.png" ) );
		checkBack = make_unique<iex2DObj>( LPSTR( "DATA/UI/selectCheck_Back.png" ) );
		checkCursor = make_unique<iex2DObj>( LPSTR( "DATA/UI/selectCheck_Cursor.png" ) );
		playerNumText = make_unique<iex2DObj>( LPSTR( "DATA/UI/playerNum_Text.png" ) );
		playerNum = make_unique<iex2DObj>( LPSTR( "DATA/UI/playerNum.png" ) );

		//	モデル読み込み
		//org[CHARACTER_TYPE::KNIGHT] = new iex3DObj( "DATA/CHR/Knight/Knight_Dammy.IEM" );		//	騎士
		org[CHARACTER_TYPE::SCAVENGER] = new iex3DObj( "DATA/CHR/Knight/Knight_Dammy.IEM" );		//	掃除屋
		org[CHARACTER_TYPE::PRINCESS] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );					//	姫
		org[CHARACTER_TYPE::SQUIRREL] = new iex3DObj( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" );		//	リス
		org[CHARACTER_TYPE::TIGER] = new iex3DObj( "DATA/CHR/ECCMAN/ECCMAN.IEM" );				//	トラ
		deskStage = make_unique<iexMesh>( LPSTR( "DATA/back/stage.IMO" ) );
		forestStage = make_unique<iexMesh>( LPSTR( "DATA/BG/Forest/model/forest.IMO" ) );

		//	画像構造体初期化
		{
			textImage.obj = new iex2DObj( "DATA/UI/menu-head.png" );
			ImageInitialize( textImage, 640, 150, 370, 150, 512, 0, 512, 256 );
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
		Random::Release();

		for ( int i = 0; i < 4; i++ )
		{
			SafeDelete( org[i] );
		}
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
		back->Render( 0, 0, 1280, 720, 0, 0, 1280, 720 );
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
		playerNum->Render( 360, 290, 100, 110, 0, playerNumSelectInfo.sy, 128, 128 );
		playerNumText->Render( 450, 300, 500, 100, 0, 0, 512, 128 );
	}

//-------------------------------------------------------------------------------
//	キャラクター選択動作
//-------------------------------------------------------------------------------

	//	キャラクター選択初期化
	void	sceneMenu::SelectCharacterInitialize( void )
	{
		//	プレイヤーにもたせる、初期化
		for ( int i = 0; i < 4; i++ )
		{
			obj[i] = org[i];
			obj[i]->SetAngle( D3DX_PI );
			obj[i]->SetPos( -7.0f + ( 14.0f / 3.0f * i ), 0.0f, 0.0f );
			obj[i]->SetScale( 0.02f );
			obj[i]->Update();
		}

		//	選択情報初期化
		for ( int i = 0; i < 4; i++ )
		{
			characterSelectInfo.character[i] = i;
			characterSelectInfo.select[i] = false;
		}

		//	変数初期化
		characterSelectInfo.imagePos = 0;
		characterSelectInfo.step = 0;
		characterSelectInfo.timer = 0;
		characterSelectInfo.playerNum = gameManager->GetPlayerNum();

		//	画像位置初期化
		ImageInitialize( textImage, 640, 150, 370, 150, 512, 0, 512, 256 );
		textImage.renderflag = true;
	}

	//	キャラクター選択
	void	sceneMenu::SelectCharacterUpdate( void )
	{
		for ( int i = 0; i < characterSelectInfo.playerNum; i++ )
		{
			//	キャンセル
			if ( input[i]->Get( KEY_B ) == 3 )		characterSelectInfo.select[i] = false;

			//	選択済みだったらキャンセルのみ受付け
			if ( characterSelectInfo.select[i] )	continue;

			//	カーソル移動
			if ( input[i]->Get( KEY_RIGHT ) == 3 )	characterSelectInfo.character[i]++;
			if ( input[i]->Get( KEY_LEFT ) == 3 )		characterSelectInfo.character[i]--;

			//	上限・下限設定
			if ( characterSelectInfo.character[i] < 0 )	characterSelectInfo.character[i] = CHARACTER_TYPE::MAX - 1;
			if ( characterSelectInfo.character[i] >= CHARACTER_TYPE::MAX )	characterSelectInfo.character[i] = 0;

			//	決定
			if ( input[i]->Get( KEY_SPACE ) == 3 || input[i]->Get( KEY_A ) == 3 )
			{
				gameManager->SetCharacterType( i, characterSelectInfo.character[i] );
				characterSelectInfo.select[i] = true;
			}

			//	モデル差し替え
			obj[i] = org[characterSelectInfo.character[i]]->Clone();
		}

		//	選択チェック
		int		selectCheck = 0;
		for ( int i = 0; i < gameManager->GetPlayerNum(); i++ )
		{
			if ( characterSelectInfo.select[i] )	selectCheck++;
		}

		//	全員未選択
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
			for ( int i = gameManager->GetPlayerNum(); i < 4; i++ )
			{
				characterSelectInfo.character[i] = Random::GetInt( 0, CHARACTER_TYPE::MAX - 1 );
				gameManager->SetCharacterType( i, characterSelectInfo.character[i] );
			
				//	モデル差し替え
				obj[i] = org[characterSelectInfo.character[i]]->Clone();
			}
			SetMode( MENU_MODE::SELECT_STAGE );
		}

		//	モデルデータ更新
		for ( int i = 0; i < 4; i++ )
		{
			obj[i]->Animation();
			obj[i]->SetAngle( D3DX_PI );
			obj[i]->SetScale( 0.02f );
			obj[i]->SetPos( -7.0f + ( 14.0f / 3.0f * i ), 0.0f, 0.0f );
			obj[i]->Update();
		}
	}

	//	キャラクター選択描画
	void	sceneMenu::SelectCharacterRender( void )
	{
		//	モデル・顔・カーソル描画
		for ( int i = 0; i < 4; i++ )
		{
			//	モデル描画
			obj[i]->Render( shader3D, "toon" );
			
			//	顔描画
			int	 x = 230 + ( 670 / 3 * i );
			Vector3	color = Vector3( 1.0f, 1.0f, 1.0f );
			face->Render( x, 280, 150, 150, 0, 256 * i, 256, 256, GetColor( color ) );

			//	カーソル描画
			x = 230 + ( 670 / 3 * characterSelectInfo.character[i] );
			if ( !characterSelectInfo.select[i] )
				cursor->Render( x + 35 * i, 250, 50, 50, 128 * ( i % 2 ), 128 * ( i / 2 ), 128, 128 );
		}

		//	テキスト画像
		RenderImage( textImage, 512, 0, 512, 256, IMAGE_MODE::ADOPTPARAM );
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
		forestStage->SetPos( 5.0f, 3.0f, 0.0f );
		forestStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
		forestStage->SetScale( 0.04f );
		forestStage->Update();

		//	パラメータ初期化
		stageSelectInfo.angle = D3DX_PI;
		stageSelectInfo.stage = 0;

		//	画像位置初期化
		ImageInitialize( textImage, 640, 150, 370, 150, 0, 256, 512, 256 );
		textImage.angle = D3DXToRadian( 5.0f );
		textImage.renderflag = true;
	}

	//	ステージ選択
	void	sceneMenu::SelectStageUpdate( void )
	{
		//	ステージ回転
		stageSelectInfo.angle += 0.01f;

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
		//	テキスト画像
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		RenderImage( textImage, 0, 256, 512, 256, IMAGE_MODE::ADOPTPARAM );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		deskStage->Render();
		forestStage->Render();

		//	テキスト画像
		RenderImage( textImage, 0, 256, 512, 256, IMAGE_MODE::ADOPTPARAM );
	}

//-------------------------------------------------------------------------------
//	最終チェック動作
//-------------------------------------------------------------------------------

	//	最終確認初期化
	void	sceneMenu::SelectCheckInitialize( void )
	{
		//	プレイヤーにもたせる、初期化
		for ( int i = 0; i < 4; i++ )
		{
			obj[i] = org[gameManager->GetCharacterType( i )]->Clone();
			obj[i]->SetAngle( D3DX_PI );
			obj[i]->SetPos( -7.0f + ( 14.0f / 3.0f * i ), 0.0f, 0.0f );
			obj[i]->SetScale( 0.02f );
			obj[i]->Update();
		}

		//	構造体初期化
		{
			checkSelectInfo.check = false;
			checkSelectInfo.select = false;
			checkSelectInfo.step = 0;
		}

		//	ステージ座標設定
		{
			deskStage->SetPos( 0.0f, 5.0f, 0.0f );
			deskStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			deskStage->SetScale( 0.08f );
			deskStage->Update();
			forestStage->SetPos( 0.0f, 5.0f, 0.0f );
			forestStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			forestStage->SetScale( 0.03f );
			forestStage->Update();
		}
	}

	//	最終確認
	void	sceneMenu::SelectCheckUpdate( void )
	{
		//	決定
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

		//	選択
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
			//	キャンセル
			if ( input[0]->Get( KEY_B ) == 3 )
			{
				SetMode( MENU_MODE::SELECT_STAGE );
			}
		}

		//	モデル更新
		for ( int i = 0; i < 4; i++ )
		{
			obj[i]->Animation();
			obj[i]->Update();
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
		for ( int i = 0; i < 4; i++ )
		{
			obj[i]->Render( shader3D, "toon" );
		}

		//	テキスト画像描画
		RenderImage( textImage, 0, 512, 512, 256, IMAGE_MODE::ADOPTPARAM );
		
		//	チェック項目描画
		if ( checkSelectInfo.check )
		{
			checkBack->Render( 390, 120, 500, 500, 0, 0, 512, 512 );
			checkCursor->Render( 440, 450, 400, 100, 0, checkSelectInfo.select * 128, 512, 128 );
		}
	}

//-------------------------------------------------------------------------------
//	メイン移動動作
//-------------------------------------------------------------------------------

	//	メイン移動初期化
	void	sceneMenu::MoveMainInitialize( void )
	{
		screen->SetScreenMode( SCREEN_MODE::WHITE_OUT, 1.0f );
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
		screen->SetScreenMode( SCREEN_MODE::WHITE_OUT, 1.0f );
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
		}

		//	モード切替
		this->mode = mode;
	}

//-------------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------------