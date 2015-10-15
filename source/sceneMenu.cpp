
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Sound.h"
#include	"Screen.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"Random.h"
#include	"BaseChara.h"
#include	"CharacterManager.h"
#include	"UI.h"
#include	"sceneMain.h"
#include	"sceneLoad.h"
#include	"sceneTitle.h"

#include	"sceneMenu.h"

//*******************************************************************************
//
//	sceneMenuクラス
//
//*******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	sceneMenu::sceneMenu( void )
	{
	
	}

	//	デストラクタ
	sceneMenu::~sceneMenu( void )
	{
		Release();
	}

	//	初期化
	bool	sceneMenu::Initialize( void )
	{
		//	camera
		m_Camera = new Camera();

		3;

		//	random
		Random::Initialize();

		//	screen
		screen->Initialize();
		screen->SetScreenMode( SCREEN_MODE::WHITE_IN, 0.5f );

		//	画像読み込み

		//	画像構造体初期化
		ImageInitialize( wipeImage, 640, 360, 1280, 720, 0, 0, 2048, 2048 );
		return	true;
	}

	//	解放
	void	sceneMenu::Release( void )
	{

	}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------

	//	更新
	void	sceneMenu::Update( void )
	{

	}

	//	描画
	void	sceneMenu::Render( void )
	{
		//	camera
		m_Camera->Activate();
		m_Camera->Clear();
	}

//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

	//	キャラクター選択
	void	sceneMenu::SelectCharacter( void )
	{

	}

	//	ステージ選択
	void	sceneMenu::SelectStage( void )
	{

	}

	//	最終確認
	void	sceneMenu::SelectCheck( void )
	{

	}

	//	メインへ
	void	sceneMenu::MoveMain( void )
	{
		MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
		return;
	}

//-------------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------------