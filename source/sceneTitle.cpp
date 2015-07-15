
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"system/Scene.h"
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
	Vector3 TARGET[4]=
	{
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 20.0f, 0.0f, 10.0f ),
		Vector3( 5.0f, 0.0f, -10.0f ),
		Vector3( -20.0f, 0.0f, 10.0f ),
	};

}
//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
sceneTitle::sceneTitle(void) : orientation(0, 0, 0, 1)
	{

	}
	
	//	デストラクタ
	sceneTitle::~sceneTitle( void )
	{
		//SafeDelete( view );
		SafeDelete( m_Stage );
		SafeDelete( m_Camera );
		SafeDelete( m_Player );
		SafeDelete( m_CollisionStage );
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
		m_Stage = new iexMesh("DATA/BG/2_1/FIELD2_1.IMO");

		//	当たり判定
		Collision::Initiallize(m_CollisionStage);

		//	プレイヤー
		m_Player = new PlayerManager();
		m_Player->Initialize( 0, 0, c_Move::TARGET[0] );
		m_Player->Initialize( 1, 0, c_Move::TARGET[1] );
		m_Player->Initialize( 2, 0, c_Move::TARGET[2] );
		m_Player->Initialize( 3, 0, c_Move::TARGET[3] );
		testpos = 0;
		t = 0;
		s_pos = m_Camera->GetPos();
		c_pos = m_Camera->GetPos();
		t_pos = Vector3( 0, 0, 0 );

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

		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneSelect() );
			return;
		}

		//	移動先を変える
		if ( KEY( KEY_C ) == 3  && t > 0.7f)
		{
			if ( testpos < 3 )	testpos++;
			else testpos = 0;
			t = 0;
		}
		if ( KEY( KEY_D ) == 3	&& t > 0.7f)
		{
			if ( testpos > 0 )	testpos--;
			else testpos = 3;
			t = 0;

		}

		//	クォータニオン＆ラープの計算
		Matrix m;
		orientation.toMatrix(m);
		Vector3 forward = Vector3(m._31, m._32, m._33);

		Vector3 d;
		Vector3 axis;
		FLOAT	angle;
		
		d = c_Move::TARGET[testpos] - c_pos;
		d.Normalize();

		forward.Normalize();
		Vector3Cross(axis, forward, d);
		angle = Vector3Dot(d, forward);
		
		
		if (t <= 0.7f){
			Lerp(c_pos, s_pos, c_Move::TARGET[testpos], t - 0.1f);
			angle = acos(angle);
			if (fabs(angle) > 1e-8f)
			{

				Quaternion q;
				//変換用
				D3DXQUATERNION q2;
				D3DXVECTOR3 axis2;
				//	IEX→D3DX
				QuaternionItoD(&q2, &q);
				Vector3ItoD(&axis2, &axis);

				D3DXQuaternionRotationAxis(&q2, &axis2, angle);

				//	D3DX→IEX
				QuaternionDtoI(&q, &q2);

				Quaternion n;
				n = orientation * q;

				orientation = QuaternionSlerp(orientation, n, 0.03);
			}
		
			t += 0.01f;
		}
		else{
			s_pos = m_Camera->GetPos();
		}
		m_Camera->Set(c_pos,t_pos);

		

	}

	//	描画
	void	sceneTitle::Render( void )
	{
		//	画面クリア
		//view->Activate();
		//view->Clear();
		m_Camera->Activate();
		m_Camera->Clear();

		//	オブジェクト描画
		m_Stage->Render( shader3D, "full_s" );
		m_Player->Render( shader3D, "full" );

		//	デバッグ用
		DrawString( "[sceneTitle]", 50, 50 );
		DrawString( "すぺーす押してね", 300, 400, 0xFFFFFF00 );
		DrawString( "はーいぷしゅっ！", 1100, 700, 0xFFFFFF00 );
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	