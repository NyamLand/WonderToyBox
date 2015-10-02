
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"Image.h"
#include	"GameManager.h"
#include	"Player.h"
#include	"Camera.h"
#include	"sceneTitle.h"
#include	"PlayerManager.h"
#include	"sceneMain.h"

#include	"sceneResult.h"

//********************************************************************************
//
//	sceneResultクラス
//
//********************************************************************************

//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------
int x = 200;
int y = 0;
	//	コンストラクタ
	sceneResult::sceneResult( void )
	{
	
	}

	//	デストラクタ
	sceneResult::~sceneResult( void )
	{
		SafeDelete( m_Camera );
		SafeDelete(m_Player);
		SafeDelete( back );
		Random::Release();
	}

	//	初期化
	bool	sceneResult::Initialize( void )
	{
		//	カメラ設定
		m_Camera = new Camera();

		//	プレイヤー初期化
		m_Player = new PlayerManager();

		//	画像初期化
		back = new iex2DObj( "DATA/Result/back.png");
		r_number = new iex2DObj( "DATA/UI/number.png");

		//	コリジョン
		collision = new iexMesh( "DATA/BG/CollisionGround.imo" );
		Collision::Initiallize( collision );
		
		for ( int i = 0; i < 4; i++ )
		{
			//	リザルト情報初期化
			coinNum[i] = gameManager->GetCoinNum(i);
			resultInfo[i].p_Coin = gameManager->GetCoinNum( i );
			resultInfo[i].p_num = i;

			//	プレイヤー初期化
			int		characterType = gameManager->GetCharacterType(i);
			Vector3	pos = Vector3(-20.0f + (10.0f * i), 0.0f, 0.0f);
			m_Player->Initialize(i, characterType, pos);
		}

		//	変数初期化
		lastBonus = 0;
		step = 0;
		playerNum = 0;
		wait = 0;
		
		//	乱数初期化
		Random::Initialize();

		//	ソート
		BubbleSort();

		//	ラストボーナス設定
		SetLastBonus();

		return	true;
	}

//----------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------

	//	更新
	void	sceneResult::Update( void ) 
	{
		//	画像移動
		if (KEY_Get(KEY_UP) == 1)y -= 10;
		if (KEY_Get(KEY_DOWN) == 1)y += 10;
		if (KEY_Get(KEY_RIGHT) == 1)x += 10;
		if (KEY_Get(KEY_LEFT) == 1)x -= 10;

		//	カメラ更新
		m_Camera->Update(VIEW_MODE::FIX, Vector3( 0.0f, 0.0f, 0.0f ) );

		//	プレイヤー更新
		m_Player->Update();

		//	タイトルへ
		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneTitle() );
			return;
		}
	}

	//	描画
	void	sceneResult::Render( void ) 
	{
		m_Camera->Activate();
		m_Camera->Clear();

		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		back->Render(0, 0, 1280, 720, 0, 0, 2048, 1024);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		m_Player->Render( shader3D,"toon" );

		//	デバッグ文字描画
		DrawString( "[sceneResult]", 50, 50 );
		DrawString( "すぺーすでタイトルへ", 300, 400, 0xFFFFFF00 );
		DrawString( GameInfo::LastBonus[lastBonus], 300, 150 );

		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "%d位 : player%d　%d枚", i + 1, resultInfo[i].p_num + 1, resultInfo[i].p_Coin );
			DrawString( str, 550, 250 + i * 30, 0xFFFFFFFF );
		}
		//コイン二桁目
		int ten = coinNum[0] / 10;
		//コイン一桁目
		int one = coinNum[0] % 10;

		int a = 0;
		r_number->Render(x + 50 * 0, y, 64, 64, ten * 64, 0, 64, 64);
		r_number->Render(x + 50 * 1, y, 64, 64, one * 64, 0, 64, 64);
	}

//----------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------

	//	バブルソート
	void	sceneResult::BubbleSort( void )
	{
		RESULT_INFO		temp;

		for ( int i = 0; i < 4; ++i )
		{
			//	後ろから順番にチェックしていく
			for ( int s =3; s > i; --s )
			{
				//	一つ下の要素と比較
				if ( resultInfo[s].p_Coin >	resultInfo[s - 1].p_Coin )
				{
					//	一時的に退避
					temp = resultInfo[s - 1];

					//	交換
					resultInfo[s - 1] = resultInfo[s];

					//	退避してたやつを戻す
					resultInfo[s] = temp;
				}
			}
		}
	}

	//	ラストボーナス設定
	void	sceneResult::SetLastBonus( void )
	{
		if ( Random::PercentageRandom( 0.3f ) )
		{
			//	ラストボーナスを設定
			lastBonus = gameManager->GetLastBonus();
		}
		else
		{
			//	違う結果が出るまでループ
			while ( lastBonus == gameManager->GetLastBonus() )
			{
				lastBonus = rand() % 5;
			}
		}
	}