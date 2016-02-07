
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
#include	"Curtain.h"
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
		namespace CAMERA_TARGET
		{
			Vector3 front(0.0f, 0.0f, -1.0f);
			Vector3 back(0.0f, 0.0f, 1.0f);
			Vector3 right(-1.0f, 0.0f, 0.0f);
			Vector3 left(1.0f, 0.0f, 0.0f);
			Vector3 up(0.0f, 1.0f, 0.0f);
		}
		namespace CHARA_ANGLE
		{
			float CharaSelectPlayer_ANGLE[4] = {
				(D3DXToRadian(60.0f)),
				(D3DXToRadian(80.0f)),
				(D3DXToRadian(100.0f)),
				(D3DXToRadian(120.0f)),
			};

			float SelectCheckPlayer_ANGLE[4] = {
				(D3DXToRadian(-120.0f)),
				(D3DXToRadian(-100.0f)),
				(D3DXToRadian(-80.0f)),
				(D3DXToRadian(-60.0f)),
			};

		}
		namespace CHECK_MODE
		{
			enum
			{
				SELECT,
				MOVETARGET,
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
		//背景用構造体初期化
		bgInfo.t = 1.0f;
		bgInfo.mt = 0.0f;
		mainView = new Camera();
		mainView->SetProjection( D3DXToRadian( 70.0f ), 1.0f, 1000.0f );
		bgInfo.cpos = Vector3(0.0f, 15.0f, 0.0f);
		bgInfo.cspos = bgInfo.cpos;
		bgInfo.cepos = Vector3( 1.0f,30.0f, 0.0f );
		bgInfo.target = CAMERA_TARGET::front;
		bgInfo.start	= bgInfo.target;
		bgInfo.end		= bgInfo.target;
		bgInfo.moveflg = false;
		mainView->Set( bgInfo.cpos, bgInfo.target );

		changeScene = false;

		//	random
		Random::Initialize();

		//	screen
		screen->SetScreenMode( SCREEN_MODE::WIPE_IN, 1.5f );

		//	ゲームマネージャ初期化
		gameManager->Initialize();
		OptionInitialize();
	
		//	画像読み込み
		back = make_unique<iex2DObj>( LPSTR( "DATA/UI/back.png" ) );
		frame = make_unique<iex2DObj>( LPSTR( "DATA/UI/frame.png" ) );
		checkBack = make_unique<iex2DObj>( LPSTR( "DATA/UI/menu/selectCheck_Back.png" ) );
		checkCursor = make_unique<iex2DObj>( LPSTR( "DATA/UI/menu/selectCheck_Cursor.png" ) );
		playerNumText = make_unique<iex2DObj>( LPSTR( "DATA/UI/menu/playerNum_Text.png" ) );
		playerNum = make_unique<iex2DObj>( LPSTR( "DATA/UI/menu/playerNum.png" ) );
		face = new iex2DObj( "DATA/UI/chara_emotion.png" );
		cursor = new iex2DObj( "DATA/UI/cursor.png" );
		decidecursor = new iex2DObj("DATA/UI/decide-cursor.png");
		cpuCursor = new iex2DObj( "DATA/UI/cpuIcon.png" );
		selectCheckCursor = new iex2DObj("DATA/UI/menu/cursor.png");
		triangleCursor = new iex2DObj("DATA/UI/tryangle.png");
		
		//	オプション関係画像読み込み
		optionImage =		new iex2DObj( "DATA/UI/OptionText.png" );
		optionTime	=		new iex2DObj( "DATA/UI/number.png" );
		optionLife	=		new iex2DObj( "DATA/UI/number.png" );
		optionMenu = new iex2DObj("DATA/UI/option-int.png");

		//	モデル読み込み
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/majo/majo.IEM" ) );		//	掃除屋
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/プリンセス/prin2.IEM" ) );					//	姫
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Thief/Thief.IEM"));		//	怪盗
		org[CHARACTER_TYPE::PIRATE] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Pirate/Pirate.IEM"));				//	海賊
		orgCannon = make_unique<iexMesh>( LPSTR( "DATA/CHR/Pirate/pirate_cannon.IMO" ) );
		FOR( 0, PLAYER_MAX )
		{
			cannon[value] = orgCannon->Clone();
		}

		//	オリジナルモデル情報初期化
		org[CHARACTER_TYPE::SCAVENGER]->SetScale( 0.015f );	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetScale( 0.04f );		//	姫
		org[CHARACTER_TYPE::THIEF]->SetScale(0.025f);		//	怪盗
		org[CHARACTER_TYPE::PIRATE]->SetScale(0.04f);			//	海賊

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle( D3DX_PI );	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetAngle( D3DX_PI );	//	姫
		org[CHARACTER_TYPE::THIEF]->SetAngle( D3DX_PI );	//	怪盗
		org[CHARACTER_TYPE::PIRATE]->SetAngle( D3DX_PI );			//	海賊

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion( 0 );	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 0 );		//	姫
		org[CHARACTER_TYPE::THIEF]->SetMotion( 0 );		//	怪盗
		org[CHARACTER_TYPE::PIRATE]->SetMotion( 0 );			//	海賊

		org[CHARACTER_TYPE::SCAVENGER]->Update();	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->Update();		//	姫
		org[CHARACTER_TYPE::THIEF ]->Update();		//	怪盗
		org[CHARACTER_TYPE::PIRATE]->Update();			//	海賊

		FOR( 0, PLAYER_MAX )
		{
			modelPos[value] = Vector3( 0.0f, 0.0f, 0.0f );
			modelAngle[value] = Vector3( 0.0f, 0.0f, 0.0f );
			modelScale[value] = Vector3( 0.0f, 0.0f, 0.0f );
			obj[value] = org[value]->Clone();
		}

		deskStage = make_unique<iexMesh>( LPSTR( "DATA/BG/stage-desk/stage.IMO" ) );
		toyStage = make_unique<iexMesh>(LPSTR("DATA/BG/stage_toy/stage_toy.IMO"));
		BG = make_unique<iexMesh>( LPSTR( "DATA/BG/MenuStage/menustage.IMO" ) );

		//	机モデル初期化
		BG->SetPos(0.0f, 0.0f, -1.0f);
		BG->SetAngle(0.0f, 0.0f, 0.0f);
		BG->SetScale(0.1f);
		BG->Update();
		//	画像構造体初期化
		{
			textImage.obj = new iex2DObj( "DATA/UI/menu/menu-head.png" );
			int x = static_cast<int>( iexSystem::ScreenWidth / 2 );
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.15f );
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.29f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
			ImageInitialize( textImage, x, y, w, h, 512, 0, 512, 256 );
			//textImage.angle = D3DXToRadian( 5.0f );
			textImage.renderflag = true;
		}

		//	モード設定
		tempmode = 0;
		SetMode( MENU_MODE::INIT );

		//BGM設定再生
		sound->PlayBGM(BGM::MENU_BGM);

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
		SafeDelete(decidecursor);
		SafeDelete( selectCheckCursor );
		SafeDelete( cpuCursor );
		SafeDelete(triangleCursor);
		Random::Release();
		sound->AllStop();

		//設計中
		SafeDelete(optionImage);
		SafeDelete(optionTime);
		SafeDelete(optionLife);
		SafeDelete(optionMenu);
	}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------

	//	更新
	void	sceneMenu::Update( void )
	{
		if ( screen->GetScreenState() )
		{
			//	オプションとの切り替え
			ChangeToOption();
		}
		//	カメラ更新
		CameraUpdate();	

		//	各モード更新
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
			if ( changeScene )
			{
				MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
				return;
			}

			break;

		case MENU_MODE::MOVE_TITLE:
			MoveTitleUpdate();
			if ( changeScene )
			{
				MainFrame->ChangeScene( new sceneLoad( new sceneTitle() ) );
				return;
			}
			break;

		case MENU_MODE::OPTION:
			OptionUpdate();
			break;
		}

		//	モデル更新
		ModelUpdate();

		//	スクリーン更新
		screen->Update();
	}

	//	描画
	void	sceneMenu::Render( void )
	{
		//	camera
		mainView->Activate();
		mainView->Clear(0xFF02B4FE);

		//	ライト設定
		dir = bgInfo.target;
		shader3D->SetValue("DirLightVec", dir);
		iexLight::DirLight(shader, 0, &dir, 1.0f, 1.0f, 1.0f);
		dir.Normalize();

		//	背景描画
		BG->Render();

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
		if (mode != MENU_MODE::OPTION){
			optionMenu->Render(80, 50, 256, 64, 0, 0, 256, 64);
		}
		
		//	スクリーン
		screen->Render();
	}

	//	カメラ更新
	void	sceneMenu::CameraUpdate( void )
	{
		//	カメラ用パラメータ加算
		bgInfo.t += 0.02f;
		if ( bgInfo.t >= 1.0f )	bgInfo.t = 1.0f;

		//	回転補間
		CubicFunctionInterpolation( bgInfo.target, bgInfo.start, bgInfo.end, bgInfo.t );

		//	カメラ更新
		mainView->Set( bgInfo.cpos, bgInfo.cpos + bgInfo.target );
	}

	//	オプションに切り替え＆その逆
	void	sceneMenu::ChangeToOption( void )
	{
		if ( KEY_Get( KEY_B6 ) == 3 )
		{
			//	現在のモードを保存してオプションへ移行
			if ( mode != MENU_MODE::OPTION )
			{
				tempmode = mode;
				SetMode( MENU_MODE::OPTION );
			}
			else
			{
				//	保存しておいたモードへ移行
				SetMode( tempmode );
			}
		}
	}

	//	モデル更新
	void	sceneMenu::ModelUpdate( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->SetPos( modelPos[value] );
			obj[value]->SetAngle( modelAngle[value] );
			obj[value]->Animation();
			obj[value]->Update();
		}

		//	大砲位置設定
		SetCannonPos();
	}

	//	大砲描画
	void	sceneMenu::CannonRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			if ( characterSelectInfo.character[value] == CHARACTER_TYPE::PIRATE )
			{
				cannon[value]->Render( shader3D, "toon" );
			}
		}
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
		if (bgInfo.t < 1.0f)return;
		//	パラメータ加算
		playerNumSelectInfo.t += 0.08f;
		if ( playerNumSelectInfo.t >= 1.0f )	playerNumSelectInfo.t = 1.0f;	

		//	移動が終わったら
		if ( playerNumSelectInfo.t >= 1.0f )
		{
			//	選択
			if ( input[0]->Get( KEY_DOWN ) == 1 || input[0]->Get( KEY_AXISY ) < 0 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				//	元の座標を保存
				playerNumSelectInfo.saveY =  128 * playerNumSelectInfo.num;
				playerNumSelectInfo.num++;
				if ( playerNumSelectInfo.num >= 4 )
				{
					playerNumSelectInfo.num = -1;
					playerNumSelectInfo.t = 1.0f;
				}
				else
				{
					playerNumSelectInfo.t = 0.0f;
				}
			}

			if ( input[0]->Get( KEY_UP ) == 1 || input[0]->Get( KEY_AXISY ) > 0 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				//	元の座標を保存
				playerNumSelectInfo.saveY = 128 * playerNumSelectInfo.num;
				playerNumSelectInfo.num--;
				if ( playerNumSelectInfo.num < 0 )
				{
					playerNumSelectInfo.num = 4;
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
				sound->PlaySE( SE::DECIDE_SE );
				gameManager->SetPlayerNum( playerNumSelectInfo.num + 1 );
				bgInfo.start = CAMERA_TARGET::front;
				bgInfo.end = CAMERA_TARGET::right;
				bgInfo.t = 0.0f;
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
		if (bgInfo.t < 1.0f)return;
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.28f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.4f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.08f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.15f );
		playerNum->Render( x, y, w, h, 0, playerNumSelectInfo.sy, 128, 128 );

		triangleCursor->Render( ( int )( x+10.0f ), ( int )( y - 130.0f ), 128, 128, 0, 0, 256, 256);
		triangleCursor->Render( ( int )( x + 10.0f ), ( int )( y + 140.0f ), 128, 128, 256, 0, 256, 256);

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
		org[CHARACTER_TYPE::SCAVENGER]->SetMotion(0);	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetMotion(0);		//	姫
		org[CHARACTER_TYPE::THIEF]->SetMotion(0);		//	怪盗
		org[CHARACTER_TYPE::PIRATE]->SetMotion(0);			//	海賊
	
		//	顔画像初期化
		FOR( 0, CHARACTER_TYPE::MAX )
		{
			int left = static_cast<int>( iexSystem::ScreenWidth / 5 );
			int x = left + left * value;
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.37f );
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.155f );
			ImageInitialize( faceImage[value], x, y, w, h, 0, 256 * value, 256, 256 );
			faceImage[value].obj = face;
		}

		//	モデル、選択情報初期化
		FOR( 0, PLAYER_MAX )
		{
			//	モデル登録、初期化
			obj[value] = org[value]->Clone();
			modelAngle[value] = Vector3( 0.0f, D3DXToRadian( 100.0f ), 0.0f );

			//	選択情報初期化
			characterSelectInfo.t[value] = 1.0f;
			characterSelectInfo.character[value] = value;
			characterSelectInfo.select[value] = false;
			
			//	カーソル初期化
			int w = static_cast<int>(iexSystem::ScreenWidth * (0.04f));
			int h = static_cast<int>(iexSystem::ScreenHeight * (0.07f));
			ImageInitialize(cursorImage[value], 0, 0, w, h, 0, 0, 0, 0);
			ImageInitialize(decidecursorImage[value], 0, 0, w, h, 0, 0, 0, 0);
			cursorImage[value].obj = cursor;
			decidecursorImage[value].obj = decidecursor;
			if ( value >= gameManager->GetPlayerNum() )
			{
				cursorImage[value].renderflag = false;
			}
			
			//	カーソル座標初期化
			characterSelectInfo.startPos[value] = faceImage[characterSelectInfo.character[value]].x;
			characterSelectInfo.endPos[value] = faceImage[characterSelectInfo.character[value]].x;
			cursorImage[value].x = faceImage[characterSelectInfo.character[value]].x;
			cursorImage[value].y = faceImage[characterSelectInfo.character[value]].y;

			//	波紋終了フラグ
			characterSelectInfo.isWaveEnd[value] = false;
		}
	
		//	変数初期化
		characterSelectInfo.imagePos = 0;
		characterSelectInfo.step = 0;
		characterSelectInfo.timer = 0;
		characterSelectInfo.playerNum = gameManager->GetPlayerNum();

		//	画像位置初期化
		//textImage.angle = D3DXToRadian( 5.0f );
		textImage.renderflag = true;

	}

	//	キャラクター選択
	void	sceneMenu::SelectCharacterUpdate( void )
	{
		if ( bgInfo.t < 1.0f )return;


		//	各プレイヤー動作
		FOR( 0, characterSelectInfo.playerNum )
		{
			characterSelectInfo.isWaveEnd[value] = false;

			//	パラメータ加算
			characterSelectInfo.t[value] += 0.08f;
			if ( characterSelectInfo.t[value] >= 1.0f )	characterSelectInfo.t[value] = 1.0f;

			//	移動補間
			CubicFunctionInterpolation( cursorImage[value].x, characterSelectInfo.startPos[value], characterSelectInfo.endPos[value], characterSelectInfo.t[value] );
			cursorImage[value].y = faceImage[characterSelectInfo.character[value]].y;

			//	補間終了してなかったらスキップ
			if ( characterSelectInfo.t[value] < 1.0f )	continue;

			//	キャンセル
			if ( input[value]->Get( KEY_B ) == 3 )		characterSelectInfo.select[value] = false;

			//	選択済みだったらキャンセルのみ受付け
			if ( characterSelectInfo.select[value] )	continue;

			//	カーソル移動
			if ( input[value]->Get( KEY_RIGHT ) == 1 || input[value]->Get( KEY_AXISX ) > 0 )	
			{
				//	効果音
				sound->PlaySE( SE::CHOICE_SE );
				
				//	現在位置を保存
				characterSelectInfo.startPos[value] = faceImage[characterSelectInfo.character[value]].x;

				//	パラメータ加算
				characterSelectInfo.character[value]++;

				//	上限設定
				if ( characterSelectInfo.character[value] >= CHARACTER_TYPE::MAX )	characterSelectInfo.character[value] = 0;

				//	カーソル位置設定
				characterSelectInfo.endPos[value] = faceImage[characterSelectInfo.character[value]].x;

				//	モデル差し替え
				obj[value] = org[characterSelectInfo.character[value]]->Clone();

				//	移動パラメータ初期化
				characterSelectInfo.t[value] = 0.0f;
			}

			if ( input[value]->Get( KEY_LEFT ) == 1 || input[value]->Get( KEY_AXISX ) < 0 )
			{
				//	効果音
				sound->PlaySE( SE::CHOICE_SE );

				//	現在位置を保存
				characterSelectInfo.startPos[value] = faceImage[characterSelectInfo.character[value]].x;

				//	減算
				characterSelectInfo.character[value]--;

				//	下限設定
				if ( characterSelectInfo.character[value] < 0 )	characterSelectInfo.character[value] = CHARACTER_TYPE::MAX - 1;

				//	カーソル位置設定
				characterSelectInfo.endPos[value] = faceImage[characterSelectInfo.character[value]].x;

				//	モデル差し替え
				obj[value] = org[characterSelectInfo.character[value]]->Clone();

				//	移動パラメータ初期化
				characterSelectInfo.t[value] = 0.0f;
			}

			//	決定
			if ( input[value]->Get( KEY_SPACE ) == 3 || input[value]->Get( KEY_A ) == 3 )
			{
				sound->PlaySE( SE::DECIDE_SE );
				gameManager->SetCharacterType( value, characterSelectInfo.character[value] );
				SetWave( cursorImage[value], 1.5f );
				characterSelectInfo.select[value] = true;
			}
		}

		//	選択済み人数をカウント&波紋更新
		int		selectCheck = 0;
		FOR( 0, gameManager->GetPlayerNum() )
		{
			if ( characterSelectInfo.select[value] )	selectCheck++;
			characterSelectInfo.isWaveEnd[value] = WaveUpdate( cursorImage[value], 80 );
		}

		//	全員未選択時にキャンセルボタンを押すとプレイ人数選択へ移行
		if ( selectCheck == 0 )
		{
			if ( KEY( KEY_B ) == 3 )
			{
				bgInfo.t = 0.0f;
				bgInfo.start = CAMERA_TARGET::right;
				bgInfo.end = CAMERA_TARGET::front;
				SetMode( MENU_MODE::SELECT_PLAYERNUM );
			}
		}


		//	データ更新
		FOR( 0, PLAYER_MAX )
		{
			modelPos[value] = Vector3( -12.0f, 10.0f, -9.0f + 6.0f * value );
			modelAngle[value] = Vector3( 0.0f, CHARA_ANGLE::CharaSelectPlayer_ANGLE[value], 0.0f );
		}

		//	全員選択済み
		int	isWaveEnd = 0;
		if ( selectCheck >= gameManager->GetPlayerNum() )
		{
			FOR( 0, PLAYER_MAX )
			{
				if ( characterSelectInfo.isWaveEnd[value] )	isWaveEnd++;
			}

			if ( isWaveEnd < gameManager->GetPlayerNum() )	return;

			//	コンピュータ分を適当に設定
			FOR( gameManager->GetPlayerNum(), PLAYER_MAX )
			{
				characterSelectInfo.character[value] = Random::GetInt( 0, CHARACTER_TYPE::MAX - 1 );
				//　デバッグ
				//characterSelectInfo.character[0] = CHARACTER_TYPE::SCAVENGER;
				characterSelectInfo.character[1] = CHARACTER_TYPE::PRINCESS;
				characterSelectInfo.character[2] = CHARACTER_TYPE::THIEF;
				//characterSelectInfo.character[3] = CHARACTER_TYPE::SCAVENGER;
				characterSelectInfo.character[3] = CHARACTER_TYPE::PIRATE;
				gameManager->SetCharacterType( value, characterSelectInfo.character[value] );
			
				//	モデル差し替え
				obj[value] = org[characterSelectInfo.character[value]]->Clone();
			}

			//	ステージ選択へ
			bgInfo.t = 0.0f;
			bgInfo.start = bgInfo.end;
			bgInfo.end = CAMERA_TARGET::back;
			SetMode( MENU_MODE::SELECT_STAGE );
		}
	}

	//	キャラクター選択描画
	void	sceneMenu::SelectCharacterRender( void )
	{
		if (bgInfo.t < 1.0f)return;
		//	テキスト画像
		RenderImage( textImage, 512, 0, 512, 256, IMAGE_MODE::ADOPTPARAM );

		//	モデル
		FOR( 0, PLAYER_MAX )
		{
			//	モデル描画
			obj[value]->Render( shader3D, "toon" );
		}

		CannonRender();
		
		FOR( 0, CHARACTER_TYPE::MAX )
		{
			//	顔描画
			RenderImage( faceImage[value], 0, 256 * value, 256, 256, IMAGE_MODE::NORMAL );
		}

		//プレイヤー1
		cursorImage[0].x -= faceImage[characterSelectInfo.character[0]].w / 2;
		cursorImage[0].y -= faceImage[characterSelectInfo.character[0]].h / 2;
		if (characterSelectInfo.select[0])	
		{
			decidecursorImage[0].x = cursorImage[0].x;	//	決定時明度下げる
			decidecursorImage[0].y = cursorImage[0].y;
		}
		

		//プレイヤー2
		cursorImage[1].x += faceImage[characterSelectInfo.character[1]].w / 2;
		cursorImage[1].y -= faceImage[characterSelectInfo.character[1]].h / 2;
		if (characterSelectInfo.select[1])	{
			decidecursorImage[1].x = cursorImage[1].x;	//	決定時明度下げる
			decidecursorImage[1].y = cursorImage[1].y;
		}


		//プレイヤー3
		cursorImage[2].x -= faceImage[characterSelectInfo.character[2]].w / 2;
		cursorImage[2].y += faceImage[characterSelectInfo.character[2]].h / 2;
		if (characterSelectInfo.select[2])	{
			decidecursorImage[2].x = cursorImage[2].x;	//	決定時明度下げる
			decidecursorImage[2].y = cursorImage[2].y;
		}

		//プレイヤー4
		cursorImage[3].x += faceImage[characterSelectInfo.character[3]].w / 2;
		cursorImage[3].y += faceImage[characterSelectInfo.character[3]].h / 2;
		if (characterSelectInfo.select[3])	{
			decidecursorImage[3].x = cursorImage[3].x;	//	決定時明度下げる
			decidecursorImage[3].y = cursorImage[3].y;
		}

		FOR( 0, PLAYER_MAX )
		{
			if ( characterSelectInfo.select[value] )
			{
				RenderImage( decidecursorImage[value], 128 * (value % 2), 128 * (value / 2), 128, 128, IMAGE_MODE::NORMAL);
				RenderImage( cursorImage[value], 128 * ( value % 2 ), 128 * ( value / 2 ), 128, 128, IMAGE_MODE::WAVE );
			}
			else
			{
				RenderImage( cursorImage[value], 128 * (value % 2), 128 * (value / 2), 128, 128, IMAGE_MODE::NORMAL );
			}
		}
		
	}

