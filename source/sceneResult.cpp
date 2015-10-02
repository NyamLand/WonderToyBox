
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
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

	//	コンストラクタ
	sceneResult::sceneResult( void )
	{
	
	}

	//	デストラクタ
	sceneResult::~sceneResult( void )
	{
		SafeDelete( view );
		SafeDelete(m_Player);
		SafeDelete( back );
		Random::Release();
	}

	//	初期化
	bool	sceneResult::Initialize( void )
	{
		//	カメラ設定
		view = new iexView();
		m_Player = new PlayerManager();
		back = new iex2DObj("DATA/Result/back.png");

		m_Camera = new Camera();
		

		for ( int i = 0; i < 4; i++ )
		{
			coinNum[i] = 0;
			resultInfo[i].p_Coin = GameManager::GetCoinNum( i );
			resultInfo[i].p_num = i;

			int		characterType = GameManager::GetCharacterType(i);
			//Vector3	pos = Vector3(-20.0f + (10.0f * i), 0.0f, 0.0f);
			Vector3	pos = Vector3(0.0f, 0.0f, 0.0f);
			m_Player->Initialize(i, characterType, pos);
		//	m_Player->Update();
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
		view->Set(Vector3(0.0f,0.0f,-10.0f),m_Player->GetPos(0));

		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneTitle() );
			return;
		}
	}

	//	描画
	void	sceneResult::Render( void ) 
	{
		view->Activate();
		view->Clear();

		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		back->Render(0, 0, 1280, 720, 0, 0, 2048, 1024);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		m_Player->Render(shader3D, "ShadowBuf");

		//	デバッグ文字描画
		DrawString( "[sceneResult]", 50, 50 );
		DrawString( "すぺーすでタイトルへ", 300, 400, 0xFFFFFF00 );
		DrawString( GameInfo::LastBonus[lastBonus], 300, 150 );

		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "%d位 : player%d　%d枚", i + 1, resultInfo[i].p_num + 1, resultInfo[i].p_Coin );
			DrawString( str, 550, 250 + i * 30, 0xFFFFFFFF );
		}
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
			lastBonus = GameManager::GetLastBonus();
		}
		else
		{
			//	違う結果が出るまでループ
			while ( lastBonus == GameManager::GetLastBonus() )
			{
				lastBonus = rand() % 5;
			}
		}
	}