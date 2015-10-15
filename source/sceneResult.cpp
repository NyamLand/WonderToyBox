
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

int	X = 400;
int Y = 200;
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
		Mode = MOVE_MODE::RESULT_MODE;
		Sy = -720.0f;
		StringPos_Y = 0;
		for ( int i = 0; i < 4; i++ ){
			number[i].hundred = 0;
			number[i].ten = 0;
			number[i].one = 0;
			playerNum[i] = i;
		}


		//フラグ全初期化
		Modeflg = false;
		addCoinflg	= false;
		for (int i = 0; i < 4; i++){
			rankingflg[i] = false;
		}

		//	乱数初期化
		Random::Initialize();


		//	ラストボーナス設定
		SetLastBonus();

		for ( int i = 0; i < 4; i++ )
		{
			lastBonusNum[i] = 0;
			//	リザルト情報初期化
			coinNum[i] = gameManager->GetCoinNum(i);
			resultInfo[i].p_Coin = gameManager->GetCoinNum( i );
			resultInfo[i].p_num = i;
			resultInfo[i].p_addCoin = coinNum[i]+lastBonusNum[i];
			//	プレイヤー初期化
			int		characterType = gameManager->GetCharacterType(i);
			Vector3	pos = Vector3( 6.0f - ( 4.0f  *  i ), 0.0f, 0.0f );
			characterManager->Initialize( i, characterType, pos, true );
		}

		//	ソート
		BubbleSort();

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
		case MOVE_MODE::RESULT_MODE:
			ResultUpdate();
			break;
		case MOVE_MODE::SELECT_MODE:
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
		
		Production();
		if (Modeflg)Mode = MOVE_MODE::SELECT_MODE;
		//	タイトルへ
		//デバッグ用
		if (KEY_Get(KEY_RIGHT) == 1){
			X += 10;
		}
		if (KEY_Get(KEY_LEFT) == 1){
			X -= 10;
		}
		if (KEY_Get(KEY_DOWN) == 1){
			Y += 10;

		}

		if (KEY_Get(KEY_UP) == 1){
			Y -= 10;

		}
	}

	//セレクト時の更新
	void	sceneResult::SelectUpdata(void)
	{
		Sy +=20;
		if (Sy > 0){
			Sy = 0;
		}
		if (KEY_Get(KEY_DOWN) == 3){
			StringPos_Y++;
			if (StringPos_Y > 2)StringPos_Y = 0;
		}

		if (KEY_Get(KEY_UP) == 3){
			StringPos_Y--;
			if (StringPos_Y < 0)StringPos_Y = 2;
		}

		if (StringPos_Y == 2 && KEY_Get(KEY_SPACE) == 3)MainFrame->ChangeScene(new sceneTitle);

		

	}


	//	描画
	void	sceneResult::Render( void ) 
	{
		m_Camera->Activate();
		m_Camera->Clear();

		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		back->Render(0, 0, 1280, 720, 0, 0, 1280, 720);
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


		result->Render(510, -10, 256, 128, 0, 0, 512, 256);	//	リザルト描画

		for (int i = 0; i < 4; i++){
			Vector3 stringPos;
			Vector3 addcoinPos;
			Vector3 rankingPos;
			WorldToClient( characterManager->GetPos(i), stringPos, matView* matProjection );
			WorldToClient(characterManager->GetPos(i), addcoinPos, matView* matProjection);
			WorldToClient(characterManager->GetPos(i), rankingPos, matView* matProjection);
			stringPos.y = 100;
			addcoinPos.y = 250;
			rankingPos.y = 350;
			r_number->Render( ( int )stringPos.x - 40 * 2, ( int )stringPos.y, 64, 64, number[i].hundred * 64, 0, 64, 64);	//	コイン三桁目
			r_number->Render( ( int )stringPos.x  -40 * 1, ( int )stringPos.y, 64, 64, number[i].ten * 64, 0, 64, 64);	//	コイン二桁目
			r_number->Render( ( int )stringPos.x  -40 * 0, ( int )stringPos.y, 64, 64, number[i].one * 64, 0, 64, 64);	//	コイン一桁目


			//r_number->Render((int)addcoinPos.x - 40 * 2, (int)addcoinPos.y, 64, 64, (resultInfo[i].p_addCoin / 100 % 10) * 64, 0, 64, 64);	//	コイン三桁目
			//r_number->Render((int)addcoinPos.x - 40 * 1, (int)addcoinPos.y, 64, 64, (resultInfo[i].p_addCoin / 10 % 10) * 64, 0, 64, 64);	//	コイン二桁目
			//r_number->Render((int)addcoinPos.x - 40 * 0, (int)addcoinPos.y, 64, 64, (resultInfo[i].p_addCoin % 10) * 64, 0, 64, 64);	//	コイン一桁目

			if (addCoinflg){
				r_number->Render((int)addcoinPos.x - 40 * 2, (int)addcoinPos.y, 64, 64, number[i].hundred * 64, 0, 64, 64);	//	コイン三桁目
				r_number->Render((int)addcoinPos.x - 40 * 1, (int)addcoinPos.y, 64, 64, number[i].ten * 64, 0, 64, 64);	//	コイン二桁目
				r_number->Render((int)addcoinPos.x - 40 * 0, (int)addcoinPos.y, 64, 64, number[i].one * 64, 0, 64, 64);	//	コイン一桁目


				r_number->Render((int)rankingPos.x - 40 * 2, (int)rankingPos.y, 128, 64, (resultInfo[i].p_num) * 128, 128, 128, 64);	//	順位
			}
		}

		if (Mode ==MOVE_MODE::SELECT_MODE){
			SelectRender();
		}


		//デバック用
		sprintf_s(str, "%d\n%d", X, Y);
		DrawString(str, 0, 250, 0xFFFFFFFF);
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
		

		switch (step)
		{
		case 0://	コイン枚数の回転
			ProductionRotation(playerNum[0]);
			break;

		case 1://	1Pのコイン枚数描画とその他回転
			ProductionCoinHandOff(number[0],resultInfo[0].p_Coin);
			ProductionRotation(playerNum[1]);
			break;

		case 2://	2Pのコイン枚数描画とその他回転
			ProductionCoinHandOff(number[1], resultInfo[1].p_Coin);
			ProductionRotation(playerNum[2]);
			break;

		case 3://	3Pのコイン枚数描画とその他回転
			ProductionCoinHandOff(number[2], resultInfo[2].p_Coin);
			ProductionRotation(playerNum[3]);
			break;
		
		case 4://	4Pのコイン枚数描画
			ProductionCoinHandOff(number[3], resultInfo[3].p_Coin);
			step++;
			break;
		case 5://ボーナス演出->後で作る
			resultcount++;
			if (resultcount > 30){
				resultcount = 0;
				step++;
			}
			break;
		case 6://ボーナス表示->後で作る
			resultcount++;
			if (resultcount > 30){
				resultcount = 0;
				step++;
			}
			break;
		case 7://	コイン合算値を描画
			ProductionCoinHandOff(number[0], resultInfo[0].p_addCoin);
			ProductionCoinHandOff(number[1], resultInfo[1].p_addCoin);
			ProductionCoinHandOff(number[2], resultInfo[2].p_addCoin);
			ProductionCoinHandOff(number[3], resultInfo[3].p_addCoin);

			addCoinflg = true;
			/*resultcount++;
			if (resultcount > 30){
				resultcount = 0;
				step++;
			}*/
			if (KEY_Get(KEY_ENTER) == 3)step++;
			break;
		case 8:	//
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
		if (KEY_Get(KEY_ENTER) == 3){
			resultcount = 0;
			step++;
		}
	}
	//	リザルトの値渡し
	void	sceneResult::ProductionCoinHandOff(NUMBER& number,int coinNum)
	{
		number.hundred = coinNum / 100 % 10;
		number.ten = coinNum / 10 % 10;
		number.one = coinNum % 10;

	}