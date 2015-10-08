
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"Sound.h"
#include	"Screen.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"Random.h"
#include	"BaseChara.h"
#include	"CharacterManager.h"
#include	"sceneMain.h"
#include	"GameManager.h"
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

	//	タイトルモード
	namespace TITLE_MODE
	{
		enum
		{
			TITLE,
			MENU,
			SELECT_PLAYERNUM,
			SELECT_CHARACTER,
			SELECT_STAGE,
			SELECT_CHECK,
			OPTION,
			CREDIT,
			MOVE_MAIN,
		};
	}

	//	メニューモード
	namespace SELECT_MODE
	{
		enum
		{
			PLAYER_NUM,
			CHARACTER,
			STAGE,
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
		SafeDelete( m_Camera );
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
		m_Camera = new Camera();

		//	マネージャー初期化
		gameManager->Initialize();

		//	パーティクル初期化
		particle->Initialize();

		//	音登録
		sound->Initialize();

		//	スクリーン初期化
		screen->Initialize();
		
		//	ステージ
		stage = new iexMesh( "DATA/BG/title_map.IMO" );

		//	乱数初期化
		Random::Initialize();		

		//	各モード初期化
		TitleInitialize();
		MenuInitialize();
		SelectInitialize();

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

	//	選択初期化
	void	sceneTitle::SelectInitialize( void )
	{
		//	キャラクター情報初期化
		for ( int i = 0; i < CHARACTER_TYPE::MAX; i++ )
		{
			characterInfo[i].name = characterName[i];
			characterInfo[i].select = false;
		}

		//	選択情報初期化
		setInfo.playerNum = 1;
		setInfo.stageType = 0;
		setInfo.step_cs = 0;
		for ( int i = 0; i < PLAYER_MAX; i++ )	setInfo.characterType[i] = i;

		//	パラメータ初期化
		selectInfo.menu_num = 0;
		selectInfo.mode = SELECT_MODE::PLAYER_NUM;
		selectInfo.select_mode = 0;

		//	プレイヤー選択情報初期化
		for ( int i = 0; i < PLAYER_MAX; i++ )  
		{
			selectInfo.playerselectInfo[i].isSelect = false;
			selectInfo.playerselectInfo[i].num = i;
		}
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

		case TITLE_MODE::SELECT_PLAYERNUM:
			SelectUpdate();
			break;

		case TITLE_MODE::SELECT_CHECK:
			SelectCheckUpdate();
			break;

		case TITLE_MODE::OPTION:
			OptionUpdate();
			break;

		case TITLE_MODE::CREDIT:
			CreditUpdate();
			break;

		case TITLE_MODE::MOVE_MAIN:
			if ( !sound->GetSEState( SE::DECIDE_SE ) )
			{
				MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
				return;
			}
			break;
		}

		//	ターゲット位置にパーティクル配置
		for ( int i = 0; i < TITLE_TARGET::END; i++)	particle->BlueFlame( TITLE_MOVE_INFO::target[i] );
		particle->Update();
		
		//	スクリーン更新
		screen->Update();	
		particle->BlueFlame( eff_pos, 1.0f );
	}

	//	描画
	void	sceneTitle::Render( void )
	{
		//	画面クリア
		m_Camera->Activate();
		m_Camera->Clear();

		//	ステージ描画
		stage->Render();

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

		case TITLE_MODE::SELECT_PLAYERNUM:
			SelectRender();
			break;

		case TITLE_MODE::SELECT_CHECK:
			SelectCheckRender();
			break;

		case TITLE_MODE::OPTION:
			OptionRender();
			break;

		case TITLE_MODE::CREDIT:
			CreditRender();
			break;

		case TITLE_MODE::MOVE_MAIN:
			break;
		}

		//	スクリーン描画
		screen->Render();

		//	デバッグ
		m_Camera->Render();

		//char	str[256];
		Matrix	mat = m_Camera->GetMatrix();
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
			static	float	speed = 0.5f;
			static	bool	curtainStateL = false;
			static	bool	curtainStateR = false;

			switch ( titleInfo.step )
			{
			case 0:
				//	カメラ更新
				m_Camera->Update( VIEW_MODE::SETUP );

				//	点滅更新
				FlashingUpdate( titleInfo.pressSpace, D3DX_PI / 180 * 4.0f );

				//	SPACEキーで選択
				if ( input[0]->Get( KEY_SPACE ) == 3 )
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
				m_Camera->Update( VIEW_MODE::TITLE );

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
				curtainStateL = Lerp( titleInfo.curtainL.tlv[0].sx, 0, -640, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[1].sx, 640, 0, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[2].sx, 0, -640, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[3].sx, 640, 0, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[0].sx, 640, 1280, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[1].sx, 1280, 1920, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[2].sx, 640, 1280, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
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
			if ( m_Camera->GetMoveState() )	//	移動が終了してたら
			{
				//	C,Vキーで選択
				if ( KEY( KEY_C ) == 3 )
				{
					menuInfo.menu_num--;
					if ( menuInfo.menu_num < 0 )		menuInfo.menu_num = TITLE_TARGET::CREDIT;
					m_Camera->SetNextPoint( menuInfo.menu_num, 0.01f );
				}
				if ( KEY( KEY_D ) == 3 )
				{
					menuInfo.menu_num++;
					if ( menuInfo.menu_num > TITLE_TARGET::CREDIT )	menuInfo.menu_num = 0;
					m_Camera->SetNextPoint( menuInfo.menu_num, 0.01f );
				}

				//	決定
				if ( KEY( KEY_SPACE ) == 3 )
				{
					switch ( menuInfo.menu_num )
					{
					case TITLE_TARGET::PLAYERNUMBER:
						mode = TITLE_MODE::SELECT_PLAYERNUM;
						break;

					case TITLE_TARGET::OPTION:
						mode = TITLE_MODE::OPTION;
						break;

					case TITLE_TARGET::CREDIT:
						mode = TITLE_MODE::CREDIT;
						break;
					}	
					sound->PlaySE( SE::DECIDE_SE );
					m_Camera->SetNextPoint( menuInfo.menu_num, 0.01f );
				}
			}

			//	カメラ更新
			m_Camera->Update( VIEW_MODE::TITLE );
		}

		//	描画
		void	sceneTitle::MenuRender( void )
		{
			DrawString( "メニュー画面だよ", 50, 50 );
			DrawString( "C・Vでどれか選んでね", 300, 400, 0xFFFFFF00 );

			switch ( menuInfo.menu_num )
			{
			case TITLE_TARGET::PLAYERNUMBER:
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

		}

	//--------------------------------------------------------
	//	選択
	//--------------------------------------------------------

		//	更新
		void	sceneTitle::SelectUpdate( void )
		{
			switch ( selectInfo.mode )
			{
			case SELECT_MODE::PLAYER_NUM:
				SelectPlayerNumUpdate();
				break;

			case SELECT_MODE::CHARACTER:
				SelectCharacterUpdate();
				break;

			case SELECT_MODE::STAGE:
				SelectStageUpdate();
				break;
			}

			//	カメラ更新
			m_Camera->Update( VIEW_MODE::TITLE );
		}

		//	描画
		void	sceneTitle::SelectRender( void )
		{
			switch ( selectInfo.mode )
			{
			case SELECT_MODE::PLAYER_NUM:
				SelectPlayerNumRender();
				break;

			case SELECT_MODE::CHARACTER:
				SelectCharacterRender();
				break;

			case SELECT_MODE::STAGE:
				SelectStageRender();
				break;
			}
		}

	//--------------------------------------------------------
	//	人数選択
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::SelectPlayerNumUpdate( void )
		{
			//	移動中は選択不可
			if( !m_Camera->GetMoveState() )	return;

			//	選択
			if ( KEY( KEY_RIGHT ) == 3 )		setInfo.playerNum++;
			if ( KEY( KEY_LEFT ) == 3 )		setInfo.playerNum--;

			//	制限
			if ( setInfo.playerNum > PLAYER_MAX )		setInfo.playerNum = 1;
			if ( setInfo.playerNum < 1 )						setInfo.playerNum = PLAYER_MAX; 

			//	選択
			if ( KEY( KEY_SPACE ) == 3 )
			{
				selectInfo.mode = SELECT_MODE::CHARACTER;
				m_Camera->SetNextPoint( TITLE_TARGET::SELECTCHARACTER, 0.01f );
				sound->PlaySE( SE::DECIDE_SE );
				return;
			}

			//	キャンセル
			if ( KEY( KEY_DOWN ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				m_Camera->SetNextPoint( TITLE_TARGET::PLAYERNUMBER, 0.01f );
			}
		}
		
		//    描画
		void    sceneTitle::SelectPlayerNumRender( void )
		{
			DrawString("人数選択だよ", 50, 50);
			DrawString("[SPACE]：キャラ選択へ", 300, 400);

			char    str[64];
			wsprintf(str, "プレイヤー人数：%d\n", setInfo.playerNum);
			IEX_DrawText(str, 300, 300, 200, 20, 0xFFFFFFFF);
		}

	//--------------------------------------------------------
	//	キャラ選択
	//--------------------------------------------------------
		
		//	更新
		void	sceneTitle::SelectCharacterUpdate( void )
		{
			if ( !m_Camera->GetMoveState() )	return;
			switch ( selectInfo.select_mode )
			{
			case 0:
				SelectPlayer();
				break;

			case 1:
				SelectCPU();
				break;

			case 2:
				selectInfo.mode = SELECT_MODE::STAGE;
				selectInfo.select_mode = 0;
				m_Camera->SetNextPoint( TITLE_TARGET::SELECTSTAGE, 0.01f );
				break;
			}
		}
		
		//    描画
		void	sceneTitle::SelectCharacterRender( void )
		{
			DrawString("キャラ選択だよ", 300, 100);
			//DrawString( "[SPACE]：ステージ選択へ", 300, 400, 0xFFFFFF00 );

			char    str[64];
			for (int p = 0; p < setInfo.playerNum; p++)
			{
				LPSTR string = characterInfo[selectInfo.playerselectInfo[p].num].name;
				sprintf_s(str, "\n%dＰのキャラタイプ：", p + 1);
				strcat(str, string);
				DrawString(str, 300, 300 + p * 20, 0xFFFFFF00);
			}
			for (int p = setInfo.playerNum; p < PLAYER_MAX; p++)
			{
				LPSTR string = characterInfo[selectInfo.playerselectInfo[p].num].name;
				sprintf_s(str, "\n%dＰのキャラタイプ：", p + 1);
				strcat(str, string);
				DrawString(str, 300, 300 + p * 20);
			}

			switch (selectInfo.select_mode)
			{
			case 0:
				DrawString("プレイヤーが使うキャラを選んでね", 300, 280, 0xFFFFFF00);
				break;

			case 1:
				DrawString("ＣＰＵが使うキャラを選んでね", 300, 280);
				break;
			}

		}

		//	プレイヤー選択
		void	sceneTitle::SelectPlayer( void )
		{
			for ( int p = 0; p < setInfo.playerNum; p++ )
			{
				if ( input[p]->Get( KEY_DOWN ) == 3 )
				{
					//	全員未選択なら戻る
					bool	isSelect[PLAYER_MAX] = 
					{
						selectInfo.playerselectInfo[0].isSelect,
						selectInfo.playerselectInfo[1].isSelect,
						selectInfo.playerselectInfo[2].isSelect,
						selectInfo.playerselectInfo[3].isSelect,
					};

					//	全員未選択なら前のメニューへ
					if ( !isSelect[0] && !isSelect[1] && !isSelect[2] && !isSelect[3] )
					{
						selectInfo.mode = TITLE_TARGET::PLAYERNUMBER;
						return;
					}

					//	選択時キャンセル
					characterInfo[selectInfo.playerselectInfo[p].num].select = false;
					selectInfo.playerselectInfo[p].isSelect= false;
				};

				//	選択済みはスルー
				if ( selectInfo.playerselectInfo[p].isSelect )	continue;

				//	左右で選択
				if ( input[p]->Get( KEY_RIGHT ) == 3 )	selectInfo.playerselectInfo[p].num++;
				if ( input[p]->Get( KEY_LEFT ) == 3 )	selectInfo.playerselectInfo[p].num--;
				
				//	数値制限
				if ( selectInfo.playerselectInfo[p].num >= CHARACTER_TYPE::MAX )	selectInfo.playerselectInfo[p].num = 0;
				if ( selectInfo.playerselectInfo[p].num < 0 )	selectInfo.playerselectInfo[p].num = CHARACTER_TYPE::MAX - 1;

				if ( input[p]->Get( KEY_SPACE ) == 3 )
				{
					//	未選択なら選択
					if ( !characterInfo[selectInfo.playerselectInfo[p].num].select )
					{
						characterInfo[selectInfo.playerselectInfo[p].num].select = true;
						selectInfo.playerselectInfo[p].isSelect = true;
						sound->PlaySE( SE::DECIDE_SE );
					}

					//	全員未選択なら戻る
					bool	isSelect[PLAYER_MAX] =
					{
						selectInfo.playerselectInfo[0].isSelect,
						selectInfo.playerselectInfo[1].isSelect,
						selectInfo.playerselectInfo[2].isSelect,
						selectInfo.playerselectInfo[3].isSelect,
					};

					//	全員の選択チェック
					bool	state = false;
					switch ( setInfo.playerNum )
					{
					case 1:
						if ( isSelect[0] )	state = true;
						break;

					case 2:
						if ( isSelect[0] && isSelect[1] )	state = true;
						break;

					case 3:
						if ( isSelect[0] && isSelect[1] && isSelect[2] )	state = true;
						break;

					case 4:
						if ( isSelect[0] && isSelect[1] && isSelect[2] && isSelect[3] )	state = true;
						break;
					}
					//	全員選択済みだったら次のステップへ
					if ( state )
					{
						selectInfo.select_mode++;
						sound->PlaySE( SE::DECIDE_SE );
						for ( int i = 0; i < PLAYER_MAX; i++ )
						{
							setInfo.characterType[i] = selectInfo.playerselectInfo[i].num;
						}
					}
				}

			}
		}

		//	CPU選択
		void	sceneTitle::SelectCPU( void )
		{
			//	プレイヤーが４人だったらつぎへ
			if ( setInfo.playerNum == PLAYER_MAX )	selectInfo.select_mode++;
			else
			{
				for ( int p = setInfo.playerNum; p < PLAYER_MAX; p++ )
				{
					//	選択
					if (KEY(KEY_RIGHT) == 3)	setInfo.characterType[p]++;
					if (KEY(KEY_LEFT) == 3)	setInfo.characterType[p]--;

					//	数値制限
					if ( setInfo.characterType[p] >= CHARACTER_TYPE::MAX )	setInfo.characterType[p] = 0;
					if ( setInfo.characterType[p] < 0 )	setInfo.characterType[p] = CHARACTER_TYPE::MAX - 1;
				}
				
				//	決定
				if ( KEY( KEY_SPACE ) == 3 )
				{
					selectInfo.select_mode++;
					sound->PlaySE( SE::DECIDE_SE );
				}
				
				//	キャンセル
				if ( KEY( KEY_DOWN ) == 3 ) selectInfo.select_mode--;
			}
		}

	//--------------------------------------------------------
	//	ステージ選択
	//--------------------------------------------------------
		
		//	更新
		void	sceneTitle::SelectStageUpdate( void )
		{
			//	移動中は選択不可
			if ( !m_Camera->GetMoveState() )	return;

			//	選択
			if ( KEY( KEY_RIGHT ) == 3 ) 	setInfo.stageType++;
			if ( KEY( KEY_LEFT ) == 3 )		setInfo.stageType--;

			//	上下限設定
			if ( setInfo.stageType >= STAGE_MAX )	setInfo.stageType = 0;
			if ( setInfo.stageType < 0 )						setInfo.stageType = STAGE_MAX - 1;

			//	決定
			if ( KEY( KEY_SPACE ) == 3 )	
			{
				mode = TITLE_MODE::SELECT_CHECK;
				sound->PlaySE( SE::DECIDE_SE );
				m_Camera->SetNextPoint( TITLE_TARGET::CHECK, 0.01f );
				return;
			}

			//	キャンセル
			if ( KEY( KEY_DOWN ) == 3 )
			{
				selectInfo.mode = TITLE_TARGET::SELECTCHARACTER;
				m_Camera->SetNextPoint( selectInfo.mode, 0.01f );
			}
		}

		//    描画
		void    sceneTitle::SelectStageRender( void )
		{
			DrawString("ステージ選択だよ", 50, 50);
			DrawString("[SPACE]：最終確認へ", 300, 400);

			char    str[64];
			wsprintf(str, "ステージ：%d番\n", setInfo.stageType);
			IEX_DrawText(str, 300, 300, 200, 20, 0xFFFFFFFF);
		}

	//--------------------------------------------------------
	//	最終確認
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::SelectCheckUpdate( void )
		{
			m_Camera->Update( VIEW_MODE::TITLE );
			if ( !m_Camera->GetMoveState() )	return;
			if ( KEY( KEY_RIGHT ) == 3 )		setInfo.ready = !setInfo.ready;
			if ( KEY( KEY_LEFT ) == 3 )		setInfo.ready = !setInfo.ready;
			if ( KEY( KEY_DOWN ) == 3 )		mode = TITLE_MODE::SELECT_PLAYERNUM;
			if ( KEY( KEY_SPACE ) == 3 )
			{
				if ( setInfo.ready )
				{
					//	情報をマネージャーに登録
					for ( int p = 0; p < PLAYER_MAX; p++ )		gameManager->SetCharacterType( p, setInfo.characterType[p] );
					gameManager->SetPlayerNum( setInfo.playerNum );
					gameManager->SetStageType( setInfo.stageType );
					sound->PlaySE( SE::DECIDE_SE );
					mode = TITLE_MODE::MOVE_MAIN;
				}
				else	mode = TITLE_MODE::SELECT_PLAYERNUM;
			}
		}
		
		//    描画
		void    sceneTitle::SelectCheckRender( void )
		{
			//　選択情報
			char    str[64];
			for (int p = 0; p < PLAYER_MAX; p++)
			{
				LPSTR string = characterInfo[setInfo.characterType[p]].name;
				wsprintf(str, "\n%dＰのキャラタイプ：", p + 1);
				strcat(str, string);
				DrawString(str, 300, 250 + p * 20);
			}

			wsprintf(str, "プレイヤー人数：%d\n", setInfo.playerNum);
			DrawString(str, 300, 350);

			wsprintf(str, "ステージ：%d番\n", setInfo.stageType);
			DrawString(str, 300, 370);

			DrawString("最終確認だよ", 50, 50);
			DrawString("[SPACE]：sceneMainへ", 300, 470);

			static bool ready = true;
			if (KEY(KEY_RIGHT) == 3) ready = !ready;
			if (KEY(KEY_LEFT) == 3) ready = !ready;
			wsprintf(str, "はい＝1、いいえ＝0：%d番\n", ready);
			IEX_DrawText(str, 300, 390, 200, 20, 0xFFFFFFFF);
		}

	//--------------------------------------------------------
	//	オプション
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::OptionUpdate( void )
		{
			m_Camera->Update( VIEW_MODE::TITLE );
			if ( !m_Camera->GetMoveState() )	return;
			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				sound->PlaySE( SE::DECIDE_SE );
				m_Camera->SetNextPoint( TITLE_TARGET::PLAYERNUMBER, 0.01f );
			}
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
			m_Camera->Update( VIEW_MODE::TITLE );
			if ( !m_Camera->GetMoveState() )	return;
			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				sound->PlaySE( SE::DECIDE_SE );
				m_Camera->SetNextPoint( TITLE_TARGET::PLAYERNUMBER, 0.01f );
			}
		}

		//	描画
		void	sceneTitle::CreditRender( void )
		{
			DrawString( "つくった戦士たちの紹介だよ", 50, 50 );
			DrawString( "[SPACE]：メニューへ", 300, 400, 0xFFFFFF00 );
		}