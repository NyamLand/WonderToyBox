
#include	"iextreme.h"
#include	"Particle.h"

//*******************************************************************************
//
//	Paticleクラス
//
//*******************************************************************************

//------------------------------------------------------------------------
//	グローバル
//------------------------------------------------------------------------

iexParticle*	Particle::particle = NULL;

//------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------

	//	コンストラクタ
	Particle::Particle( void )
	{

	}

	//	デストラクタ
	Particle::~Particle( void )
	{
		Release();
	}

	//	初期化
	bool	Particle::Initialize( void )
	{
		particle = new iexParticle();
		particle->Initialize( "DATA/particle.png", 10000 );

		return	true;
	}

	//	解放
	void	Particle::Release( void )
	{
		particle->Release();
	}

//------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------

	//	更新
	void	Particle::Update( void )
	{
		particle->Update();
	}

	//	描画
	void	Particle::Render( void )
	{
		particle->Render();
	}

//------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------

	//	青炎
	void	Particle::BlueFlame( const Vector3& pos, const float& scale )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<5; j++ )
		{
			Pos.x = pos.x + ( ( rand() % 1000 - 1000 ) * ( 0.001f * scale ) );
			Pos.y = pos.y + ( ( rand() % 500 - 500 ) * ( 0.001f * scale ) );
			Pos.z = pos.z + ( ( rand() % 1000 - 1000 ) * ( 0.001f * scale ) );

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = (rand() % 100)* (scale * 0.00015f);
			Power.z = 0.0f;

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			particle->Set( 6, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.2f, 1.0f, 0.3f, scale, RS_COPY );
		}
	}

	//	火花
	void	Particle::Spark( const Vector3& pos, const float& scale )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<10; j++ )
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = ( rand() % 200 - 100 ) * ( 0.002f * scale );
			Move.y = 1.0f * scale;
			Move.z = ( rand() % 200 - 100 ) * ( 0.002f * scale );

			Power.x = 0.0f;
			Power.y = -( rand() % 100 ) * ( scale * 0.001f );
			Power.z = 0.0f;

			//					画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			particle->Set( 1, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.5f, 0.6f, 1.0f, scale, RS_COPY );
		}
	}