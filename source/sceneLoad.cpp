
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

#define	INTERVAL	300		//	差し替えの間隔
#define	INTERPOLATION_SPEED		0.01f	//	補間速度

	//	static宣言
	bool	sceneLoad::threadState;

//----------------------------------------------------------------------------------
//	初期化・解放	
//----------------------------------------------------------------------------------
	
	//	コンストラクタ
	sceneLoad::sceneLoad( Scene*	nextScene ) : newScene( nextScene ), timer( 0 )
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
		SafeDelete( nowLoading );	
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
		bgImage[2].obj = new iex2DObj( "DATA/UI/Load/Lord-back03.png" );
		bgImage[3].obj = new iex2DObj( "DATA/UI/Load/Lord-back04.png" );
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w = static_cast<int>( iexSystem::ScreenWidth );
		int h = static_cast<int>( iexSystem::ScreenHeight );
		ImageInitialize( bgImage[0], x, y, w, h, 0, 0, 1280, 720 );
		ImageInitialize( bgImage[1], x, y, w, h, 0, 0, 1280, 720 );

		//	NowLoading初期化
		nowLoading = new iex2DObj( "DATA/UI/Loading.png" );

		//	pressAnyKey初期化
		pressAnyKeyImage.obj = nowLoading;
		ImageInitialize( pressAnyKeyImage,  1380, 865, 350, 150, 0, 128, 512, 128 );
		
		//	別スレッド作成
		_beginthread( Thread, 0, ( void* )newScene );
		
		//	変数初期化
		timer = 0;
		t = 0.0f;
		reverseFlag = false;
		isEnd = false;
		changeSceneFlag = false;
		loadingTimer = 0;
		renderCount = 0;
		return	true;
	}

//----------------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------------
	
	//	更新
	void	sceneLoad::Update( void )
	{
		//	背景画像差し替え
		MoveBG();

		//	NowLoading更新
		MoveNowLoading();

		//	読み込み完了後キー入力受付
		if ( threadState ){
			if ( KEY_Get( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
			{
				changeSceneFlag = true;
			}
			FlashingUpdate( pressAnyKeyImage, 0.1f );
		}

		//	シーン切り替え
		if ( changeSceneFlag )
		{
			MainFrame->ChangeScene( newScene, false );
			return;
		}

	}

	//	描画
	void	sceneLoad::Render( void )
	{
		//	画面クリア
		mainView->Activate();
		mainView->Clear();

		//	背景描画
		if ( reverseFlag )
		{
			RenderImage( bgImage[0], bgImage[0].sx, bgImage[0].sy, bgImage[0].sw, bgImage[0].sh, IMAGE_MODE::NORMAL );
			RenderImage( bgImage[1], bgImage[1].sx, bgImage[1].sy, bgImage[1].sw, bgImage[1].sh, IMAGE_MODE::NORMAL );
		}
		else
		{
			RenderImage( bgImage[1], bgImage[1].sx, bgImage[1].sy, bgImage[1].sw, bgImage[1].sh, IMAGE_MODE::NORMAL );
			RenderImage( bgImage[0], bgImage[0].sx, bgImage[0].sy, bgImage[0].sw, bgImage[0].sh, IMAGE_MODE::NORMAL );
		}

		if ( !threadState )	RenderNowLoading();
		else						RenderImage( pressAnyKeyImage, pressAnyKeyImage.sx, pressAnyKeyImage.sy, pressAnyKeyImage.sw, pressAnyKeyImage.sh, IMAGE_MODE::FLASH );
	}

	//	NowLoading描画
	void	sceneLoad::RenderNowLoading( void )
	{
		//	時間がないので座標直打ち
		int	plusSw[4] = { 0, 25, 55, 97 };
		int	plusW[4] = { 0, 18, 40, 70 };

		nowLoading->Render( 1230, 790, 300 + plusW[renderCount], 150, 0, 0, 415 + plusSw[renderCount], 128 );
	}

//----------------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------------

	//	NowLoading動作
	void	sceneLoad::MoveNowLoading( void )
	{
		loadingTimer++;

		if ( loadingTimer % 30 == 0 )
		{
			renderCount++;
			if ( renderCount > 3 )	renderCount = 1;
		}
	}

	//	背景移動
	void	sceneLoad::MoveBG( void )
	{
		static	int	centerX = static_cast<int>( iexSystem::ScreenWidth * 0.5f );

		//	タイマー加算
		timer++;

		//	一定時間たったら差し替えスタート
		if ( timer >= INTERVAL )
		{
			//	裏側の画像を元の位置に配置
			bgImage[!reverseFlag].x = centerX;

			//	補間パラメータ加算
			t += INTERPOLATION_SPEED;
			if ( t >= 1.0f )		t = 1.0f;

			//	補間
			isEnd = CubicFunctionInterpolation( bgImage[reverseFlag].x, centerX, -centerX, t );

			//	補間終了後リセット
			if ( isEnd )
			{
				timer = 0;
				t = 0.0f;
				reverseFlag = !reverseFlag;
			}
		}
	}
	
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