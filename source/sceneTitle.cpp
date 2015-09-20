
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"Sound.h"
#include	"Image.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"sceneMain.h"
#include	"GameManager.h"
#include	"sceneLoad.h"

#include	"sceneTitle.h"

#define		PLAYER_NUM	4

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
}

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	sceneTitle::sceneTitle( void )	: mode( TITLE )
	{

	}
	
	//	デストラクタ
	sceneTitle::~sceneTitle( void )
	{
		SafeDelete( m_Stage );
		SafeDelete( m_Camera );
		SafeDelete( m_Player );
		SafeDelete( m_CollisionStage );
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

		//	マネージャー初期化
		GameManager::Initialize();

		//	音登録
		sound->Initialize();

		//	カメラ設定
		m_Camera = new Camera();

		//	ステージ
		m_CollisionStage = new iexMesh( "DATA/BG/CollisionGround.IMO" );
		m_Stage = new iexMesh( "DATA/BG/2_1/FIELD2_1.IMO" );

		//	当たり判定
		Collision::Initiallize( m_CollisionStage );

		//	プレイヤー初期化
		m_Player = new PlayerManager();
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			m_Player->Initialize( i, PlayerData::PRINCESS, c_Move::TARGET[i] );
		}

		//	構造体初期化
		{
			//	キャラクター情報初期化
			for ( int i = 0; i < PlayerData::CHARACTER_MAX; i++ )
			{
				characterInfo[i].name = PlayerData::characterName[i];
				characterInfo[i].select = false;
			}
			
			//	選択情報初期化
			selectInfo.playerNum = 1;
			selectInfo.stageType = 0;
			for ( int i = 0; i < PLAYER_NUM; i++ )	selectInfo.characterType[i] = i;

			//	カメラ情報構造体初期化
			cameraInfo.lerpStartPos = cameraInfo.pos = m_Camera->GetPos();
			cameraInfo.target = Vector3( 0.0f, 0.0f, 0.0f );
			cameraInfo.t = 0.0f;
			cameraInfo.posNum = 0;
		}

		return	true;
	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	更新
	void	sceneTitle::Update( void )
	{
		//	プレイヤー更新
		m_Player->Update();

		switch ( mode )
		{
		case TITLE:
			TitleUpdate();
			break;
		
		case MENU:
			MenuUpdate();
			break;

		case SELECT_PLAYERNUM:
			SelectPlayerNumUpdate();
			break;

		case SELECT_CHARACTER:
			SelectCharacterUpdate();
			break;

		case SELECT_STAGE:
			SelectStageUpdate();
			break;

		case SELECT_CHECK:
			SelectCheckUpdate();
			break;

		case OPTION:
			OptionUpdate();
			break;

		case CREDIT:
			CreditUpdate();
			break;

		case MOVE_MAIN:
			if ( !sound->GetSEState( SE::DECIDE_SE ) )
			{
				MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
				return;
			}
			break;
		}

		
	}

	//	描画
	void	sceneTitle::Render( void )
	{
		//	画面クリア
		m_Camera->Activate();
		m_Camera->Clear();

		//	オブジェクト描画
		m_Stage->Render( shader3D, "full" );
		m_Player->Render( shader3D, "full" );

		switch ( mode )
		{
		case TITLE:
			TitleRender();
			break;

		case MENU:
			MenuRender();
			break;

		case SELECT_PLAYERNUM:
			SelectPlayerNumRender();
			break;

		case SELECT_CHARACTER:
			SelectCharacterRender();
			break;

		case SELECT_STAGE:
			SelectStageRender();
			break;

		case SELECT_CHECK:
			SelectCheckRender();
			break;

		case OPTION:
			OptionRender();
			break;

		case CREDIT:
			CreditRender();
			break;

		case MOVE_MAIN:
			break;
		}

		////	デバッグ用
		//DrawString( "[sceneTitle]", 50, 50 );
		//DrawString( "すぺーす押してね", 300, 400, 0xFFFFFF00 );
		//DrawString( "はーいぷしゅっ！", 1100, 700, 0xFFFFFF00 );
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
			//　SPACEでスタート
			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = MENU;
				sound->PlaySE( SE::DECIDE_SE );
			}
		}

		//	描画
		void	sceneTitle::TitleRender( void )
		{
			DrawString( "タイトルだよ", 50, 50 );
			DrawString( "[SPACE]：メニューへ", 300, 400, 0xFFFFFF00 );
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
				case 0:		mode = SELECT_PLAYERNUM;	break;
				case 1:		mode = OPTION;						break;
				case 2:		mode = CREDIT;						break;
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

			if ( selectInfo.playerNum > PLAYER_NUM )	selectInfo.playerNum = 1;
			if ( selectInfo.playerNum < 1 )						selectInfo.playerNum = PLAYER_NUM; 

			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = SELECT_CHARACTER;
				sound->PlaySE( SE::DECIDE_SE );
			}
			if ( KEY( KEY_DOWN ) == 3 )		mode = MENU;
		}
		
		//	描画
		void	sceneTitle::SelectPlayerNumRender( void )
		{
			DrawString( "人数選択だよ", 50, 50 );
			DrawString( "[SPACE]：キャラ選択へ", 300, 400, 0xFFFFFF00 );

			char	str[64];
			wsprintf( str, "プレイヤー人数：%d\n", selectInfo.playerNum );
			IEX_DrawText( str, 300, 300, 200, 20, 0xFFFF00FF );
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
			static int step = 0;
			int selectCheck = 0;

			switch ( step )
			{
			case 0:		//　プレイヤーキャラ選択
				for ( int p = 0; p < selectInfo.playerNum; p++ )
				{
					if ( input[p]->Get( KEY_DOWN ) == 3 )
					{
						//	全員分の入力チェック
						selectCheck = 0;
						for ( int i = 0; i < PLAYER_NUM; i++ )
						{
							if ( select[i] )		selectCheck++;
						}

						//	全員未選択なら戻る
						if ( selectCheck == 0 )
						{
							mode = SELECT_PLAYERNUM;
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
						for ( int i = 0; i < PLAYER_NUM; i++ )
						{
							if (select[i])		selectCheck++;
						}

						//	全員選択済みだったら次のステップへ
						if ( selectCheck >= selectInfo.playerNum )
						{
							step++;
							sound->PlaySE( SE::DECIDE_SE );
						}
					}

					//	数値制限
					if ( selectInfo.characterType[p] >= PlayerData::CHARACTER_MAX )	selectInfo.characterType[p] = 0;
					if ( selectInfo.characterType[p] < 0 )	selectInfo.characterType[p] = PlayerData::CHARACTER_MAX - 1;
				}
				break;

			case 1:		//　CPUキャラ選択
				if ( selectInfo.playerNum == PLAYER_NUM )	step++;
				else
				{
					for ( int p = selectInfo.playerNum; p < PLAYER_NUM; p++ )
					{
						if ( KEY( KEY_RIGHT ) == 3 )	selectInfo.characterType[p]++;
						if ( KEY( KEY_LEFT ) == 3 )	selectInfo.characterType[p]--;

						//	数値制限
						if ( selectInfo.characterType[p] >= PlayerData::CHARACTER_MAX )	selectInfo.characterType[p] = 0;
						if ( selectInfo.characterType[p] < 0 )	selectInfo.characterType[p] = PlayerData::CHARACTER_MAX - 1;
					}
					if ( KEY( KEY_SPACE ) == 3 )
					{
						step++;
						sound->PlaySE( SE::DECIDE_SE );
					}
					if ( KEY( KEY_DOWN ) == 3 ) step--;
				}
				break;

			case 2:
				mode = SELECT_STAGE;
				step = 0;
				break;
			}
		}
		
		//	描画
		void	sceneTitle::SelectCharacterRender( void )
		{
			DrawString( "キャラ選択だよ", 50, 50 );
			DrawString( "[SPACE]：ステージ選択へ", 300, 400, 0xFFFFFF00 );

			char	str[64];
			for ( int p = 0; p < PLAYER_NUM; p++ )
			{
				LPSTR string = characterInfo[selectInfo.characterType[p]].name;
				sprintf_s( str, "\n%dＰのキャラタイプ：", p + 1 );
				strcat( str, string );
				DrawString( str, 300, 300 + p * 20 );
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
				mode = SELECT_CHECK;
				sound->PlaySE( SE::DECIDE_SE );
			}
			if ( KEY( KEY_DOWN ) == 3 )		mode = SELECT_CHARACTER;
		}

		//	描画
		void	sceneTitle::SelectStageRender( void )
		{
			DrawString( "ステージ選択だよ", 50, 50 );
			DrawString( "[SPACE]：最終確認へ", 300, 400, 0xFFFFFF00 );

			char	str[64];
			wsprintf( str, "ステージ：%d番\n", selectInfo.stageType );
			IEX_DrawText( str, 300, 300, 200, 20, 0xFFFF00FF );
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
			if ( KEY( KEY_DOWN ) == 3 )		mode = SELECT_STAGE;
			if ( KEY( KEY_SPACE ) == 3 )
			{
				if ( selectInfo.ready )
				{
					//	情報をマネージャーに登録
					for ( int p = 0; p < PLAYER_NUM; p++ )		GameManager::SetCharacterType( p, selectInfo.characterType[p] );
					GameManager::SetPlayerNum( selectInfo.playerNum );
					GameManager::SetStageType( selectInfo.stageType );
					sound->PlaySE( SE::DECIDE_SE );
					mode = MOVE_MAIN;
				}
				else	mode = SELECT_STAGE;
			}
		}
		
		//	描画
		void	sceneTitle::SelectCheckRender( void )
		{
			//　選択情報
			char	str[64];
			for ( int p = 0; p < PLAYER_NUM; p++ )
			{
				LPSTR string = characterInfo[selectInfo.characterType[p]].name;
				wsprintf( str, "\n%dＰのキャラタイプ：", p + 1 );
				strcat( str, string );
				DrawString( str, 300, 250 + p * 20, 0xFFFF00FF );
			}

			wsprintf( str, "プレイヤー人数：%d\n", selectInfo.playerNum );
			DrawString( str, 300, 350, 0xFFFF00FF );

			wsprintf( str, "ステージ：%d番\n", selectInfo.stageType );
			DrawString( str, 300, 370, 0xFFFF00FF );

			DrawString( "最終確認だよ", 50, 50 );
			DrawString( "[SPACE]：sceneMainへ", 300, 470, 0xFFFFFF00 );

			static bool ready = true;
			if ( KEY( KEY_RIGHT ) == 3 ) ready = !ready;
			if ( KEY( KEY_LEFT ) == 3 ) ready = !ready;
			wsprintf( str, "はい＝1、いいえ＝0：%d番\n", ready );
			IEX_DrawText( str, 300, 390, 200, 20, 0xFFFF00FF );
		}

	//--------------------------------------------------------
	//	オプション
	//--------------------------------------------------------
	
		//	更新
		void	sceneTitle::OptionUpdate( void )
		{
			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = MENU;
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
				mode = MENU;
				sound->PlaySE( SE::DECIDE_SE );
			}
		}

		//	描画
		void	sceneTitle::CreditRender( void )
		{
			DrawString( "つくった戦士たちの紹介だよ", 50, 50 );
			DrawString( "[SPACE]：メニューへ", 300, 400, 0xFFFFFF00 );
		}