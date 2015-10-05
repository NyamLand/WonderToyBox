
#include	"iextreme.h"
#include	"system\System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"

#include	"Effect.h"

//*******************************************************************************
//
//	Effectクラス
//
//*******************************************************************************

	//　実体宣言
	Effect*		m_Effect = nullptr;
	namespace{
		namespace{
			const float POS[3] =
			{
				3.0f,		//	最大
				-3.0f,		//	最少
				0.5f,		//	高さ
			};

		}
	}

//-------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------

	//	コンストラクタ
	Effect::Effect(void) : aura(nullptr), isAura(false), _adjustV(0)
	{
	
	}

	//	デストラクタ
	Effect::~Effect( void )
	{
		SafeDelete(aura);
	}

	//	初期化
	void	Effect::Initialize()
	{
		aura = new iexMesh("DATA\\Effect\\aura.IMO");

		circle_pic[0] = new iex2DObj("DATA/Effect/PL1.png");
		circle_pic[1] = new iex2DObj("DATA/Effect/PL2.png");
		circle_pic[2] = new iex2DObj("DATA/Effect/PL3.png");
		circle_pic[3] = new iex2DObj("DATA/Effect/PL4.png");

		for ( int i = 0; i < 4; i++ ){
			//	頂点設定
			circle[i].poligon[0].x = 0;
			circle[i].poligon[0].y = 0;
			circle[i].poligon[0].z = 0;
			circle[i].poligon[0].tu = 0;
			circle[i].poligon[0].tv = 0;
			
			circle[i].poligon[1].x = 0;
			circle[i].poligon[1].y = 0;
			circle[i].poligon[1].z = 0;
			circle[i].poligon[1].tu = 0;
			circle[i].poligon[1].tv = 0;

			circle[i].poligon[2].x = 0;
			circle[i].poligon[2].y = 0;
			circle[i].poligon[2].z = 0;
			circle[i].poligon[2].tu = 0;
			circle[i].poligon[2].tv = 0;

			circle[i].poligon[3].x = 0;
			circle[i].poligon[3].y = 0;
			circle[i].poligon[3].z = 0;
			circle[i].poligon[3].tu = 0;
			circle[i].poligon[3].tv = 0;

			circle[i].c_pos = Vector3(0, 10, 0);
		
			//	頂点設定
			circle_out[i].poligon[0].x = 0;
			circle_out[i].poligon[0].y = 0;
			circle_out[i].poligon[0].z = 0;
			circle_out[i].poligon[0].tu = 0;
			circle_out[i].poligon[0].tv = 0;

			circle_out[i].poligon[1].x = 0;
			circle_out[i].poligon[1].y = 0;
			circle_out[i].poligon[1].z = 0;
			circle_out[i].poligon[1].tu = 0;
			circle_out[i].poligon[1].tv = 0;

			circle_out[i].poligon[2].x = 0;
			circle_out[i].poligon[2].y = 0;
			circle_out[i].poligon[2].z = 0;
			circle_out[i].poligon[2].tu = 0;
			circle_out[i].poligon[2].tv = 0;

			circle_out[i].poligon[3].x = 0;
			circle_out[i].poligon[3].y = 0;
			circle_out[i].poligon[3].z = 0;
			circle_out[i].poligon[3].tu = 0;
			circle_out[i].poligon[3].tv = 0;
		
			circle_out[i].c_pos = Vector3(0, 10, 0);

			c_angle = 0.0f;
		}
	}

//-------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------

	//	更新
	void	Effect::Update( void )
	{
		if (gameManager->GetMode() == GAME_MODE::CLIMAX)	isAura = true;
		if (isAura)
		{
			Vector3 p = characterManager->GetPos(gameManager->GetWorst()) + Vector3(0, 1.5f, 0);
			aura->SetPos(p);
			aura->SetScale(0.02f);
			aura->SetAngle(0.0f);
			aura->Update();
		}
		_adjustV += 0.02f;
		shader3D->SetValue("adjustV", _adjustV);

		//	情報更新
		for (int i = 0; i < 4; i++){
			PoligonSet( &circle[i] );
			PoligonSet( &circle_out[i] );
			CirclePosSet( &circle[i], i );
			CirclePosSet( &circle_out[i], i );
		}
		//	回転
		Spin();
		c_angle += 0.1f;

	}

	//	回転
	void	Effect::Spin( void )
	{
		//	頂点回転
		for (int i = 0; i < 4; i++)
		{
			for (int n = 0; n < 4; n++)
			{
				circle_out[i].poligon[n].x = cosf(c_angle) * (circle[i].poligon[n].x - circle[i].c_pos.x) - sinf(c_angle) * (circle[i].poligon[n].z - circle[i].c_pos.z) + circle[i].c_pos.x;
				circle_out[i].poligon[n].z = sinf(c_angle) * (circle[i].poligon[n].x - circle[i].c_pos.x) + cosf(c_angle) * (circle[i].poligon[n].z - circle[i].c_pos.z) + circle[i].c_pos.z;
			}
		}
	}

	//	描画
	void	Effect::Render( void )
	{
		if(isAura)	aura->Render(shader3D,"effect_add");
		for (int i = 0; i < 4; i++){
			iexPolygon::Render3D( circle_out[i].poligon, 2, circle_pic[i], RS_COPY );
			particle->BlueFlame( Vector3( circle_out[0].poligon[i].x, circle_out[0].poligon[i].y, circle_out[0].poligon[i].z ), 0.1f );
		}
	}



//-------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------
	void	Effect::PoligonSet( Circle* c )
	{
			//	保存用

			c->poligon[0].x = POS[1]	+ c->c_pos.x;
			c->poligon[0].y = POS[2]	+ c->c_pos.y;
			c->poligon[0].z = POS[0]	+ c->c_pos.z;
			c->poligon[0].tu = 0.0f;
			c->poligon[0].tv = 0.0f;

			c->poligon[1].x = POS[0]	+ c->c_pos.x;
			c->poligon[1].y = POS[2]	+ c->c_pos.y;
			c->poligon[1].z = POS[0]	+ c->c_pos.z;
			c->poligon[1].tu = 1.0;
			c->poligon[1].tv = 0.0f;

			c->poligon[2].x = POS[1] + c->c_pos.x;
			c->poligon[2].y = POS[2] + c->c_pos.y;
			c->poligon[2].z = POS[1] + c->c_pos.z;
			c->poligon[2].tu = 0.0f;
			c->poligon[2].tv = 1.0f;

			c->poligon[3].x = POS[0] + c->c_pos.x;
			c->poligon[3].y = POS[2] + c->c_pos.y;
			c->poligon[3].z = POS[1] + c->c_pos.z;
			c->poligon[3].tu = 1.0f;
			c->poligon[3].tv = 1.0f;			
	}

	void	Effect::CirclePosSet( Circle* c, int i )
	{
			c->c_pos = characterManager->GetPos(i) + Vector3( 0.0f, 0.4f, 0.0f );
	}


	void	LVChange( LVERTEX &l,Vector3 &p )
	{
		p.x = l.x;
		p.y = l.y;
		p.z = l.z;
	}


//-------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------