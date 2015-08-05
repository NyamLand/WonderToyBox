
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"sceneMain.h"
#include	"sceneSelect.h"

#include	"sceneTitle.h"

//*****************************************************************************
//
//	sceneTitleクラス
//
//*****************************************************************************

namespace c_Move{
	Vector3 TARGET[MAX_TARGET]=
	{
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 20.0f, 0.0f, 10.0f ),
		Vector3( 5.0f, 0.0f, -10.0f ),
		Vector3( -20.0f, 0.0f, 10.0f ),
		Vector3( 0.0f, 50.0f, 200.0f ),
	};

}

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	sceneTitle::sceneTitle( void ) : orientation( 0, 0, 0, 1 )
	{

	}
	
	//	デストラクタ
	sceneTitle::~sceneTitle( void )
	{
		SafeDelete( m_Stage );
		SafeDelete( m_Camera );
		SafeDelete( m_Player );
		SafeDelete( m_CollisionStage );
		SafeDelete( screen );
	}
	
	//	初期化
	bool	sceneTitle::Initialize( void )
	{
		//view = new iexView();

		//	環境設定
		iexLight::SetAmbient(0x404040);
		iexLight::SetFog(800, 1000, 0);

		//	ライト設定
		Vector3 dir(1.0f, -1.0f, -0.5f);
		dir.Normalize();
		iexLight::DirLight(shader3D, 0, &dir, 0.8f, 0.8f, 0.8f);

		//	カメラ設定
		m_Camera = new Camera();

		//	ステージ
		m_CollisionStage = new iexMesh("DATA/BG/CollisionGround.IMO");
		m_Stage = new iexMesh( "DATA/BG/2_1/FIELD2_1.IMO" );

		//	当たり判定
		Collision::Initiallize( m_CollisionStage );

		//	プレイヤー
		m_Player = new PlayerManager();
		m_Player->Initialize( 0, PlayerData::Y2009, c_Move::TARGET[0] );
		m_Player->Initialize( 1, PlayerData::Y2009, c_Move::TARGET[1] );
		m_Player->Initialize( 2, PlayerData::Y2009, c_Move::TARGET[2] );
		m_Player->Initialize( 3, PlayerData::Y2009, c_Move::TARGET[3] );
		testpos = 0;
		t = 0;
		s_pos = m_Camera->GetPos();
		c_pos = m_Camera->GetPos();
		t_pos = Vector3( 0, 0, 0 );

		//	シーン切り替え用ステート
		state_step = 0;
		scene_change = MAX_TARGET - 1;
		//	背景・スクリーン画像初期化
		screen = new iex2DObj("DATA/Screen.png");

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
		//	プレイヤー更新
		m_Player->Update();


		//	移動先を変える
		if ( KEY( KEY_C ) == 3  && t > 0.7f)
		{
			if ( testpos < 3 )	testpos++;
			else testpos = 0;
			t = 0;
		}
		if ( KEY( KEY_D ) == 3	&& t > 0.7f )
		{
			if ( testpos > 0 )	testpos--;
			else testpos = 3;
			t = 0;

		}		
		
		if ( t <= 0.7f ){
			Lerp( c_pos, s_pos, c_Move::TARGET[testpos], t - 0.1f );
	
		}
		else{
			s_pos = m_Camera->GetPos();
		}

		//	カメラ更新
		m_Camera->Update( VIEW_MODE::SLERP, c_Move::TARGET[testpos] );
		m_Camera->SetPos( c_pos );

		//	スペースを押してシーンを切り替える処理
		SceneState();

		//	
		if (t < 1.0f){
			t += 0.01f;
		}
	}

	//	シーンステート
	void	sceneTitle::SceneState( void )
	{
		switch (state_step)
		{
		case 0:
			if (KEY(KEY_SPACE) == 3)
			{
				state_step++;
				//MainFrame->ChangeScene(new sceneSelect());
				//return;
			}
			break;
		case 1:
			testpos = scene_change;
			t = 0;
			state_step++;
			break;

		case 2:
			if (t >= 1.0f)
			{
				MainFrame->ChangeScene(new sceneSelect());
				return;
			}
			break;

		}
	}

	//	描画
	void	sceneTitle::Render( void )
	{
		//	画面クリア
		m_Camera->Activate();
		m_Camera->Clear();

		//	オブジェクト描画
		m_Stage->Render( shader3D, "full" );
		m_Player->Render( shader3D, "full" );

		//	デバッグ用
		DrawString( "[sceneTitle]", 50, 50 );
		DrawString( "すぺーす押してね", 300, 400, 0xFFFFFF00 );
		DrawString( "はーいぷしゅっ！", 1100, 700, 0xFFFFFF00 );

		if (state_step >= 2){
			shader2D->SetValue("alpha", t);
			screen->Render(0, 0, 1280, 720, 0, 0, 512, 512, shader2D, "copy");
		}
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	