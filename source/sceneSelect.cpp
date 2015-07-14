
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
	sceneSelect::sceneSelect( void )
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

//--------------------------------------------------------------------------------
//	更新・描画
//--------------------------------------------------------------------------------

	//	更新
	void	sceneSelect::Update( void )
	{
		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneMain() );
			return;
		}
	}

	//	描画
	void	sceneSelect::Render( void )
	{
		view->Activate();
		view->Clear();
		
		//	デバッグ文字描画
		DrawString( "[sceneSelect]", 50, 50 );
		DrawString( "すぺーすでメインへ",300, 400, 0xFFFFFF00 );
	}
