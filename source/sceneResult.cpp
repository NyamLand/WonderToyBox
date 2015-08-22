
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"GameManager.h"
#include	"sceneTitle.h"
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
	}

	//	初期化
	bool	sceneResult::Initialize( void )
	{
		//	カメラ設定
		view = new iexView();

		for ( int i = 0; i < 4; i++ )
		{
			resultInfo[i].p_Coin = GameManager::GetCoinNum( i );
			resultInfo[i].p_num = i;
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

		//	デバッグ文字描画
		DrawString( "[sceneResult]", 50, 50 );
		DrawString( "すぺーすでタイトルへ", 300, 400, 0xFFFFFF00 );

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