
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"system/Scene.h"
#include	"GlobalFunction.h"
#include	"Particle.h"
#include	"sceneMain.h"
#include	"sceneSelect.h"

#include	"sceneTitle.h"

//*****************************************************************************
//
//	sceneTitleクラス
//
//*****************************************************************************

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	sceneTitle::sceneTitle( void )
	{
	
	}
	
	//	デストラクタ
	sceneTitle::~sceneTitle( void )
	{
		SafeDelete( view );
	}
	
	//	初期化
	bool	sceneTitle::Initialize( void )
	{
		view = new iexView();
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
		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneSelect() );
			return;
		}
	}

	//	描画
	void	sceneTitle::Render( void )
	{
		//	画面クリア
		view->Activate();
		view->Clear();

		//	デバッグ用
		DrawString( "[sceneTitle]", 50, 50 );
		DrawString( "すぺーす押してね", 300, 400, 0xFFFFFF00 );
		DrawString( "はーいぷしゅっ！", 1100, 700, 0xFFFFFF00 );
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	