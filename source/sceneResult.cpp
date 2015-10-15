
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
			RESULT_MODE,
			SELECT_MODE
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
		SafeDelete( Sback );
		SafeDelete( Smenu );
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
		Sback = new iex2DObj("DATA/Result/black.png");
		Smenu = new iex2DObj("DATA/Result/result-cho.png");

		//	コリジョン
		collision = new iexMesh( "DATA/BG/CollisionGround.imo" );
		Collision::Initiallize( collision );
		
		for ( int i = 0; i < 4; i++ )
		{
			//	リザルト情報初期化
			coinNum[i] = gameManager->GetCoinNum(i);
			resultInfo[i].p_Coin = gameManager->GetCoinNum( i );
			resultInfo[i].p_num = i;
			addcoinNum[i] = 0;
			rank[i] = 0;
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
		Modeflg = false;
		for ( int i = 0; i < 4; i++ ){
			hundred[i] = 0;
			ten[i] = 0;
			one[i] = 0;
		}
		resultcount = 0;
		Mode = 0;
		Sy = -720.0f;
		Sx = 0.0f;
		//	乱数初期化
		Random::Initialize();


		//	ラストボーナス設定
		SetLastBonus();

		//	ソート
		BubbleSort();

		//	ランキング設定
		SetRank();

		return	true;
	}

//----------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------

	//	更新
	void	sceneResult::Update( void ) 
	{
		switch (Mode)
		{
		case 0:
			ResultUpdate();
			break;
		case 1:
			SelectUpdata();
			break;
		default:
			break;
		}
		
	}

	//リザルト時の更新
	void	sceneResult::ResultUpdate( void )
	{
		//	カメラ更新
		m_Camera->Update(VIEW_MODE::RESULT, Vector3(0.0f, 0.0f, 0.0f));

		//	プレイヤー更新
		characterManager->Update();
		for (int i = 0; i < 4; i++){
			addcoinNum[i] = coinNum[i] + lastBonus;
		}
		Production();
		if (Modeflg)Mode = 1;
		//	タイトルへ
		if (KEY(KEY_SPACE) == 3)
		{
			MainFrame->ChangeScene(new sceneTitle());
			return;
		}
	}

	//セレクト時の更新
	void	sceneResult::SelectUpdata(void)
	{
		Sy +=20;
		if (Sy > 0){
			Sy = 0;
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
			Vector3 addcoinPos;
			Vector3 juniPos;
			WorldToClient( characterManager->GetPos(i), stringPos, matView* matProjection );
			WorldToClient( characterManager->GetPos(i), addcoinPos, matView* matProjection );
			WorldToClient( characterManager->GetPos(i), juniPos, matView* matProjection );
			stringPos.y = 100;
			addcoinPos.y = 250;
			juniPos.y = 350;
			r_number->Render( ( int )stringPos.x - 40 * 2, ( int )stringPos.y, 64, 64, hundred[i] * 64, 0, 64, 64 );	//	コイン三桁目
			r_number->Render( ( int )stringPos.x  -40 * 1, ( int )stringPos.y, 64, 64, ten[i] * 64, 0, 64, 64 );	//	コイン二桁目
			r_number->Render( ( int )stringPos.x  -40 * 0, ( int )stringPos.y, 64, 64, one[i] * 64, 0, 64, 64 );	//	コイン一桁目

			if ( Modeflg ){
				r_number->Render( ( int )addcoinPos.x - 40 * 2, ( int )addcoinPos.y, 64, 64, ( addcoinNum[i] / 100 % 10 ) * 64, 0, 64, 64 );	//	コイン三桁目
				r_number->Render( ( int )addcoinPos.x - 40 * 1, ( int )addcoinPos.y, 64, 64, ( addcoinNum[i] / 10 % 10 ) * 64, 0, 64, 64 );	//	コイン二桁目
				r_number->Render( ( int )addcoinPos.x - 40 * 0, ( int )addcoinPos.y, 64, 64, ( addcoinNum[i] % 10 ) * 64, 0, 64, 64 );	//	コイン一桁目

				r_number->Render((int)juniPos.x - 40 * 2, (int)juniPos.y, 128, 64, (rank[i]) * 128, 128, 128, 64);	//	順位
			}
		}

		/*if (Mode >= 1){
			SelectRender();
		}*/
	}

	//セレクト画面描画
	void	sceneResult::SelectRender( void )
	{
		Sback->Render((int)Sx, (int)Sy, 1280, 720, 0, 0, 64, 64);
		if (Sy >= 0)
		{
			Smenu->Render(50, 150, 1200, 720, 0, 0, 512, 512);
			Smenu->Render(50, 150, 1200, 250, 512, 0, 512, 170);
			Smenu->Render(50, 300, 1200, 250, 512, 100, 512, 170);
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

	//	リザルト演出
	void	sceneResult::Production( void )
	{
		int	player = 0;
		int player_num = 0;

		switch (step)
		{
		case 0:
			ProductionRotation( player );
			break;

		case 1:
			player = 1;
			ProductionCoinHandOff(player_num);
			ProductionRotation(player);
			break;

		case 2:
			player=2;
			player_num=1;
			ProductionCoinHandOff(player_num);
			ProductionRotation(player);
			break;

		case 3:
			player=3;
			player_num=2;
			ProductionCoinHandOff(player_num);
			ProductionRotation(player);
			break;
		
		case 4:
			player_num=3;
			ProductionCoinHandOff(player_num);
			step++;
			break;
		case 5:
			resultcount++;
			if (resultcount > 60){
				resultcount = 0;
				step++;
			}
			break;
		case 6:
			Modeflg = true;
		}

	}

	//	リザルト時の回転演出関数
	void	sceneResult::ProductionRotation(int playerNum)
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

	//	リザルトの値渡し
	void	sceneResult::ProductionCoinHandOff(int playerNum)
	{
		hundred[playerNum] = coinNum[playerNum] / 100 % 10;
		ten[playerNum] = coinNum[playerNum] / 10 % 10;
		one[playerNum] = coinNum[playerNum] % 10;

	}

	//	順位設定
	void	sceneResult::SetRank( void )
	{
		for ( int i = 0; i < 4; i++ )
		{
			for ( int n = 0; n < 4; n++ )
			{
				if ( i == resultInfo[n].p_num )	rank[i] =n;
			}
		}
	}