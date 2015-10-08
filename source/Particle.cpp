
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Random.h"
#include	"Particle.h"

//*******************************************************************************
//
//	Paticleクラス
//
//*******************************************************************************

//------------------------------------------------------------------------
//	グローバル
//------------------------------------------------------------------------

namespace
{
	namespace
	{
		enum EFF_TYPE
		{
			NORMAL,
			STAR,
			SMOKE,
			ARROW,
			END,
		};
	}

	namespace
	{
		const		LPSTR	filename[] =
		{
			"DATA/particle.png",
			"DATA/Effect/star.png",
			"DATA/Effect/smoke.png",
			"DATA/Effect/arrow.png",
		};
	}
}


//------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------

	//	初期化
	bool	Particle::Initialize( void )
	{
		pt = nullptr;
		pt = new iexParticle();
		pt->Initialize( "DATA/particle.png", 10000 );

		effectImage = nullptr;
		effectImage = new iex2DObj * [END];
		for ( int i = 0; i < END; i++ )	effectImage[i] = new iex2DObj( filename[i] );
		timer = 0;

		return	true;
	}

	//	解放
	void	Particle::Release( void )
	{
		SafeDelete( pt );
		SafeDeleteArray( effectImage );
	}

//------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------

	//	更新
	void	Particle::Update( void )
	{
		pt->Update();
	}

	//	描画
	void	Particle::Render( void )
	{
		pt->Render();
	}

//------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------

	//	青炎
	void	Particle::BlueFlame( const Vector3& pos, const float& scale )
	{
		pt->SetImage( effectImage[NORMAL] );
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<5; j++ )
		{
			Pos.x = pos.x + ( Random::GetInt( -1000, 0 )	 * ( 0.001f * scale ) );
			Pos.y = pos.y + ( Random::GetInt( -500, 0 )	 * ( 0.001f * scale ) );
			Pos.z = pos.z + ( Random::GetInt( -1000, 0 )		* ( 0.001f * scale ) );

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = Random::GetInt( 0,100 ) * ( scale * 0.00015f );
			Power.z = 0.0f;

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( 6, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.2f, 1.0f, 0.3f, scale, RS_COPY );
		}
	}

	//	火花
	void	Particle::Spark( const Vector3& pos, const float& scale )
	{
		pt->SetImage( effectImage[STAR] );
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<10; j++ )
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = Random::GetInt(-100, 100) *(0.002f * scale);
			Move.y = 1.0f * scale;
			Move.z = Random::GetInt(-100, 100) * (0.002f * scale);

			Power.x = 0.0f;
			Power.y = Random::GetInt(-100, 0) * ( scale * 0.001f );
			Power.z = 0.0f;

			//					画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( 1, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale, RS_COPY );
		}
	}

	//	星
	void	Particle::Hit( const Vector3& pos,const int& time, const float& scale )
	{
		pt->SetImage( effectImage[STAR] );
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;

		for ( int j = 0; j<10; j++ )
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = Random::GetInt( -50, 50 ) * ( 0.005f * scale );
			Move.y = 0.8f * scale + (0.2f * scale * (j %2));
			Move.z = Random::GetInt( -20, 20 ) * ( 0.02f * scale );

			Power.x = 0.0f;
			Power.y = -(scale * 0.05f);
			Power.z = 0.0f;

			//					画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( 1, 0, 1.0f, 30, 0.0f, 15, 0.5f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale, RS_COPY );
		}
	}

	//	煙
	void	Particle::Smoke(const Vector3& pos,const int& time, const float& scale)
	{
		pt->SetImage(effectImage[SMOKE]);
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j<1; j++)
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = 0.1f * scale;
			Move.y = 0.08f * scale;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;

			//					画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set(0, 0, 1.0f, 30, 1.0f, 15, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}


	//		矢印アップ
	void	Particle::Arrow_UP(const Vector3& pos, const int& time, const float& scale , const int move)
	{
		pt->SetImage(effectImage[ARROW]);
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j<1; j++)
		{
			Pos.x = pos.x + (Random::GetInt(-move, move)	 * (0.01f * scale));
			Pos.y = pos.y;
			Pos.z = pos.z + (Random::GetInt(-move, move)	 * (0.01f * scale));

			Move.x = 0.0f;
			Move.y = Random::GetInt(0, 100)	 * (0.001f * scale);
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;


			//					画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set(0, 0, 0.6f, 25, 0.0f, 10, 0.3f, &Pos, &Move, &Power, 0.7f, 0.1f, 0.1f, scale, RS_COPY);
		}
	}

	//	矢印ダウン
	void	Particle::Arrow_DOWN(const Vector3& pos, const int& time, const float& scale, const int move)
	{
		pt->SetImage(effectImage[ARROW]);
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j < 1; j++)
		{
			Pos.x = pos.x + (Random::GetInt(-move, move)	 * (0.01f * scale));
			Pos.y = pos.y + 1.5f * scale;
			Pos.z = pos.z + (Random::GetInt(-move, move)	 * (0.01f * scale));

			Move.x = 0.0f;
			Move.y = Random::GetInt(-100, 0)	 * (0.001f * scale);
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;


			//					画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set(0, 0, 0.6f, 25, 0.0f, 10, 0.3f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}
//------------------------------------------------------------------------
//	情報取得
//------------------------------------------------------------------------

	//	実体取得
	Particle*	Particle::GetInstance( void )
	{
		static	Particle	out;
		return	&out;
	}