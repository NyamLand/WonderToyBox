
#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	< process.h >
#include	"Camera.h"
#include	"sceneLoad.h"
using namespace std;

//*******************************************************************************
//
//	sceneLoadクラス
//
//*******************************************************************************

//----------------------------------------------------------------------------------
//	グローバル
//----------------------------------------------------------------------------------
	
	//	static宣言
	bool	sceneLoad::threadState;

//----------------------------------------------------------------------------------
//	初期化・解放	
//----------------------------------------------------------------------------------
	
	//	コンストラクタ
	sceneLoad::sceneLoad( Scene*	nextScene ) : newScene( nextScene )
	{
		//	フラグ初期化
		threadState = false;
		loadflg = false;
	}

	//	デストラクタ
	sceneLoad::~sceneLoad( void )
	{
		SafeDelete( bgImage[0].obj );
		SafeDelete( bgImage[1].obj );
		SafeDelete( load_anykey.obj );	
	}

	//	初期化
	bool	sceneLoad::Initialize( void )
	{
		//    環境設定
		iexLight::SetAmbient( 0x808080 );
		iexLight::SetFog( 800, 1000, 0 );

		//	カメラ設定
		mainView = new Camera();

		//	背景初期化
		bgImage[0].obj = new iex2DObj( "DATA/UI/Load/Lord-back01.png" );
		bgImage[1].obj = new iex2DObj( "DATA/UI/Load/Lord-back02.png" );
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w = static_cast<int>( iexSystem::ScreenWidth );
		int h = static_cast<int>( iexSystem::ScreenHeight );
		ImageInitialize( bgImage[0], x, y, w, h, 0, 0, 1280, 720 );

		x = static_cast<int>( iexSystem::ScreenWidth * 1.5f );
		ImageInitialize( bgImage[1], x, y, w, h, 0, 0, 1280, 720 );

		//	PressSpace画像構造体設定
		load_anykey.obj = new iex2DObj( "DATA/UI/pressspace.png" );
		x = static_cast<int>( iexSystem::ScreenWidth * 0.86f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.93f );
		w = static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		h = static_cast<int>( iexSystem::ScreenHeight * 0.18f );
		ImageInitialize( load_anykey, x, y, w, h, 0, 0, 256, 128 );
		
		//	別スレッド作成
		_beginthread( Thread, 0, ( void* )newScene );
		
		return	true;
	}

//----------------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------------
	
	//	更新
	void	sceneLoad::Update( void )
	{

		//	点滅更新
		FlashingUpdate( load_anykey, D3DX_PI / 180 * 4.0f );

		//	シーン切り換え
		if ( threadState ){
			if ( KEY_Get( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 ){

				//	pressspace波紋
				static	float	wavespeed = 1.5f;
				SetWave( load_anykey, wavespeed );
				MainFrame->ChangeScene( newScene, false );
				return;
			}
		}
	}

	//	描画
	void	sceneLoad::Render( void )
	{
		//	画面クリア
		mainView->Activate();
		mainView->Clear();

		//	背景描画
		RenderImage( bgImage[0], bgImage[0].sx, bgImage[0].sy, bgImage[0].sw, bgImage[0].sh, IMAGE_MODE::NORMAL );

		//	pressSpace描画
		if ( threadState )
		{
			RenderImage( load_anykey, 0, 0, 256, 128, IMAGE_MODE::FLASH );
			RenderImage( load_anykey, 0, 0, 256, 128, IMAGE_MODE::WAVE );
		}
	}

//----------------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------------
	
	//	スレッド
	void	sceneLoad::Thread( void* arg )
	{
		//	並列させる処理
		Scene* scene = ( Scene* )arg;
		scene->Initialize();

		//	スレッド終了
		threadState = true;
		_endthread();
	}