//-------------------------------------------------------------------------------
//	ステージ選択動作
//-------------------------------------------------------------------------------

	//	ステージ選択初期化
	void	sceneMenu::SelectStageInitialize( void )
	{
		//	机モデル初期化
		deskStage->SetPos( 0.0f,0.0f,0.0f );
		deskStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
		deskStage->SetScale( 0.1f );
		deskStage->Update();

		
		//　おもちゃモデル初期化
		toyStage->SetPos(0.0f,0.0f,0.0f);
		toyStage->SetAngle(D3DXToRadian(30.0f), D3DXToRadian(-80.0f), 0.0f);
		toyStage->SetScale(0.09f);
		toyStage->Update();

		//	パラメータ初期化
		stageSelectInfo.angle = D3DX_PI;
		stageSelectInfo.stage = 0;
	}

	//	ステージ選択
	void	sceneMenu::SelectStageUpdate( void )
	{
		if (bgInfo.t < 1.0f)return;
		//	ステージ回転
		static float atai = 0.01f;
		stageSelectInfo.angle += atai;
		if (stageSelectInfo.angle>3.8f || stageSelectInfo.angle < 2.4f)
		{
			atai *= -1.0f;
		}//stageSelectInfo.angle = 3.5f;
		//stageSelectInfo.angle = 1.8f;

		//	ステージの向きを設定(選択中回転)
		switch ( stageSelectInfo.stage )
		{
		case 0:
			deskStage->SetPos(-2.0f, 14.0f, 8.0f);
			toyStage->SetPos(7.0f, 14.0f, 15.0f);
			deskStage->SetAngle( D3DXToRadian( 30.0f ), stageSelectInfo.angle, 0.0f );
			toyStage->SetAngle(D3DXToRadian(30.0f), D3DX_PI, 0.0f);
			break;

		case 1:
			deskStage->SetPos(-7.0f, 14.0f, 15.0f);
			toyStage->SetPos(2.0f, 13.0f, 8.0f);
			deskStage->SetAngle(D3DXToRadian(30.0f), D3DX_PI, 0.0f);
			toyStage->SetAngle(D3DXToRadian(30.0f), stageSelectInfo.angle, 0.0f);
			break;
		}

		//	左右で選択
		{
			if ( KEY( KEY_RIGHT ) == 3 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				stageSelectInfo.angle = D3DX_PI;
				stageSelectInfo.stage++;
			}

			if ( KEY( KEY_LEFT ) == 3 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				stageSelectInfo.angle = D3DX_PI;
				stageSelectInfo.stage--;
			}

			//	上限・下限設定
			if ( stageSelectInfo.stage >= 2 )	stageSelectInfo.stage = 0;
			if ( stageSelectInfo.stage < 0 )		stageSelectInfo.stage = 1;
		}

		//	決定
		if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
		{
			sound->PlaySE( SE::DECIDE_SE );
			//	マネージャーに情報をセット
			gameManager->SetStageType( stageSelectInfo.stage );

			//	モデルの角度を戻す
			deskStage->SetAngle( D3DXToRadian( 5.0f ), D3DX_PI, 0.0f );
			toyStage->SetAngle(D3DXToRadian(5.0f), D3DX_PI, 0.0f);

			//	次のモードへ
			bgInfo.t = 0.0f;
			bgInfo.start = bgInfo.end;
			bgInfo.end = CAMERA_TARGET::left;
			SetMode( MENU_MODE::SELECT_CHECK );
		}

		//	キャンセル
		if ( KEY( KEY_B ) == 3 )
		{
			bgInfo.t = 0.0f;
			bgInfo.start = bgInfo.end;
			bgInfo.end = CAMERA_TARGET::right;
			SetMode( MENU_MODE::SELECT_CHARACTER );
		}

		//	ステージ更新
		deskStage->Update();
		toyStage->Update();
	}

	//	ステージ選択描画
	void	sceneMenu::SelectStageRender( void )
	{
		if (bgInfo.t < 1.0f)return;
		//	テキスト画像描画(背面に描画)
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		RenderImage( textImage, 0, 256, 512, 256, IMAGE_MODE::ADOPTPARAM );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		deskStage->Render();
		toyStage->Render();
	}

