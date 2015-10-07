
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
	//	カメラ移動用位置情報
	namespace c_Move{
		Vector3 TARGET[4]=
		{
			Vector3( 0.0f, 0.0f, 0.0f ),
			Vector3( 20.0f, 0.0f, 10.0f ),
			Vector3( 5.0f, 0.0f, -10.0f ),
			Vector3( -20.0f, 0.0f, 10.0f ),
		};

	}

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
		stage = new iexMesh( "DATA/BG/2_1/FIELD2_1.IMO" );

		//	乱数初期化
		Random::Initialize();		

		//	各モード初期化
		TitleInitialize();

		//	構造体初期化
		{
			//	キャラクター情報初期化
			for ( int i = 0; i < CHARACTER_TYPE::MAX; i++ )
			{
				characterInfo[i].name = characterName[i];
				characterInfo[i].select = false;
			}
			
			//	選択情報初期化
			selectInfo.playerNum = 1;
			selectInfo.stageType = 0;
			selectInfo.step_cs = 0;
			for ( int i = 0; i < PLAYER_MAX; i++ )	selectInfo.characterType[i] = i;

			//	カメラ情報構造体初期化
			cameraInfo.lerpStartPos = cameraInfo.pos = m_Camera->GetPos();
			cameraInfo.target = Vector3( 0.0f, 0.0f, 0.0f );
			cameraInfo.t = 0.0f;
			cameraInfo.posNum = 0;
		}

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
			SelectPlayerNumUpdate();
			break;

		case TITLE_MODE::SELECT_CHARACTER:
			SelectCharacterUpdate();
			break;

		case TITLE_MODE::SELECT_STAGE:
			SelectStageUpdate();
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
		for ( int i = 0; i < PLAYER_MAX; i++)	particle->BlueFlame( c_Move::TARGET[i] );
		particle->Update();

		//	スクリーン更新
		screen->Update();	
	}

	//	描画
	void	sceneTitle::Render( void )
	{
		//	画面クリア
		m_Camera->Activate();
		m_Camera->Clear();

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

		case TITLE_MODE::SELECT_PLAYERNUM:
			SelectPlayerNumRender();
			break;

		case TITLE_MODE::SELECT_CHARACTER:
			SelectCharacterRender();
			break;

		case TITLE_MODE::SELECT_STAGE:
			SelectStageRender();
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
		char	str[256];
		Matrix	mat = m_Camera->GetMatrix();
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();
		sprintf_s( str, "t.x = %f\nt.y = %f\nt.z = %f\n", front.x, front.y, front.z );
		DrawString( str, 50, 500 );
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
				m_Camera->Update( VIEW_MODE::SLERP, c_Move::TARGET[cameraInfo.posNum] );

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
			//	移動先を変える
			if ( KEY( KEY_C ) == 3 && cameraInfo.t > 0.7f )
			{
				if ( cameraInfo.posNum < 3 )	cameraInfo.posNum++;
				else cameraInfo.posNum = 0;
				cameraInfo.t = 0.0f;
			}
			if ( KEY( KEY_D ) == 3 && cameraInfo.t > 0.7f )
			{
				if ( cameraInfo.posNum > 0 )	cameraInfo.posNum--;
				else cameraInfo.posNum = 3;
				cameraInfo.t = 0;
			}

			//	カメラ移動
			if ( cameraInfo.t <= 0.7f ){
				CubicFunctionInterpolation( cameraInfo.pos, cameraInfo.lerpStartPos, c_Move::TARGET[cameraInfo.posNum], cameraInfo.t - 0.1f );
			}
			else{
				cameraInfo.lerpStartPos = m_Camera->GetPos();
			}

			//	決定
			if ( KEY( KEY_SPACE ) == 3 )
			{
				switch ( cameraInfo.posNum )
				{
				case 0:		mode = TITLE_MODE::SELECT_PLAYERNUM;	break;
				case 1:		mode = TITLE_MODE::OPTION;						break;
				case 2:		mode = TITLE_MODE::CREDIT;						break;
				}	
				sound->PlaySE( SE::DECIDE_SE );
			}

			//	カメラ更新
			m_Camera->Update( VIEW_MODE::SLERP, c_Move::TARGET[cameraInfo.posNum] );
			m_Camera->SetPos( cameraInfo.pos );

			//	パラメータ加算
			cameraInfo.t += 0.01f;
			if ( cameraInfo.t >= 1.0f )	cameraInfo.t = 1.0f;
		}

		//	描画
		void	sceneTitle::MenuRender( void )
		{
			DrawString( "メニュー画面だよ", 50, 50 );
			DrawString( "C・Vでどれか選んでね", 300, 400, 0xFFFFFF00 );

			switch ( cameraInfo.posNum )
			{
			case 0:
				DrawString( "みんなであそぶモード！", 50, 100 );
				DrawString( "[SPACE]：キャラ選択へ", 300, 200, 0xFFFFFF00 );
				break;

			case 1:
				DrawString( "オプション", 50, 100 );
				DrawString( "[SPACE]：オプションへ", 300, 200, 0xFFFFFF00 );
				break;

			case 2:
				DrawString( "つくった戦士たち！", 50, 100 );
				DrawString( "[SPACE]：クレジットへ", 300, 200, 0xFFFFFF00 );
				break;
			}

		}

	//--------------------------------------------------------
	//	人数選択
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::SelectPlayerNumUpdate( void )
		{
			//　「A」「D」で選択
			//　「SPACE」で決定
			//　「S」で戻る

			if ( KEY( KEY_RIGHT ) == 3 )		selectInfo.playerNum++;
			if ( KEY( KEY_LEFT ) == 3 )		selectInfo.playerNum--;

			if ( selectInfo.playerNum > PLAYER_MAX )	selectInfo.playerNum = 1;
			if ( selectInfo.playerNum < 1 )						selectInfo.playerNum = PLAYER_MAX; 

			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::SELECT_CHARACTER;
				sound->PlaySE( SE::DECIDE_SE );
			}
			if ( KEY( KEY_DOWN ) == 3 )		mode = TITLE_MODE::MENU;
		}
		
		//    描画
		void    sceneTitle::SelectPlayerNumRender(void)
		{
			DrawString("人数選択だよ", 50, 50);
			DrawString("[SPACE]：キャラ選択へ", 300, 400);

			char    str[64];
			wsprintf(str, "プレイヤー人数：%d\n", selectInfo.playerNum);
			IEX_DrawText(str, 300, 300, 200, 20, 0xFFFFFFFF);
		}

	//--------------------------------------------------------
	//	キャラ選択
	//--------------------------------------------------------
		
		//	更新
		void	sceneTitle::SelectCharacterUpdate( void )
		{
			//　プレイヤーキャラ選択　→　CPUキャラ選択
			//　「A」「D」で選択
			//　「SPACE」で決定
			//　「S」で戻る

			static bool	select[4] = { false, false, false, false };
			int selectCheck = 0;

			switch ( selectInfo.step_cs )
			{
			case 0:		//　プレイヤーキャラ選択
				for ( int p = 0; p < selectInfo.playerNum; p++ )
				{
					if ( input[p]->Get( KEY_DOWN ) == 3 )
					{
						//	全員分の入力チェック
						selectCheck = 0;
						for ( int i = 0; i < PLAYER_MAX; i++ )
						{
							if ( select[i] )		selectCheck++;
						}

						//	全員未選択なら戻る
						if ( selectCheck == 0 )
						{
							mode = TITLE_MODE::SELECT_PLAYERNUM;
							return;
						}

						//	選択時キャンセル
						characterInfo[selectInfo.characterType[p]].select = false;
						select[p] = false;
					};

					if ( select[p] )	continue;
					if ( input[p]->Get( KEY_RIGHT ) == 3 )	selectInfo.characterType[p]++;
					if ( input[p]->Get( KEY_LEFT ) == 3 )	selectInfo.characterType[p]--;
					if ( input[p]->Get( KEY_SPACE ) == 3 )
					{
						//	未選択なら選択
						if ( !characterInfo[selectInfo.characterType[p]].select )
						{
							characterInfo[selectInfo.characterType[p]].select = true;
							select[p] = true;
							sound->PlaySE( SE::DECIDE_SE );
						}

						//	全員分の入力チェック
						selectCheck = 0;
						for ( int i = 0; i < PLAYER_MAX; i++ )
						{
							if (select[i])		selectCheck++;
						}

						//	全員選択済みだったら次のステップへ
						if ( selectCheck >= selectInfo.playerNum )
						{
							selectInfo.step_cs++;
							sound->PlaySE( SE::DECIDE_SE );
						}
					}

					//	数値制限
					if ( selectInfo.characterType[p] >= CHARACTER_TYPE::MAX )	selectInfo.characterType[p] = 0;
					if ( selectInfo.characterType[p] < 0 )	selectInfo.characterType[p] = CHARACTER_TYPE::MAX - 1;
				}
				break;

			case 1:		//　CPUキャラ選択
				if ( selectInfo.playerNum == PLAYER_MAX )	selectInfo.step_cs++;
				else
				{
					for ( int p = selectInfo.playerNum; p < PLAYER_MAX; p++ )
					{
						if ( KEY( KEY_RIGHT ) == 3 )	selectInfo.characterType[p]++;
						if ( KEY( KEY_LEFT ) == 3 )	selectInfo.characterType[p]--;

						//	数値制限
						if ( selectInfo.characterType[p] >= CHARACTER_TYPE::MAX )	selectInfo.characterType[p] = 0;
						if ( selectInfo.characterType[p] < 0 )	selectInfo.characterType[p] = CHARACTER_TYPE::MAX - 1;
					}
					if ( KEY( KEY_SPACE ) == 3 )
					{
						selectInfo.step_cs++;
						sound->PlaySE( SE::DECIDE_SE );
					}
					if ( KEY( KEY_DOWN ) == 3 ) selectInfo.step_cs--;
				}
				break;

			case 2:
				mode = TITLE_MODE::SELECT_STAGE;
				selectInfo.step_cs = 0;
				break;
			}
		}
		
		//    描画
		void    sceneTitle::SelectCharacterRender(void)
		{
			DrawString("キャラ選択だよ", 300, 100);
			//DrawString( "[SPACE]：ステージ選択へ", 300, 400, 0xFFFFFF00 );

			char    str[64];
			for (int p = 0; p < selectInfo.playerNum; p++)
			{
				LPSTR string = characterInfo[selectInfo.characterType[p]].name;
				sprintf_s(str, "\n%dＰのキャラタイプ：", p + 1);
				strcat(str, string);
				DrawString(str, 300, 300 + p * 20, 0xFFFFFF00);
			}
			for (int p = selectInfo.playerNum; p < PLAYER_MAX; p++)
			{
				LPSTR string = characterInfo[selectInfo.characterType[p]].name;
				sprintf_s(str, "\n%dＰのキャラタイプ：", p + 1);
				strcat(str, string);
				DrawString(str, 300, 300 + p * 20);
			}

			switch (selectInfo.step_cs)
			{
			case 0:
				DrawString("プレイヤーが使うキャラを選んでね", 300, 280, 0xFFFFFF00);
				break;

			case 1:
				DrawString("ＣＰＵが使うキャラを選んでね", 300, 280);
				break;
			}

		}

	//--------------------------------------------------------
	//	ステージ選択
	//--------------------------------------------------------
		
		//	更新
		void	sceneTitle::SelectStageUpdate( void )
		{
			//　「A」「D」で選択
			//　「SPACE」で決定
			//　「S」で戻る

			//	選択
			if ( KEY( KEY_RIGHT ) == 3 ) 	selectInfo.stageType++;
			if ( KEY( KEY_LEFT ) == 3 )		selectInfo.stageType--;

			//	上下限設定
			if ( selectInfo.stageType >= STAGE_MAX )	selectInfo.stageType = 0;
			if ( selectInfo.stageType < 0 )					selectInfo.stageType = STAGE_MAX - 1;

			//	決定・キャンセル
			if ( KEY( KEY_SPACE ) == 3 )	
			{
				mode = TITLE_MODE::SELECT_CHECK;
				sound->PlaySE( SE::DECIDE_SE );
			}
			if ( KEY( KEY_DOWN ) == 3 )		mode = TITLE_MODE::SELECT_CHARACTER;
		}

		//    描画
		void    sceneTitle::SelectStageRender(void)
		{
			DrawString("ステージ選択だよ", 50, 50);
			DrawString("[SPACE]：最終確認へ", 300, 400);

			char    str[64];
			wsprintf(str, "ステージ：%d番\n", selectInfo.stageType);
			IEX_DrawText(str, 300, 300, 200, 20, 0xFFFFFFFF);
		}

	//--------------------------------------------------------
	//	最終確認
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::SelectCheckUpdate( void )
		{
			//		「A」「D」で、はい・いいえ的なやつを選ぶ
			//　	「SPACE」で決定
			//　	「S」で戻る
			if ( KEY( KEY_RIGHT ) == 3 )		selectInfo.ready = !selectInfo.ready;
			if ( KEY( KEY_LEFT ) == 3 )		selectInfo.ready = !selectInfo.ready;
			if ( KEY( KEY_DOWN ) == 3 )		mode = TITLE_MODE::SELECT_STAGE;
			if ( KEY( KEY_SPACE ) == 3 )
			{
				if ( selectInfo.ready )
				{
					//	情報をマネージャーに登録
					for ( int p = 0; p < PLAYER_MAX; p++ )		gameManager->SetCharacterType( p, selectInfo.characterType[p] );
					gameManager->SetPlayerNum( selectInfo.playerNum );
					gameManager->SetStageType( selectInfo.stageType );
					sound->PlaySE( SE::DECIDE_SE );
					mode = TITLE_MODE::MOVE_MAIN;
				}
				else	mode = TITLE_MODE::SELECT_STAGE;
			}
		}
		
		//    描画
		void    sceneTitle::SelectCheckRender(void)
		{
			//　選択情報
			char    str[64];
			for (int p = 0; p < PLAYER_MAX; p++)
			{
				LPSTR string = characterInfo[selectInfo.characterType[p]].name;
				wsprintf(str, "\n%dＰのキャラタイプ：", p + 1);
				strcat(str, string);
				DrawString(str, 300, 250 + p * 20);
			}

			wsprintf(str, "プレイヤー人数：%d\n", selectInfo.playerNum);
			DrawString(str, 300, 350);

			wsprintf(str, "ステージ：%d番\n", selectInfo.stageType);
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
			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				sound->PlaySE( SE::DECIDE_SE );
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
			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				sound->PlaySE( SE::DECIDE_SE );
			}
		}

		//	描画
		void	sceneTitle::CreditRender( void )
		{
			DrawString( "つくった戦士たちの紹介だよ", 50, 50 );
			DrawString( "[SPACE]：メニューへ", 300, 400, 0xFFFFFF00 );
		}