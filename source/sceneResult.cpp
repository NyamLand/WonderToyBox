
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"CharacterManager.h"
#include	"sceneTitle.h"
#include	"sceneMain.h"

#include	"sceneResult.h"

//********************************************************************************
//
//	sceneResultクラス
//
//********************************************************************************

//----------------------------------------------------------------------------
//	グローバル
//----------------------------------------------------------------------------

namespace
{
	namespace MOVE_MODE
	{
		enum
		{

		};
	}
}

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
		SafeDelete( m_Camera );
		SafeDelete( back );
		characterManager->Release();
		Random::Release();
	}

	//	初期化
	bool	sceneResult::Initialize( void )
	{
		//	カメラ設定
		m_Camera = new Camera();

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
			Vector3	pos = Vector3( 6.0f - ( 4.0f  *  i ), 0.0f, 0.0f );
			characterManager->Initialize( i, characterType, pos, true );
		}

		//	変数初期化
		lastBonus = 0;
		step = 0;
		playerNum = 0;
		wait = 0;
		for ( int i = 0; i < 4; i++ ){
			hundred[i] = 0;
			ten[i] = 0;
			one[i] = 0;
		}
		resultcount = 0;
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

		//	カメラ更新
		m_Camera->Update(VIEW_MODE::RESULT, Vector3( 0.0f, 0.0f, 0.0f ) );

		//	プレイヤー更新
		characterManager->Update();

		Production();
		
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

		characterManager->Render( shader3D, "toon" );

		//	デバッグ文字描画
		DrawString( "[sceneResult]", 50, 50 );
		DrawString( "すぺーすでタイトルへ", 300, 400, 0xFFFFFF00 );
		DrawString( GameInfo::LastBonus[lastBonus], 300, 150 );

		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "%d位 : player%d　%d枚", i + 1, resultInfo[i].p_num + 1, resultInfo[i].p_Coin );
			DrawString( str, 550, 250 + i * 30, 0xFFFFFFFF );
		}

		for (int i = 0; i < 4; i++){
			Vector3 stringPos;
			WorldToClient( characterManager->GetPos(i), stringPos, matView* matProjection );
			stringPos.y = 100;
			r_number->Render( ( int )stringPos.x - 40 * 2, ( int )stringPos.y, 64, 64, hundred[i] * 64, 0, 64, 64);	//	コイン三桁目
			r_number->Render( ( int )stringPos.x  -40 * 1, ( int )stringPos.y, 64, 64, ten[i] * 64, 0, 64, 64);	//	コイン二桁目
			r_number->Render( ( int )stringPos.x  -40 * 0, ( int )stringPos.y, 64, 64, one[i] * 64, 0, 64, 64);	//	コイン一桁目

			
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

	void	sceneResult::Production( void )
	{
		int	player = 0;
		int player_num = 0;

		switch (step)
		{
		case 0:
			Production_Rotation( player );
			break;

		case 1:
			player = 1;
			Production_Coin_hand_off(player_num);
			Production_Rotation( player );
			break;

		case 2:
			player=2;
			player_num=1;
			Production_Coin_hand_off(player_num);
			Production_Rotation(player);
			break;

		case 3:
			player=3;
			player_num=2;
			Production_Coin_hand_off(player_num);
			Production_Rotation(player);
			break;
		
		case 4:
			player_num=3;
			Production_Coin_hand_off(player_num);
			break;
		}

	}

	void	sceneResult::Production_Rotation(int playerNum)
	{
		resultcount++;
		for (int i = playerNum; i < 4; i++){

			hundred[i]++;
			if (hundred[i]>=10)hundred[i] = 0;
			ten[i]++;
			if (ten[i] >= 10)ten[i] = 0;
			one[i]++;
			if (one[i] >= 10)one[i] = 0;
		}
		if (resultcount > 60){
			resultcount = 0;
			step++;
		}
		if (KEY_Get(KEY_ENTER) == 3){
			resultcount = 0;
			step++;
		}
	}

	void	sceneResult::Production_Coin_hand_off(int playerNum)
	{
		hundred[playerNum] = coinNum[playerNum] / 100 % 10;
		ten[playerNum] = coinNum[playerNum] / 10 % 10;
		one[playerNum] = coinNum[playerNum] % 10;

	}