//-------------------------------------------------------------------------------
//	最終チェック動作
//-------------------------------------------------------------------------------

	//	最終確認初期化
	void	sceneMenu::SelectCheckInitialize( void )
	{
		//	キャラ別モーション設定
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 0 );
		
		//	各プレイヤーモデル初期化
		static	Vector3	cursorPos[4];
		FOR( 0, PLAYER_MAX )
		{
			obj[value] = org[gameManager->GetCharacterType( value )]->Clone();
			modelPos[value] = Vector3( 10.0f, 14.0f, 4.0f - 3.0f * value );
			obj[value]->SetPos( modelPos[value] );
			obj[value]->Update();
			WorldToClient( obj[value]->GetPos(), cursorPos[value], matView * matProjection );
		}

		//	構造体初期化
		{
			checkSelectInfo.check = false;
			checkSelectInfo.select = false;
			checkSelectInfo.step = 0;
		}

		//	ステージ座標、向き設定
		{
			deskStage->SetPos( 7.0f, 15.5f, 2.5f );
			deskStage->SetAngle(D3DXToRadian(45.0f), D3DXToRadian(-90.0f), 0.0f);
			deskStage->SetScale( 0.03f );
			deskStage->Update();
		
			toyStage->SetPos(7.0f, 15.5f, 2.5f);
			toyStage->SetAngle(D3DXToRadian(45.0f), D3DXToRadian(-90.0f), 0.0f);
			toyStage->SetScale(0.02f);
			toyStage->Update();
		}

		//	カーソル初期化
		{
			FOR( 0, PLAYER_MAX )
			{
				//	座標初期化
				cursorImage[value].x = static_cast<int>( cursorPos[value].x - ( iexSystem::ScreenWidth * 0.05f ) );
				cursorImage[value].y = static_cast<int>( cursorPos[value].y - ( iexSystem::ScreenHeight * 0.3f ) );
				cursorImage[value].renderflag = true;
				
				//	CPUかプレイヤーで画像差し替え
				if ( value >= gameManager->GetPlayerNum() )
					cursorImage[value].obj = cpuCursor;
			}
		}
	}

	//	最終確認
	void	sceneMenu::SelectCheckUpdate( void )
	{
		Vector3	vec;	//	カメラから移動先へのベクトル保存用
		if ( bgInfo.t < 1.0f )return;
		switch (checkSelectInfo.step)
		{
		case CHECK_MODE::SELECT:
		//	決定（はい：メインへ、いいえ：キャラ選択へ）
		if ( input[0]->Get( KEY_A ) == 3 || input[0]->Get( KEY_SPACE ) == 3 )
		{
			sound->PlaySE( SE::DECIDE_SE );
			//	確認表示
			if ( !checkSelectInfo.check )
			{
				checkSelectInfo.check = true;
			}
			else
			{
				//	はい・いいえ
				if ( !checkSelectInfo.select )	//	はい
				{
					bgInfo.t = 0.0f;
					bgInfo.start = bgInfo.end;
					vec = bgInfo.cepos - bgInfo.cpos;
					vec.Normalize();
					bgInfo.end = vec;
					checkSelectInfo.step = CHECK_MODE::MOVETARGET;
				}				
				else //いいえ
				{
					bgInfo.t = 0.0f;
					bgInfo.start = CAMERA_TARGET::left;
					bgInfo.end = CAMERA_TARGET::back;
					SetMode( MENU_MODE::SELECT_STAGE );
				}
			}
		}


		//	選択中動作(カーソル移動, キャンセル)
		if ( checkSelectInfo.check )
		{
			if ( input[0]->Get( KEY_RIGHT ) == 3 || input[0]->Get( KEY_LEFT ) == 3 )
			{
				sound->PlaySE(SE::CHOICE_SE);
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
				bgInfo.t = 0.0f;
				bgInfo.start = CAMERA_TARGET::left;
				bgInfo.end = CAMERA_TARGET::back;
				SetMode( MENU_MODE::SELECT_STAGE );
				return;
			}
		}

		//	モデル更新
		FOR( 0, PLAYER_MAX )
		{
			modelPos[value] = Vector3( 12.0f, 10.0f, 8.0f - 6.0f * value );
			modelAngle[value] = Vector3( 0.0f, CHARA_ANGLE::SelectCheckPlayer_ANGLE[value], 0.0f );
		}
		break;

		case  CHECK_MODE::MOVETARGET:
			if (bgInfo.t >= 1.0f)
			{
				SetMode(MENU_MODE::MOVE_MAIN);
			}
			break;
		}
	}

	//	最終確認描画
	void	sceneMenu::SelectCheckRender( void )
	{
		if (bgInfo.t < 1.0f)return;
		//	ステージ描画
		switch ( gameManager->GetStageType() )
		{
		case 0:		deskStage->Render();		break;
		case 1:		toyStage->Render();		break;
		}
		
		//	大砲描画
		CannonRender();

		//	プレイヤー描画
		FOR( 0, PLAYER_MAX )
		{
			//	モデル描画
			obj[value]->Render( shader3D, "toon" );

			if ( value < gameManager->GetPlayerNum() )
				RenderImage( cursorImage[value], 128 * ( value % 2 ), 128 * ( value / 2 ), 128, 128, IMAGE_MODE::NORMAL );
			else
				RenderImage( cursorImage[value], 0, 0, 64, 64, IMAGE_MODE::NORMAL );
		}

		//	オプション確認描画
		OptionSelectRender();

		//	テキスト画像描画
		RenderImage( textImage, 0, 512, 512, 256, IMAGE_MODE::ADOPTPARAM );
		
		//	チェック項目描画
		if ( checkSelectInfo.check )
		{
			//	確認の背景描画
			int x = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.17f );
			int w = static_cast<int>( iexSystem::ScreenWidth *  0.39f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.7f );
			checkBack->Render( x, y, w, h, 0, 0, 512, 512 );

			//	確認項目描画
			x = static_cast<int>( iexSystem::ScreenWidth * 0.34f );
			y = static_cast<int>( iexSystem::ScreenHeight * 0.62f );
			w = static_cast<int>( iexSystem::ScreenWidth *  0.31f );
			h = static_cast<int>( iexSystem::ScreenHeight * 0.13f );
			checkCursor->Render( x, y, w, h, 0, checkSelectInfo.select * 128, 512, 128 );

			//	確認カーソル描画
			if ( !checkSelectInfo.select )		x = static_cast<int>( iexSystem::ScreenWidth * 0.369f );
			else											x = static_cast<int>( iexSystem::ScreenWidth * 0.51f );
			y = static_cast<int>( iexSystem::ScreenHeight * 0.64f );
			w = static_cast<int>( iexSystem::ScreenWidth *  0.11f );
			h = static_cast<int>( iexSystem::ScreenHeight * 0.1f );
			selectCheckCursor->Render( x, y, w, h, 0, 0, 256, 256 );
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
		//	回転が終了していなかったらスキップ
		if ( bgInfo.t < 1.0f )return;

		//	変数準備
		bool	isEnd = false;

		//	パラメータ加算
		bgInfo.mt += 0.01f;
		if ( bgInfo.mt >= 1.0f )	bgInfo.mt = 1.0f;

		isEnd = CubicFunctionInterpolation( bgInfo.cpos, bgInfo.cspos, bgInfo.cepos, bgInfo.mt );
		if ( isEnd )
		{
			if ( screen->GetScreenState() )
			{
				changeScene = true;
				//MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
				//return;
			}
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
			changeScene = true;
			//MainFrame->ChangeScene( new sceneLoad( new sceneTitle() ) );
			//return;
		}
	}

	//	タイトル移動描画
	void	sceneMenu::MoveTitleRender( void )
	{

	}

