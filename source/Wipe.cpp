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
	PlayerWipe::PlayerWipe( void )
	{
		WIPE_LEFT	= ( float )SPACE;
		WIPE_RIGHT	= ( float )iexSystem::ScreenWidth - WIPE_WIDTH - SPACE;
		WIPE_UP		= ( float )SPACE;
		WIPE_DOWN	= ( float )iexSystem::ScreenHeight - WIPE_HEIGHT - SPACE;
		LEN_MAX		= 0.0f;
	}
	
	//	初期化
	bool	PlayerWipe::Initialize( void )
	{
		//	レンダーターゲット用
		FOR(0, PLAYER_MAX)
		{
			//	カメラ初期化
			playerView[value]	= make_unique<Camera>();
			playerView[value]->SetPos(Vector3(0.0f, 20.0f, -10.0f));

			//	変数初期化
			wipe[value]			= make_unique<iex2DObj>( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET );
			color[value]		= Vector3( 0.0f, 1.0f, 0.0f );
			pos[value]			= Vector3( 0.0f, 0.0f, 0.0f );
			arrow_pos[value]	= Vector3( 0.0f, 0.0f, 0.0f );
			arrow_angle[value]	= 0.0f;
		}

		//	画像セット
		arrow.obj = new iex2DObj("DATA/Effect/Particle.png");
		ImageInitialize(arrow, 0, 0, SPACE, SPACE, 128 * 3, 0, 128, 128);

		//	ワイプ情報セット
		Set();

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
		
	}

	//	ワイプポジション、カラーを設定
	void	PlayerWipe::Set( void )
	{
		//	P1
		pos[0].x = WIPE_LEFT;
		pos[0].y = WIPE_UP;
		color[0] = Vector3(1.0f, 0.0f, 0.0f);

		//	P2
		pos[1].x = WIPE_RIGHT;
		pos[1].y = WIPE_UP;
		color[1] = Vector3(0.0f, 0.0f, 1.0f);


		//	P3
		pos[2].x = WIPE_LEFT;
		pos[2].y = WIPE_DOWN;
		color[2] = Vector3(1.0f, 1.0f, 0.0f);
	
		//	P4
		pos[3].x = WIPE_RIGHT;
		pos[3].y = WIPE_DOWN;
		color[3] = Vector3(0.0f, 1.0f, 0.0f);
		
	}

	//	ワイプ描画判定
	void	PlayerWipe::Check( int num )
	{
		FOR(0, PLAYER_MAX)
		{
			//	視野角内にそのプレイヤーがいるか
			Vector3	stringPos;
			
			//	画面外判定
			if (!WorldToClient(characterManager->GetPos(num), stringPos, matView* matProjection))
			{
				check[num] = true;
			}

			else
			{
				check[num] = false;
			}
		}
	}

	//	プレイヤーの方向へ矢印
	void	PlayerWipe::Arrow( int num, Vector3 target )
	{
		Vector3	stringPos;
		WorldToClient(LengthChecker(num, target), stringPos, matView* matProjection);
	
		//	ワイプ中心
		Vector3 center;
		center.x = pos[num].x + WIPE_WIDTH / 2;
		center.y = pos[num].y + WIPE_HEIGHT / 2;
		
		//	ワイプからプレイヤーへの法線（クライアントポジション）
		Vector3 normal = stringPos - center;
		float angle = atan2f(normal.y, normal.x);

		//	ワイプ中心から矢印中心までの半径
		float r = (( float )arrow.w / 2) + (WIPE_WIDTH / 2);

		//	
		arrow_pos[num].x = center.x + (cosf(angle) * r);
		arrow_pos[num].y = center.y + (sinf(angle) * r);


		//	ワイプからプレイヤーへの角度
		arrow_angle[num] = angle;


	}

	//	ワイプの移動
	void	PlayerWipe::Move( int num, Vector3 target )
	{
		Vector3	stringPos;
		WorldToClient(LengthChecker(num,target), stringPos, matView* matProjection);

		//Vector3 targetPos;
		//WorldToClient(target, targetPos, matView* matProjection);


		//	中心から左
		if (stringPos.x < iexSystem::ScreenWidth / 2)
		{
			pos[num].x = WIPE_LEFT;
		}
		//	中心から右
		else
		{
			pos[num].x = WIPE_RIGHT;
		}

		//	高さを計算
		Vector3 v1, v2;
		Vector3 start = Vector3(pos[num].x, WIPE_UP, 0);
		Vector3 end = start;
		end.y = WIPE_DOWN;
		v1 = stringPos - start;
		v2 = end  - start;
		v2.Normalize();

		float dot = Vector3Dot(v1, v2);
		if (dot < 0) dot = -dot;

		pos[num] = start + (v2 * dot);

		if (pos[num].y > WIPE_DOWN)	pos[num].y = WIPE_DOWN;
		if (pos[num].y < WIPE_UP)	pos[num].y = WIPE_UP;

	}

	//	距離の修正判定
	Vector3	PlayerWipe::LengthChecker( int num, Vector3 target )
	{
		//	カメラとターゲットの射影長を取る
		Vector3 v1;
		v1 = Vector3(mainView->GetSpringPos().x, 0, mainView->GetSpringPos().z) - target;
		LEN_MAX = v1.Length();

		//	プレイヤーとターゲットの距離を取る
		v1 = characterManager->GetPos(num) - target;
		
		if (num == 3) printf("%f,%f,%f\n", characterManager->GetPos(num).x, characterManager->GetPos(num).y, characterManager->GetPos(num).z);

		Vector3 test;
		//	距離が離れすぎていないか判定
		if (LEN_MAX < v1.Length())
		{
			v1.Normalize();
			if (num == 3)
			{
				num = num;
				test = characterManager->GetPos(num);
				int s = 0;
			}
			return target + v1 * LEN_MAX;
		}

		return characterManager->GetPos(num);
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
				wipe[value]->Render(( int )pos[value].x, ( int )pos[value].y, w, h, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shader2D, "WipeEffect");
				arrow.angle = arrow_angle[value] + PI /2;		arrow.p = ::GetPoint(( int )arrow_pos[value].x, ( int )arrow_pos[value].y);
				RenderImage(arrow, arrow.sx, arrow.sy, arrow.sw, arrow.sh, IMAGE_MODE::ADOPTPARAM, ( int )arrow_pos[value].x, ( int )arrow_pos[value].y);
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

