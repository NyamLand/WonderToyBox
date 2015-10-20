
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
	//デバック用
	int	Xs = 400;
	int Ys = 200;
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
		SafeDelete( load );
		SafeDelete(load_anykey.obj);
		
	}

	//	初期化
	bool	sceneLoad::Initialize( void )
	{
		//    環境設定
		iexLight::SetAmbient( 0x808080 );
		iexLight::SetFog( 800, 1000, 0 );

		//	カメラ設定
		mainView = new Camera();
		load = new iex2DObj("DATA/Load/Lord-back.png");
		load_anykey.obj = new iex2DObj("DATA/UI/pressspace.png");
		ImageInitialize(load_anykey, 1100, 675, 256, 128, 0, 0, 256, 128);
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
		FlashingUpdate(load_anykey, D3DX_PI / 180 * 4.0f);

			//	シーン切り換え
			if ( threadState ){
				if (KEY_Get(KEY_SPACE) == 3 || KEY(KEY_A) == 3){

					//	pressspace波紋
					static	float	wavespeed = 1.5f;
					SetWave(load_anykey, wavespeed);
					MainFrame->ChangeScene(newScene, false);
					return;
				}
			}

			//デバッグ用
			if (KEY_Get(KEY_RIGHT) == 1){
				Xs += 10;
			}
			if (KEY_Get(KEY_LEFT) == 1){
				Xs -= 10;
			}
			if (KEY_Get(KEY_DOWN) == 1){
				Ys += 10;

			}

			if (KEY_Get(KEY_UP) == 1){
				Ys -= 10;

			}
		}

	//	描画
	void	sceneLoad::Render( void )
	{
		//	画面クリア
		mainView->Activate();
		mainView->Clear();
		load->Render(0, 0, 1280, 720, 0, 0, 1280, 720);
		DrawString( "ロード中", 200, 300 );

		//	pressSpace描画
		RenderImage(load_anykey, 0, 0, 256, 128, IMAGE_MODE::FLASH);
		RenderImage(load_anykey, 0, 0, 256, 128, IMAGE_MODE::WAVE);



		//デバック用
		sprintf_s(stri, "%d\n%d", Xs, Ys);
		DrawString(stri, 0, 250, 0xFFFFFFFF);
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