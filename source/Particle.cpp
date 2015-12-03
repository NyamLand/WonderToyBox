
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
			SPARK,
			STAR,
			SMOKE,
			ARROW_UP,
			ARROW_DOWN,
			DUST,
			SUCK,
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
		pt->Initialize( "DATA/Effect/particle.png", 10000 );

		timer = 0;

		return	true;
	}

	//	解放
	void	Particle::Release( void )
	{
		SafeDelete( pt );
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
			pt->Set( SPARK, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.2f, 1.0f, 0.3f, scale, RS_COPY );
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

			Move.x = Random::GetInt(-100, 100) *(0.002f * scale);
			Move.y = 1.0f * scale;
			Move.z = Random::GetInt(-100, 100) * (0.002f * scale);

			Power.x = 0.0f;
			Power.y = Random::GetInt(-100, 0) * ( scale * 0.001f );
			Power.z = 0.0f;

			//					画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( STAR, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale, RS_COPY );
		}
	}

	//	星
	void	Particle::Hit( const Vector3& pos, const float& scale,const int& time )
	{
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
			pt->Set( STAR, 0, 1.0f, 30, 0.0f, 15, 0.5f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale, RS_COPY );
		}
	}

	//	煙
	void	Particle::Smoke(const Vector3& pos, const float& scale, const int& time)
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j<1; j++)
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = -0.1f * scale;
			Move.y = 0.08f * scale;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;

			//					画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set(SMOKE, 0, 1.0f, 30, 1.0f, 15, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}


	//		矢印アップ
	void	Particle::Arrow_UP(const Vector3& pos, const float& scale , const int move, const int& time)
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j<1; j++)
		{
			Pos.x = pos.x + (Random::GetInt(-move, move)	 * (0.01f * scale));
			Pos.y = pos.y;
			Pos.z = pos.z + (Random::GetInt(-move, move)	 * (0.01f * scale));

			Move.x = 0.0f;
			Move.y = Random::GetInt(50, 100)	 * (0.001f * scale);
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;


			//					画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set(ARROW_UP, 0, 1.0f, 45, 0.0f, 20, 0.5f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}

	//	矢印ダウン
	void	Particle::Arrow_DOWN(const Vector3& pos, const float& scale, const int move, const int& time)
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j < 1; j++)
		{
			Pos.x = pos.x + (Random::GetInt(-move, move)	 * (0.01f * scale));
			Pos.y = pos.y + 3.0f * scale;
			Pos.z = pos.z + (Random::GetInt(-move, move)	 * (0.01f * scale));

			Move.x = 0.0f;
			Move.y = Random::GetInt(-100, -50)	 * (0.0007f * scale);
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;


			//					画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set(ARROW_DOWN, 0, 1.0f, 60, 0.0f, 40, 0.5f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}

	//	吸い込み
	void	Particle::Suck(const Vector3& pos, const Vector3& target, const Vector3& side, const float& length, const float& scale, const int& time)
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j < 1; j++)
		{
			Vector3 v1, center, ran_pos;
			float	t_len;

			//	ターゲットとポジションの中心点をとる
			v1 = pos - target;
			t_len = v1.Length();
			v1.Normalize();
			center = target + v1 * (t_len / 2);

			//	ランダムで法線用ポジションをとる
			ran_pos = center + side * (Random::GetFloat(-(t_len / 2) , t_len / 2));

			//	法線を使って最終的発生位置を決める
			v1 = ran_pos - pos;
			v1.Normalize();
			Pos =  pos + v1 * t_len;

			//	向きを逆に
			Move = -v1 * scale;

			Power = Move * (Random::GetFloat(0.0001f, 0.001f));

			//		画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set(SUCK, 0, 1.0f, 2 * (int)length, 1.0f, 0, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}

	//	砂埃
	void	Particle::Dust(const Vector3& pos, const Vector3& back, const Vector3& side, const float& scale, const int& time)
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j < 1; j++)
		{
			Pos = pos;


			Vector3 ideal_pos,ran_pos;
			
			//	背後の理想ポジション
			ideal_pos = pos + back * 1.0f;

			//	ランダムで法線用ポジションをとる
			ran_pos = ideal_pos + side * (Random::GetFloat(-1.0f, 1.0f));
			
			//	法線をセット
			Move = ran_pos - pos;
			Move.Normalize();

			Move *= (Random::GetFloat(0.05f, 0.2f) * scale);


			Power = Move * (Random::GetFloat(0.00001f, 0.0001f)) + Vector3(0.0f, 1.0f, 0.0f) * (Random::GetFloat(0.0001f, 0.001f));


			////	法線を使って最終的発生位置を決める
			//v1 = ran_pos - pos;
			//v1.Normalize();
			//Pos = pos + v1 * t_len;

			////	向きを逆に
			//Move = -v1;

			//Power = Move * (Random::GetFloat(0.0001f, 0.001f));


			//		画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set(DUST, 0, 0.0f, 60, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
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