//-------------------------------------------------------------------------------
//	オプション関数
//-------------------------------------------------------------------------------
	
	//	オプション初期化
	void	sceneMenu::OptionInitialize( void )
	{
		//　構造体初期化
		optionInfo.itemflg = true;
		optionInfo.life = LIFE_MAX_NUM::LIFE_5;
		optionInfo.minute = 1;
		optionInfo.second = 0;
		optionInfo.step = 0;
		gameManager->SetItemFlg(optionInfo.itemflg);
		gameManager->SetMaxLife(optionInfo.life);
		gameManager->SetTime(optionInfo.minute, optionInfo.second);
	}

	//	オプション更新
	void	sceneMenu::OptionUpdate( void )
	{
		if (input[0]->Get(KEY_DOWN) == 3)
		{
			sound->PlaySE(SE::DECIDE_SE);
			if (optionInfo.step<3)
			optionInfo.step++;
		}
		if (input[0]->Get(KEY_UP) == 3)
		{
			sound->PlaySE(SE::DECIDE_SE);
			if (optionInfo.step>0){
				optionInfo.step--;
			}
		}

		switch (optionInfo.step)
		{
		case 0:
			if (input[0]->Get(KEY_RIGHT) == 3 || input[0]->Get(KEY_LEFT) == 3){
				sound->PlaySE(SE::CHOICE_SE);
				if (optionInfo.itemflg == false){
					optionInfo.itemflg = true;
				}
				else{
					optionInfo.itemflg = false;
				}
			}
			break;
		case 1:
			if ( KEY( KEY_RIGHT ) == 3 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				optionInfo.life++;
				if ( optionInfo.life >= LIFE_MAX_NUM::END )		optionInfo.life = LIFE_MAX_NUM::LIFE_3;
			}

			if ( KEY( KEY_LEFT ) == 3 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				optionInfo.life--;
				if ( optionInfo.life < LIFE_MAX_NUM::LIFE_3 )	optionInfo.life = LIFE_MAX_NUM::LIFE_5;
			}
			break;
		case 2:
			if (KEY_Get(KEY_RIGHT) == 3){
				if (optionInfo.minute<5){
					sound->PlaySE(SE::CHOICE_SE);
					optionInfo.minute++;
				}
			}
			else if (KEY_Get(KEY_LEFT) == 3){
				if (optionInfo.minute>1){
					sound->PlaySE(SE::CHOICE_SE);
					optionInfo.minute--;
				}
			}
			break;
		case 3:
			if (KEY_Get(KEY_RIGHT) == 3){
				sound->PlaySE(SE::CHOICE_SE);
				optionInfo.second = 30;
			}
			else if (KEY_Get(KEY_LEFT) == 3){
				sound->PlaySE(SE::CHOICE_SE);
				optionInfo.second = 0;
			}
			break;
		}
			gameManager->SetItemFlg(optionInfo.itemflg);
			gameManager->SetMaxLife(optionInfo.life);
			gameManager->SetTime(optionInfo.minute,optionInfo.second);
	}

	//	オプション描画
	void	sceneMenu::OptionRender( void )
	{
		optionImage->Render(300, 150, 512, 128, 0, 128*2, 512, 128);
		optionImage->Render(300, 350, 512, 128, 0, 128*0, 512, 128);
		optionImage->Render(400, 550, 256, 128, 0, 128*1, 256, 128);

		//アイテムの有無描画
		if (optionInfo.itemflg){
			optionImage->Render(950, 150, 256, 128, 0, 128 * 3, 256, 128);
		}
		else{
			optionImage->Render(950, 150, 256, 128, 256, 128 * 3, 256, 128);

		}
		//lifeMAXの描画
		optionLife->Render(940,	350, 128, 128,(optionInfo.life+3)*64, 128 * 0, 64, 64);

		TimerRender();
		ArrowRender();

		optionMenu->Render(80, 50, 256, 128, 0, 64, 256, 128);
	}

	//	タイマー描画
	void	sceneMenu::TimerRender( void )
	{
		optionLife->Render(930 , 550, 128, 128, optionInfo.minute*64, 64 * 0, 64, 64);
		optionLife->Render(1140, 550, 128, 128, ((optionInfo.second / 10) % 10) * 64, 64 * 0, 64, 64);
		optionLife->Render(1220, 550, 128, 128, 0, 64*0, 64, 64);
		optionImage->Render(1020, 550, 128, 128, 256, 128 * 1, 128, 128);
		optionImage->Render(1300, 550, 128, 128, 384, 128 * 1, 128, 128);

	}

	//	項目描画
	void	sceneMenu::OptionSelectRender( void )
	{
		
		//項目描画
		optionImage->Render(800, 320, 256, 64, 0, 128 * 2, 512, 128);
		optionImage->Render(800, 420, 256, 64, 0, 128 * 0, 512, 128);
		optionImage->Render(800, 520, 128, 64, 0, 128 * 1, 256, 128);
		//アイテムの有無描画
		if (optionInfo.itemflg){
			optionImage->Render(1100, 320, 128, 64, 0, 128 * 3, 256, 128);
		}
		else{
			optionImage->Render(1100, 320, 128, 64, 256, 128 * 3, 256, 128);

		}
		//コインMAXの描画
		optionLife->Render(1100, 420, 64, 64, ( optionInfo.life + 3 ) * 64, 128 * 0, 64, 64);
		//タイム
		optionLife->Render(1100, 520, 64, 64, optionInfo.minute * 64, 64 * 0, 64, 64);
		optionLife->Render(1220, 520, 64, 64, ((optionInfo.second / 10) % 10) * 64, 64 * 0, 64, 64);
		optionLife->Render(1260, 520, 64, 64, 0, 64 * 0, 64, 64);
		optionImage->Render(1160, 520, 64, 64, 256, 128 * 1, 128, 128);
		optionImage->Render(1320, 520, 64, 64, 384, 128 * 1, 128, 128);
	}

	//	矢印描画
	void	sceneMenu::ArrowRender( void )
	{
		switch (optionInfo.step){
		case 0:
			optionLife->Render(830, 150, 128, 128, 64*4, 64 * 3, 64, 64);
			break;
		case 1:
			optionLife->Render(830, 350, 128, 128, 64 * 4, 64 * 3, 64, 64);
			break;
		case 2:
			optionLife->Render(830, 550, 128, 128, 64 * 4, 64 * 3, 64, 64);
			optionLife->Render(930, 550, 128, 128, optionInfo.minute * 64, 64 * 1, 64, 64);
			break;
		case 3:
			optionLife->Render(830, 550, 128, 128, 64 * 4, 64 * 3, 64, 64);
			optionLife->Render(1140, 550, 128, 128, ((optionInfo.second / 10) % 10) * 64, 64 * 1, 64, 64);
			optionLife->Render(1220, 550, 128, 128, 0, 64 * 1, 64, 64);
			break;
		}
	}

//-------------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------------

	//	大砲位置設定
	void	sceneMenu::SetCannonPos( void )
	{
		Matrix	mat;
		Matrix	cannonMat;
		Vector3	up = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	cannonPos = Vector3( 0.0f, 0.0f, 0.0f );
		FOR( 0, PLAYER_MAX )
		{
			if ( characterSelectInfo.character[value] != CHARACTER_TYPE::PIRATE )	continue;
			mat = *obj[value]->GetBone( 7 ) * obj[value]->TransMatrix;
			cannon[value]->TransMatrix = mat;
			up = Vector3( mat._21, mat._22, mat._23 );
			up.Normalize();
			cannonPos = Vector3( cannon[value]->TransMatrix._41, cannon[value]->TransMatrix._42, cannon[value]->TransMatrix._43 );
			cannonPos += up * 0.5f;
			cannon[value]->TransMatrix._41 = cannonPos.x;
			cannon[value]->TransMatrix._42 = cannonPos.y;
			cannon[value]->TransMatrix._43 = cannonPos.z;
		}
	}

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
			//OptionInitialize();
			break;
		}

		//	モード切替
		this->mode = mode;
	}
