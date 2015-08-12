
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"sceneMain.h"

#include	"sceneTitle.h"
#include	"GameManager.h"


#define		PLAYER_NUM	4
//*****************************************************************************
//
//	sceneTitleクラス
//
//*****************************************************************************

//-----------------------------------------------------------------------------------
//	グローバル
//-----------------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	sceneTitle::sceneTitle(void) : orientation(0, 0, 0, 1), 
		mode(TITLE), playerNum(0), stageType(0)
	{

	}
	
	//	デストラクタ
	sceneTitle::~sceneTitle( void )
	{
		SafeDelete( m_Stage );
		SafeDelete( m_Camera );
		SafeDelete( m_Player );
		SafeDelete( m_CollisionStage );
	}
	
	//	初期化
	bool	sceneTitle::Initialize( void )
	{
		//view = new iexView();

		//	環境設定
		iexLight::SetAmbient(0x404040);
		iexLight::SetFog(800, 1000, 0);

		//	ライト設定
		Vector3 dir(1.0f, -1.0f, -0.5f);
		dir.Normalize();
		iexLight::DirLight(shader3D, 0, &dir, 0.8f, 0.8f, 0.8f);

		//	カメラ設定
		m_Camera = new Camera();

		//	ステージ
		m_CollisionStage = new iexMesh("DATA/BG/CollisionGround.IMO");
		m_Stage = new iexMesh( "DATA/BG/2_1/FIELD2_1.IMO" );

		//	当たり判定
		Collision::Initiallize( m_CollisionStage );

		//	プレイヤー
		m_Player = new PlayerManager();
		m_Player->Initialize( 0, PlayerData::Y2009, c_Move::TARGET[0] );
		m_Player->Initialize( 1, PlayerData::Y2009, c_Move::TARGET[1] );
		m_Player->Initialize( 2, PlayerData::Y2009, c_Move::TARGET[2] );
		m_Player->Initialize( 3, PlayerData::Y2009, c_Move::TARGET[3] );
		testpos = 0;
		t = 0;
		s_pos = m_Camera->GetPos();
		c_pos = m_Camera->GetPos();
		t_pos = Vector3( 0, 0, 0 );

		playerNum = 1;

		return	true;
	}

	//	サウンド初期化
	void	sceneTitle::InitSound( void )
	{

	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	更新
	void	sceneTitle::Update( void )
	{
		//	プレイヤー更新
		m_Player->Update();

		switch (mode)
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

		default:
			mode = 0;
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
		m_Stage->Render(shader3D, "full");
		m_Player->Render(shader3D, "full");

		switch (mode)
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

		default:
			mode = 0;
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
	void	sceneTitle::TitleUpdate()
	{
		//　SPACEでスタート
		if (KEY(KEY_SPACE) == 3)
		{
			mode = MENU;
		}
	}

	void	sceneTitle::TitleRender()
	{
		DrawString("タイトルだよ", 50, 50);
		DrawString("[SPACE]：メニューへ", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	メニュー
	//--------------------------------------------------------
	void	sceneTitle::MenuUpdate()
	{
		//	移動先を変える
		if (KEY(KEY_C) == 3 && t > 0.7f)
		{
			if (testpos < 3)	testpos++;
			else testpos = 0;
			t = 0;
		}
		if (KEY(KEY_D) == 3 && t > 0.7f)
		{
			if (testpos > 0)	testpos--;
			else testpos = 3;
			t = 0;

		}

		if (t <= 0.7f){
			CubicFunctionInterpolation( c_pos, s_pos, c_Move::TARGET[testpos], t - 0.1f );

		}
		else{
			s_pos = m_Camera->GetPos();
		}

		switch ( testpos )
		{
		case 0:
			if ( KEY( KEY_SPACE ) == 3 ) mode = SELECT_PLAYERNUM;
			break;
		case 1:
			if ( KEY( KEY_SPACE ) == 3 ) mode = OPTION;
			break;
		case 2:
			if (KEY(KEY_SPACE) == 3) mode = CREDIT;
			break;
		}

		//	カメラ更新
		m_Camera->Update(VIEW_MODE::SLERP, c_Move::TARGET[testpos]);
		m_Camera->SetPos(c_pos);

		t += 0.01f;
	}
	
	void	sceneTitle::MenuRender()
	{
		DrawString("メニュー画面だよ", 50, 50);
		DrawString("C・Vでどれか選んでね", 300, 400, 0xFFFFFF00);

		switch (testpos)
		{
		case 0:
			DrawString("みんなであそぶモード！", 50, 100);
			DrawString("[SPACE]：キャラ選択へ", 300, 200, 0xFFFFFF00);
			break;
		case 1:
			DrawString("オプション", 50, 100);
			DrawString("[SPACE]：オプションへ", 300, 200, 0xFFFFFF00);
			break;
		case 2:
			DrawString("つくった戦士たち！", 50, 100);
			DrawString("[SPACE]：クレジットへ", 300, 200, 0xFFFFFF00);
			break;
		}

	}

	//--------------------------------------------------------
	//	人数選択
	//--------------------------------------------------------
	void	sceneTitle::SelectPlayerNumUpdate(void)
	{
		//　「A」「D」で選択
		//　「SPACE」で決定
		//　「S」で戻る

		if (KEY(KEY_RIGHT) == 3)
		{
			if (playerNum < PLAYER_NUM)	playerNum++;
		}
		if (KEY(KEY_LEFT) == 3)
		{
			if (playerNum > 1)	playerNum--;
		}
		if (KEY(KEY_SPACE) == 3) mode = SELECT_CHARACTER;

		if (KEY(KEY_DOWN) == 3)	mode = MENU;
	}

	void	sceneTitle::SelectPlayerNumRender(void)
	{
		DrawString("人数選択だよ", 50, 50);
		DrawString("[SPACE]：キャラ選択へ", 300, 400, 0xFFFFFF00);

		char	str[64];
		wsprintf(str, "プレイヤー人数：%d\n", playerNum);
		IEX_DrawText(str, 300, 300, 200, 20, 0xFFFF00FF);
	}

	//--------------------------------------------------------
	//	キャラ選択
	//--------------------------------------------------------
	void	sceneTitle::SelectCharacterUpdate(void)
	{
		//　プレイヤーキャラ選択　→　CPUキャラ選択
		//　「A」「D」で選択
		//　「SPACE」で決定
		//　「S」で戻る

		int type[PLAYER_NUM];
		static int step = 0;
		switch (step)
		{
		case 0:		//　プレイヤーキャラ選択
			for (int p = 0; p < playerNum; p++)
			{
				type[p] = m_Player->GetType(p);
				if (KEY(KEY_RIGHT) == 3)
				{
					if (type[p] < 10)	//　←マジックナンバーはキャラの種類の最大数
					{
						m_Player->SetType(p, type[p] + 1);
					}
				}
				if (KEY(KEY_LEFT) == 3)
				{
					if (type[p] > 0)
					{
						m_Player->SetType(p, type[p] - 1);
					}
				}
			}
			if (KEY(KEY_SPACE) == 3) step++;
			if (KEY(KEY_DOWN) == 3)	mode = SELECT_PLAYERNUM;
			break;

		case 1:		//　CPUキャラ選択
			if (playerNum == PLAYER_NUM)	step++;
			else
			{
				for (int p = playerNum; p < PLAYER_NUM; p++)
				{
					type[p] = m_Player->GetType(p);
					if (KEY(KEY_RIGHT) == 3)
					{
						if (type[p] < 10)	//　←マジックナンバーはキャラの種類の最大数
						{
							m_Player->SetType(p, type[p] + 1);
						}
					}
					if (KEY(KEY_LEFT) == 3)
					{
						if (type[p] > 0)
						{
							m_Player->SetType(p, type[p] - 1);
						}
					}
				}
				if (KEY(KEY_SPACE) == 3) step++;
				if (KEY(KEY_DOWN) == 3) step--;
			}
			break;

		case 2:
			mode = SELECT_STAGE;
			step = 0;
			break;
		}
	}

	void	sceneTitle::SelectCharacterRender(void)
	{
		DrawString("キャラ選択だよ", 50, 50);
		DrawString("[SPACE]：ステージ選択へ", 300, 400, 0xFFFFFF00);

		char	str[64];
		for (int p = 0; p < PLAYER_NUM; p++)
		{
			wsprintf(str, "%dＰのキャラタイプ： %d\n", p + 1, m_Player->GetType(p));
			IEX_DrawText(str, 300, 300 + p * 20, 200, 20, 0xFFFF00FF);
		}
	}

	//--------------------------------------------------------
	//	ステージ選択
	//--------------------------------------------------------
	void	sceneTitle::SelectStageUpdate(void)
	{
		//　「A」「D」で選択
		//　「SPACE」で決定
		//　「S」で戻る

		if (KEY(KEY_SPACE) == 3) mode = SELECT_CHECK;

		if (KEY(KEY_RIGHT) == 3)
		{
			if (stageType < 8)	stageType++;
		}
		if (KEY(KEY_LEFT) == 3)
		{
			if (stageType > 0)	stageType--;
		}

		if (KEY(KEY_DOWN) == 3)	mode = SELECT_CHARACTER;
	}

	void	sceneTitle::SelectStageRender(void)
	{
		DrawString("ステージ選択だよ", 50, 50);
		DrawString("[SPACE]：最終確認へ", 300, 400, 0xFFFFFF00);

		char	str[64];
		wsprintf(str, "ステージ：%d番\n", stageType);
		IEX_DrawText(str, 300, 300, 200, 20, 0xFFFF00FF);
	}

	//--------------------------------------------------------
	//	最終確認
	//--------------------------------------------------------
	void	sceneTitle::SelectCheckUpdate(void)
	{
		//	「A」「D」で、はい・いいえ的なやつを選ぶ
		//　「SPACE」で決定
		//　「S」で戻る

		static bool ready = true;
		if (KEY(KEY_RIGHT) == 3) ready = !ready;
		if (KEY(KEY_LEFT) == 3) ready = !ready;

		if (ready)
		{
			if (KEY(KEY_SPACE) == 3)
			{
				for (int p = 0; p < playerNum; p++)
				{
					GameManager::charatype[p] = m_Player->GetType(p);
				}
				GameManager::playerNum = playerNum;
				GameManager::stageType = stageType;
				MainFrame->ChangeScene(new sceneMain());
				return;
			}
		}
		else
		{
			if (KEY(KEY_SPACE) == 3)	mode = SELECT_STAGE;
		}

		if (KEY(KEY_DOWN) == 3)	mode = SELECT_STAGE;
	}

	void	sceneTitle::SelectCheckRender(void)
	{
		//　選択情報
		char	str[64];
		for (int p = 0; p < PLAYER_NUM; p++)
		{
			wsprintf(str, "%dＰのキャラタイプ： %d\n", p + 1, m_Player->GetType(p));
			IEX_DrawText(str, 300, 300 + p * 20, 200, 20, 0xFFFF00FF);
		}
		wsprintf(str, "プレイヤー人数：%d\n", playerNum);
		IEX_DrawText(str, 300, 380, 200, 20, 0xFFFF00FF);
		wsprintf(str, "ステージ：%d番\n", stageType);
		IEX_DrawText(str, 300, 400, 200, 20, 0xFFFF00FF);

		DrawString("最終確認だよ", 50, 50);
		DrawString("[SPACE]：sceneMainへ", 300, 420, 0xFFFFFF00);

		static bool ready = true;
		if (KEY(KEY_RIGHT) == 3) ready = !ready;
		if (KEY(KEY_LEFT) == 3) ready = !ready;
		wsprintf(str, "はい＝1、いいえ＝0：%d番\n", ready);
		IEX_DrawText(str, 300, 440, 200, 20, 0xFFFF00FF);
	}

	//--------------------------------------------------------
	//	オプション
	//--------------------------------------------------------
	void	sceneTitle::OptionUpdate()
	{
		if (KEY(KEY_SPACE) == 3) mode = MENU;
	}
	
	void	sceneTitle::OptionRender()
	{
		DrawString("オプションだよ。設定いじってね", 50, 50);
		DrawString("[SPACE]：メニューへ", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	クレジット
	//--------------------------------------------------------
	void	sceneTitle::CreditUpdate()
	{
		if (KEY(KEY_SPACE) == 3) mode = MENU;
	}

	void	sceneTitle::CreditRender()
	{
		DrawString("つくった戦士たちの紹介だよ", 50, 50);
		DrawString("[SPACE]：メニューへ", 300, 400, 0xFFFFFF00);
	}