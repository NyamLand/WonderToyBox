#include	"iextreme.h"
#include	"system\System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Camera.h"
#include	"CharacterManager.h"
#include	"Particle.h"

#include	"Wipe.h"


//*****************************************************************************************************************************
//
//	初期化
//
//*****************************************************************************************************************************

	//	コンストラクタ
	PlayerWipe::PlayerWipe(void) : ASPECT( 0 )
	{
	
	}
	
	//	初期化
	bool	PlayerWipe::Initialize( void )
	{
		//***********************
		//	定義
		//***********************

		//	カメラの視野角
		ASPECT = D3DX_PI / 4.0f;

		//***********************

		//	レンダーターゲット用
		FOR(0, PLAYER_MAX)
		{
			//	カメラ初期化
			playerView[value]	= make_unique<Camera>();
			playerView[value]->SetPos(Vector3(0.0f, 20.0f, -10.0f));

			//	変数初期化
			wipe[value]			= make_unique<iex2DObj>( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET );
			color[value]		= Vector3( 0.0f, 1.0f, 0.0f );
		}
		return true;
	}


	//	デストラクタ
	PlayerWipe::~PlayerWipe( void )
	{

	}


//*****************************************************************************************************************************
//
//		更新
//
//*****************************************************************************************************************************
	
	//	更新
	void	PlayerWipe::Update( void )
	{
		FOR(0, PLAYER_MAX){
			//	カメラ更新
			playerView[value]->Update(VIEW_MODE::INDIVIDUAL, characterManager->GetPos(value));
			playerView[value]->SetPos(characterManager->GetPos(value) + Vector3(0.0f, 20.0f, -10.0f));
		}
		//	ワイプ情報セット
		Set();
	}

	//	ワイプポジション、カラーを設定
	void	PlayerWipe::Set( void )
	{
		//	P1
		pos[0].x = 0;
		pos[0].y = 0;
		color[0] = Vector3(1.0f, 0.0f, 0.0f);

		//	P2
		pos[1].x = iexSystem::ScreenWidth - WIPE_WIDTH;
		pos[1].y = 0;
		color[1] = Vector3(0.0f, 0.0f, 1.0f);


		//	P3
		pos[2].x = 0;
		pos[2].y = iexSystem::ScreenHeight - WIPE_HEIGHT;
		color[2] = Vector3(1.0f, 1.0f, 0.0f);
	
		//	P4
		pos[3].x = iexSystem::ScreenWidth - WIPE_WIDTH;
		pos[3].y = iexSystem::ScreenHeight - WIPE_HEIGHT;
		color[3] = Vector3(0.0f, 1.0f, 0.0f);
		
	}

	//	ワイプ描画判定
	void	PlayerWipe::Check( float angle, int num )
	{
		FOR(0, PLAYER_MAX)
		{
			//	視野角内にそのプレイヤーがいるか
			Vector3	stringPos;
			WorldToClient(characterManager->GetPos(num), stringPos, matView* matProjection);

			if (stringPos.x < 0	||
				stringPos.x > iexSystem::ScreenWidth ||
				stringPos.y < 0 ||
				stringPos.y > iexSystem::ScreenHeight)
			{
				check[num] = true;
			}

			else
			{
				check[num] = false;
			}
		}
	}

//*****************************************************************************************************************************
//
//		描画関連
//
//*****************************************************************************************************************************

	//	描画
	void	PlayerWipe::Render( void )
	{
		FOR(0, PLAYER_MAX)
		{
			//	座標・スケール設定
			int w = WIPE_WIDTH;
			int h = WIPE_HEIGHT;
			float	wipe_size = 100.0f;
			float	frame_size = 10.0f;

			//	シェーダーへセット
			shader2D->SetValue("center_posX", pos[value].x);
			shader2D->SetValue("center_posY", pos[value].y);
			shader2D->SetValue("picture_width", w);
			shader2D->SetValue("picture_height", h);
			shader2D->SetValue("wipe_size", wipe_size);
			shader2D->SetValue("frame_size", frame_size);
			shader2D->SetValue("frame_color", color[value]);

			//	ワイプ描画
			if (check[value]){
				wipe[value]->Render(pos[value].x, pos[value].y, w, h, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shader2D, "WipeEffect");
			}
		}
	}




	//	レンダーターゲット用
	void	PlayerWipe::RenderTarget( int num )
	{
		//	レンダーターゲットを切り替え
		wipe[num]->RenderTarget(0);

		//	画面クリア
		playerView[num]->Activate();
		playerView[num]->Clear();

	}

//------------------------------------------------------------------------------------
//	情報取得
//------------------------------------------------------------------------------------
	
	//	実体取得
	PlayerWipe*		PlayerWipe::GetInstance()
	{
		static	PlayerWipe out;
		return &out;
	}

