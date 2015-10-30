
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
#include	"sceneMenu.h"
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
		SafeDelete( mainView );
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
		mainView = new Camera();

		//	画像初期化
		back = new iex2DObj( "DATA/Result/result-back.png");
		r_number = new iex2DObj( "DATA/UI/number.png");
		Sback = new iex2DObj("DATA/Result/black.png");
		Smenu = new iex2DObj("DATA/Result/result-cho.png");
		result = new iex2DObj("DATA/UI/menu-head.png");

		//	コリジョン
		collision = new iexMesh( "DATA/BG/CollisionGround.imo" );
		Collision::Initiallize( collision );
		
		//	変数初期化
		lastBonus = 0;
		step = 0;
		wait = 0;
		resultcount = 0;
		mode = MOVE_MODE::RESULT_MODE;
		Sy = -720;
		StringPos_Y = 0;
		waitTimer = 0;

		//フラグ全初期化
		Modeflg = false;
		bonusflg = false;
		addCoinflg	= false;

		//	乱数初期化
		Random::Initialize();
		
		//	ラストボーナス設定
		SetLastBonus();

		for ( int i = 0; i < 4; i++ )
		{
			lastBonusNum[i] = 0;

			//	数値構造体初期化
			{
				number[i].hundred = 0;
				number[i].ten = 0;
				number[i].one = 0;
				number[i].H_flg = false;
			}

			//	ボーナス数値構造体
			{
				BonusNumber[i].hundred = 0;
				BonusNumber[i].ten = 0;
				BonusNumber[i].one = 0;
				BonusNumber[i].H_flg = false;

			}
			
			//	ランキング用構造体
			{
				rank[i].rank = 0;
				rank[i].rankflg = false;
			}
			
			//	リザルト情報初期化
			coinNum[i] = gameManager->GetCoinNum(i);
			lastBonusNum[i] = 0;
			totalCoinNum[i] = coinNum[i] + lastBonusNum[i];
			resultInfo[i].p_Coin = gameManager->GetCoinNum( i );
			resultInfo[i].p_num = i;
			resultInfo[i].p_addCoin = coinNum[i]+lastBonusNum[i];

			//	プレイヤー初期化
			int		characterType = gameManager->GetCharacterType( i );
			Vector3	pos = Vector3( 6.0f - ( 4.0f  *  i ), 0.0f, 0.0f );
			characterManager->Initialize( i, characterType, pos, true );
		}

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
		switch ( mode )
		{
		case MOVE_MODE::RESULT_MODE:
			ResultUpdate();
			break;

		case MOVE_MODE::SELECT_MODE:
			SelectUpdata();
			break;
		}
	}

	//リザルト時の更新
	void	sceneResult::ResultUpdate( void )
	{
		//	カメラ更新
		mainView->Update( VIEW_MODE::RESULT, Vector3( 0.0f, 0.0f, 0.0f ) );

		//	プレイヤー更新
		characterManager->Update();
		
		//	演出
		Production();
		if ( Modeflg )mode = MOVE_MODE::SELECT_MODE;
	}

	//セレクト時の更新
	void	sceneResult::SelectUpdata( void )
	{
		Sy +=20;
		if ( Sy > 0 ){
			Sy = 0;
		}

		if ( input[0]->Get( KEY_UP ) == 3)		StringPos_Y--;
		if ( input[0]->Get( KEY_DOWN ) == 3 )	StringPos_Y++;
		if ( StringPos_Y >= 3 )	StringPos_Y = 0;
		if ( StringPos_Y < 0 )		StringPos_Y = 2;

		if ( KEY_Get( KEY_SPACE ) == 3 || KEY_Get( KEY_A ) == 3 )
		{
			switch ( StringPos_Y )
			{
			case 0:
				//	ゲーム情報初期化
				gameManager->RetryInitialize();
				MainFrame->ChangeScene( new sceneMain() );
				return;
				break;

			case 1:
				MainFrame->ChangeScene( new sceneMenu() );
				return;
				break;

			case 2:
				MainFrame->ChangeScene( new sceneTitle() );
				return;
				break;
			}
		}
	}

	//	描画
	void	sceneResult::Render( void ) 
	{
		mainView->Activate();
		mainView->Clear();

		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		back->Render(0, 0, 1280, 720, 0, 0, 1280, 720);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		characterManager->Render( shader3D, "toon" );

		result->Render(510, -10, 256, 128, 0, 0, 512, 256);	//	リザルト描画

		for (int i = 0; i < 4; i++){
			Vector3 stringPos;
			Vector3	bonusPos;
			Vector3 rankingPos;
			WorldToClient( characterManager->GetPos(i), stringPos, matView* matProjection );
			WorldToClient( characterManager->GetPos(i), bonusPos, matView* matProjection);
			WorldToClient( characterManager->GetPos(i), rankingPos, matView* matProjection );
			stringPos.y = 100;
			bonusPos.y = 170;
			rankingPos.y = 350;
			
			
			//	取得コイン枚数の描画
			{
				ResultRender(number[i], stringPos);
			}

			//	ボーナスコイン枚数の描画
			{
				if(bonusflg){
					ResultRender(BonusNumber[i], bonusPos);
				}
			}

			//	順位の描画
			{
				if (rank[i].rankflg){
					r_number->Render((int)rankingPos.x - 40 * 2, (int)rankingPos.y, 128, 64, rank[i].rank * 128, 128, 128, 64);
				}
			}
		//	デバッグ文字描画
		/*DrawString( "[sceneResult]", 50, 50 );
		DrawString( "すぺーすでタイトルへ", 300, 400, 0xFFFFFF00 );
		DrawString( GameInfo::LastBonus[lastBonus], 300, 150 );

		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "%d位 : player%d　%d枚", i + 1, resultInfo[i].p_num + 1, resultInfo[i].p_addCoin );
			DrawString( str, 550, 250 + i * 30, 0xFFFFFFFF );
		}*/
			}
		

		if ( mode == MOVE_MODE::SELECT_MODE ){
			SelectRender();
		}
	}

	//	リザルト描画
	void	sceneResult::ResultRender(NUMBER_INFO& number, Vector3 Pos)
	{
		if (number.H_flg){
			r_number->Render((int)Pos.x - 40 * 2, (int)Pos.y, 64, 64, number.hundred * 64, 0, 64, 64);	//	コイン三桁目
			r_number->Render((int)Pos.x - 40 * 1, (int)Pos.y, 64, 64, number.ten * 64, 0, 64, 64);		//	コイン二桁目
			r_number->Render((int)Pos.x - 40 * 0, (int)Pos.y, 64, 64, number.one * 64, 0, 64, 64);		//	コイン一桁目
		}
		else{

			r_number->Render((int)Pos.x - 40 * 2, (int)Pos.y, 64, 64, number.ten * 64, 0, 64, 64);		//	コイン二桁目
			r_number->Render((int)Pos.x - 40 * 1, (int)Pos.y, 64, 64, number.one * 64, 0, 64, 64);		//	コイン一桁目
		}

	}

	//セレクト画面描画
	void	sceneResult::SelectRender( void )
	{
		Sback->Render(0, Sy, 1280, 720, 0, 0, 64, 64);
		if (Sy >= 0)
		{
			for (int i = 0; i < 3; i++){
				Smenu->Render(400, 200 + i * 128, 512, 128, 0, i * 128, 512, 128);

			}
			Smenu->Render(400, 200 + StringPos_Y * 128, 512, 128, 512, StringPos_Y * 128, 512, 128);
			
			
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
				if ( resultInfo[s].p_addCoin >	resultInfo[s - 1].p_addCoin )
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
		switch ( step )
		{
		case 0://	コイン枚数の回転
			ProductionRotation( step );
			break;

		case 1://	1Pのコイン枚数決定とその他回転
			ProductionCoinHandOff( number[step - 1],coinNum[step - 1] );
			ProductionRotation( step );
			break;

		case 2://	2Pのコイン枚数決定とその他回転
			ProductionCoinHandOff( number[step - 1], coinNum[step - 1] );
			ProductionRotation( step );
			break;

		case 3://	3Pのコイン枚数決定とその他回転
			ProductionCoinHandOff( number[step - 1], coinNum[step - 1] );
			ProductionRotation( step );
			break;
		
		case 4://	4Pのコイン枚数描画
			ProductionCoinHandOff( number[step - 1], coinNum[step - 1] );
			step++;
			break;

		case 5://ボーナス演出->後で作る
			waitTimer++;
			if (waitTimer > 30){
				waitTimer = 0;
				step++;
			}
			break;

		case 6://ボーナス表示->後で作る
			for (int i = 0; i < 4; i++)
			{
				ProductionCoinHandOff(BonusNumber[i], lastBonusNum[i]);
			}

			bonusflg = true;


			waitTimer++;
			if (waitTimer > 30){
				waitTimer = 0;
				step++;
			}
			break;

		case 7://	コイン合算値を描画


			waitTimer++;
			if (waitTimer > 60){
				waitTimer = 0;
				step++;
			}
			break;
		case 8:	//	ビリから順に描画
			RankRender(3);
			waitTimer++;
			if (waitTimer > 30){
				waitTimer = 0;
				step++;
			}
			break;
		case 9:
			RankRender(2);
			waitTimer++;
			if (waitTimer > 30){
				waitTimer = 0;
				step++;
			}
			break;
		case 10:
			RankRender(1);
			waitTimer++;
			if (waitTimer > 30){
				waitTimer = 0;
				step++;
			}
			break;
		case 11:
			RankRender(0);
			if (KEY_Get(KEY_A) == 3 || KEY_Get(KEY_SPACE))step++;
			break;
		case 12:
			Modeflg = true;

			break;
		}

	}

	//	リザルト時の回転演出関数
	void	sceneResult::ProductionRotation(int playerNum)
	{
		resultcount++;
		for (int i = playerNum; i < 4; i++){

			number[i].hundred++;
			if (number[i].hundred >= 10)number[i].hundred = 0;
			number[i].ten++;
			if (number[i].ten >= 10)number[i].ten = 0;
			number[i].one++;
			if (number[i].one >= 10)number[i].one = 0;
		}
		if (resultcount > 60){
			resultcount = 0;
			step++;
		}
		if (KEY_Get(KEY_A) == 3){
			resultcount = 0;
			step++;
		}
	}

	//	リザルトの値渡し
	void	sceneResult::ProductionCoinHandOff(NUMBER_INFO& number,int coinNum)
	{
		number.hundred = coinNum / 100 % 10;
		if (number.hundred > 0){
			number.H_flg = true;
		}
		number.ten = coinNum / 10 % 10;
		number.one = coinNum % 10;
	}

	//	ランク設定
	void	sceneResult::SetRank( void )
	{
		for ( int i = 0; i < 4; i++ )
		{
			for ( int n = 0; n < 4; n++ )
			{
				if ( i == resultInfo[n].p_num )
				{
					rank[i].rank = n;
				}
			}
		}
	}

	void	sceneResult::RankRender(int ranking)
	{
		for (int i = 0; i < 4; i++)
		{
			if (rank[i].rank == ranking)
			if (rank[i].rankflg == false){
				rank[i].rankflg = true;
			}
		}
	}