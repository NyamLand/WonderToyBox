
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system\Scene.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"sceneMain.h"
#include	"sceneTitle.h"

#include	"sceneSelect.h"

//******************************************************************************
//
//	sceneSelectクラス
//
//******************************************************************************

//--------------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------------

	//	コンストラクタ
	sceneSelect::sceneSelect( void ) : mode(0)
	{
	
	}

	//	デストラクタ
	sceneSelect::~sceneSelect( void )
	{
		SafeDelete( view );
	}

	//	初期化
	bool	sceneSelect::Initialize( void )
	{
		//	カメラ設定
		view = new iexView();
		return	true;
	}

//--------------------------------------------------------
//	全体更新
//--------------------------------------------------------
	void	sceneSelect::Update( void )
	{
		switch ( mode )
		{
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
		
		default:
			mode = 0;
			break;
		}
	}

//--------------------------------------------------------
//	全体描画
//--------------------------------------------------------
	void	sceneSelect::Render( void )
	{
		view->Activate();
		view->Clear();
		
		switch ( mode )
		{
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

		default:
			mode = 0;
			break;
		}

		//	デバッグ文字描画
		//DrawString( "[sceneSelect]", 50, 50 );
		//DrawString( "すぺーすでメインへ",300, 400, 0xFFFFFF00 );
	}

//******************************************************************
//　各画面のメソッド
//******************************************************************

	//--------------------------------------------------------
	//	人数選択
	//--------------------------------------------------------
	void	sceneSelect::SelectPlayerNumUpdate( void )
	{
		if (KEY(KEY_SPACE) == 3) mode = SELECT_CHARACTER;
	}

	void	sceneSelect::SelectPlayerNumRender( void )
	{
		DrawString("人数選択だよ", 50, 50);
		DrawString("[SPACE]：キャラ選択へ", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	キャラ選択
	//--------------------------------------------------------
	void	sceneSelect::SelectCharacterUpdate( void )
	{
		if (KEY(KEY_SPACE) == 3) mode = SELECT_STAGE;
	}

	void	sceneSelect::SelectCharacterRender( void )
	{
		DrawString("キャラ選択だよ", 50, 50);
		DrawString("[SPACE]：ステージ選択へ", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	ステージ選択
	//--------------------------------------------------------
	void	sceneSelect::SelectStageUpdate( void )
	{
		if ( KEY( KEY_SPACE ) == 3 ) mode = SELECT_CHECK;
	}

	void	sceneSelect::SelectStageRender( void )
	{
		DrawString( "ステージ選択だよ", 50, 50 );
		DrawString( "[SPACE]：最終確認へ", 300, 400, 0xFFFFFF00 );
	}

	//--------------------------------------------------------
	//	最終確認
	//--------------------------------------------------------
	void	sceneSelect::SelectCheckUpdate( void )
	{
		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneMain() );
			return;
		}
	}

	void	sceneSelect::SelectCheckRender( void )
	{
		DrawString( "最終確認だよ", 50, 50 );
		DrawString( "[SPACE]：sceneMainへ", 300, 400, 0xFFFFFF00 );
	}
