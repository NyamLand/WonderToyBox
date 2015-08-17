
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
	}

	//	デストラクタ
	sceneLoad::~sceneLoad( void )
	{

	}

	//	初期化
	bool	sceneLoad::Initialize( void )
	{
		//    環境設定
		iexLight::SetAmbient( 0x808080 );
		iexLight::SetFog( 800, 1000, 0 );

		//	カメラ設定
		m_Camera = new Camera();

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
			//	シーン切り換え
			if ( threadState ){
				MainFrame->ChangeScene( newScene, false );
				return;
			}
		}

	//	描画
	void	sceneLoad::Render( void )
	{
		//	画面クリア
		m_Camera->Activate();
		m_Camera->Clear( 0xFF55FFFF );
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