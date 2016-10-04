
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
		for ( int j = 0; j<Par2; j++ )
		{
			Pos.x = pos.x + ( Random::GetFloat( -1000.0f, 0.0f )	 * ( 0.001f * scale ) );
			Pos.y = pos.y + ( Random::GetFloat( -500.0f, 0.0f )	 * ( 0.001f * scale ) );
			Pos.z = pos.z + ( Random::GetFloat( -1000.0f, 0.0f )		* ( 0.001f * scale ) );

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = Random::GetFloat( 0.0f,100.0f ) * ( scale * 0.00015f );
			Power.z = 0.0f;

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( SPARK, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.2f, 1.0f, 0.3f, scale, RS_COPY );
		}
	}

	//	火花
	void	Particle::Spark( const Vector3& pos, const float& scale )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<Par3; j++ )
		{
			Pos.x = pos.x + sinf( Random::GetFloat( 0.0f, 2.0f * D3DX_PI ) ) * 2.0f * scale;
			Pos.y = pos.y;
			Pos.z = pos.z + cosf( Random::GetFloat( 0.0f, 2.0f * D3DX_PI ) ) * 2.0f * scale;

			Move.x = Random::GetFloat( -100.0f, 100.0f ) *( 0.002f * scale );
			Move.y = 1.0f * scale;
			Move.z = Random::GetFloat( -100.0f, 100.0f ) * ( 0.002f * scale );

			Power.x = 0.0f;
			Power.y = Random::GetFloat( -100.0f, 0.0f ) * ( scale * 0.001f );
			Power.z = 0.0f;

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( STAR, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale * 2.0f, RS_COPY );
		}
	}

	//	星
	void	Particle::Hit( const Vector3& pos, const float& scale,const int& time )
	{
		Vector3	Pos, Move, Power;
		timer++;
		if ( timer % time != 0 ) return;

		for ( int j = 0; j<Par3; j++ )
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = Random::GetFloat( -50.0f, 50.0f ) * ( 0.005f * scale );
			Move.y = 0.8f * scale + ( 0.2f * scale * ( j % 2 ) );
			Move.z = Random::GetFloat( -20.0f, 20.0f ) * ( 0.02f * scale );

			Power.x = 0.0f;
			Power.y = -( scale * 0.05f );
			Power.z = 0.0f;

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( STAR, 0, 1.0f, 30, 0.0f, 15, 0.5f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale, RS_COPY );
		}
	}

	//	煙
	void	Particle::Smoke( const Vector3& pos, const float& scale, const int& time )
	{
		Vector3	Pos, Move, Power;
		timer++;
		if ( timer % time != 0 ) return;
		for ( int j = 0; j<Par1; j++ )
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

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( SMOKE, 0, 1.0f, 30, 1.0f, 15, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY );
		}
	}


	//		パワーアップ
	void	Particle::PowerUp( const Vector3& pos, const float& scale , const int& move, const int& time )
	{
		Vector3	Pos, Move, Power;
		timer++;
		if ( timer % time != 0 ) return;
		for ( int j = 0; j<Par1; j++ )
		{
			Pos.x = pos.x + ( Random::GetInt( -move, move )	 * ( 0.01f * scale ) );
			Pos.y = pos.y;
			Pos.z = pos.z + ( Random::GetInt( -move, move )	 * ( 0.01f * scale ) );

			Move.x = 0.0f;
			Move.y = Random::GetFloat(50.0f, 100.0f)	 * ( 0.001f * scale );
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;


			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( POWER_UP, 0, 1.0f, 45, 0.0f, 20, 0.5f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY );
		}
	}

	//	スピードアップ
	void	Particle::SpeedUp( const Vector3& pos, const float& scale, const int& move, const int& time )
	{
		Vector3	Pos, Move, Power;
		timer++;
		if ( timer % time != 0 ) return;
		for ( int j = 0; j<Par1; j++ )
		{
			Pos.x = pos.x + (Random::GetInt( -move, move )	 * ( 0.01f * scale ) );
			Pos.y = pos.y;
			Pos.z = pos.z + (Random::GetInt( -move, move )	 * ( 0.01f * scale ) );

			Move.x = 0.0f;
			Move.y = Random::GetFloat( 50.0f, 100.0f )	 * ( 0.001f * scale );
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;


			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( SPEED_UP, 0, 1.0f, 45, 0.0f, 20, 0.5f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY );
		}
	}

	//	吸い込み
	void	Particle::Suck( const Vector3& pos, const Vector3& target, const Vector3& side, const float& length, const float& scale, const int& time )
	{
		Vector3	Pos, Move, Power;
		int speed = 5;
		timer++;
		if ( timer % time != 0 ) return;
		for ( int j = 0; j < Par1; j++ )
		{
			Vector3 v1, center, ran_pos;
			float	t_len;

			//	ターゲットとポジションの中心点をとる
			v1 = pos - target;
			t_len = v1.Length();
			v1.Normalize();
			center = target + v1 * ( t_len / 2 );

			//	ランダムで法線用ポジションをとる
			ran_pos = center + side * ( Random::GetFloat( -( t_len / 2 ) , t_len / 2 ) );

			//	法線を使って最終的発生位置を決める
			v1 = ran_pos - pos;
			v1.Normalize();
			Pos =  pos + v1 * t_len;

			//	向きを逆に
			//Move = -v1 * scale;
			Move = -v1 * length / ( speed * length );

			Power = Move * ( Random::GetFloat( 0.0001f, 0.001f ) );

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( SUCK, 0, 1.0f, speed * ( int )length, 1.0f, 0, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY );
		}
	}

	//	砂埃
	void	Particle::Dust( const Vector3& pos, const Vector3& back, const Vector3& side, const float& scale, const int& time )
	{
		Vector3	Pos, Move, Power;
		timer++;
		if ( timer % time != 0 ) return;
		for ( int j = 0; j < Par1; j++ )
		{
			Pos = pos;


			Vector3 ideal_pos,ran_pos;
			
			//	背後の理想ポジション
			ideal_pos = pos + back * 1.0f;

			//	ランダムで法線用ポジションをとる
			ran_pos = ideal_pos + side * ( Random::GetFloat( -1.0f, 1.0f ) );
			
			//	法線をセット
			Move = ran_pos - pos;
			Move.Normalize();

			Move *= ( Random::GetFloat( 0.05f, 0.2f ) * scale );


			Power = Move * ( Random::GetFloat( 0.00001f, 0.0001f ) ) + Vector3( 0.0f, 1.0f, 0.0f ) * ( Random::GetFloat( 0.0001f, 0.001f ) );


			////	法線を使って最終的発生位置を決める
			//v1 = ran_pos - pos;
			//v1.Normalize();
			//Pos = pos + v1 * t_len;

			////	向きを逆に
			//Move = -v1;

			//Power = Move * (Random::GetFloat(0.0001f, 0.001f));


			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( DUST, 0, 0.0f, 60, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY );
		}
	}

	//	爆発
	void	Particle::Bom( const Vector3& pos, const float& move, const float& scale, const int& time )
	{
		Vector3	Pos, Move, Power;
		timer++;
		if ( timer % time != 0 ) return;

		for ( int j = 0; j<Par3; j++ )
		{
			Pos.x = pos.x + ( Random::GetFloat( -move, move ) * ( 0.2f * scale ) );
			Pos.y = pos.y + ( Random::GetFloat( 0.0f, move ) * ( 0.2f * scale ) );
			Pos.z = pos.z + ( Random::GetFloat( -move, move ) * ( 0.2f * scale ) );

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( DUST, 0, 1.0f, 60, 0.0f, 40, 0.5f, &Pos, &Move, &Power, 0.8f, 0.0f, 0.0f, scale, RS_COPY );

			//	炎の灰をイメージ
			if ( j <= Par3 - 2 ) continue;

			Pos.x = pos.x + ( Random::GetFloat( -move, move ) * (0.2f * scale ) );
			Pos.y = pos.y + ( Random::GetFloat( 0.0f, move ) * (0.2f * scale ) );
			Pos.z = pos.z + ( Random::GetFloat( -move, move ) * (0.2f * scale ) );

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			IEX_SetParticle( DUST, 0, 1.0f, 60, 0.0f, 40, 0.5f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.8f, scale, RS_SUB );
		}
	}

	//	花
	void	Particle::Flower( const Vector3& pos, const float& scale, const Vector3& color)
	{
		Vector3	Pos, Move, Power;

		for ( int i = 0; i < Par3; i++ )
		{
			Pos.x = pos.x + sinf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.007f * scale;
			Pos.y = pos.y + ( Random::GetInt( 0, 200 ) - 100 ) * 0.007f * scale;
			Pos.z = pos.z + cosf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.007f * scale;

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power = Pos - pos;
			Power.Normalize();
			Power *= 0.001f * scale;

			pt->Set( FLOWER, 0, 0.0f, 30, 0.5f, 20, 1.0f, &Pos, &Move, &Power, color.x, color.y, color.z, scale * 0.2f, RS_COPY );
		}
	}

	//	花ばらまき
	void	Particle::FlowerDisseminate( const Vector3& pos, const float& scale, const float& speed, const Vector3& color )
	{
		Vector3	Pos, Move, Power;

		for ( int i = 0; i < Par3; i++ )
		{
			Pos.x = pos.x + sinf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.007f * scale;
			Pos.y = pos.y;
			Pos.z = pos.z + cosf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.007f * scale;

			Move.x = 0.0f;
			Move.y = -0.1f;
			Move.z = 0.0f;

			Power = Pos - pos;
			Power.y = 2.0f;
			Power.Normalize();
			Power *= ( 0.0005f * speed ) * scale;

			pt->Set( FLOWER, 0, 0.0f, 30, 0.5f, 30, 1.0f, &Pos, &Move, &Power, color.x, color.y, color.z, scale * 0.1f, RS_COPY );
		}
	}

	//	大砲用エフェクト
	void	Particle::Bomb( const Vector3& pos, const float& scale, const Vector3& color )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<Par3; j++ )
		{
			Pos.x = pos.x + sinf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 100 ) - 50 ) * 0.001f * scale;
			Pos.y = pos.y + sinf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 100 ) - 50 ) * 0.001f * scale;
			Pos.z = pos.z + cosf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 100 ) - 50 ) * 0.001f * scale;

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Vector3	vec = Pos - pos;
			vec.Normalize();
			Power = vec* 0.01f;

			pt->Set( DUST, 0, 1.0f, 30, 1.0f, 60, 0.1f, &Pos, &Move, &Power, color.x, color.y, color.z, scale * 15, RS_COPY);
		}
	}

	//	コイン枚数用エフェクト
	void	Particle::CoinGet( const Vector3& pos, const float& scale )
	{
		Vector3	Pos, Move, Power;

		for ( int j = 0; j<Par3; j++ )
		{
			Pos = pos;

			Move.x = Random::GetFloat( -50.0f, 50.0f ) * ( 0.005f * scale );
			Move.y = 0.8f * scale + ( 0.2f * scale * ( j % 2 ) );
			Move.z = Random::GetFloat( -20.0f, 20.0f ) * ( 0.02f * scale );

			Power.x = 0.0f;
			Power.y = -( scale * 0.05f );
			Power.z = 0.0f;

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( COIN, 0, 1.0f, 30, 1.0f, 15, 11.0f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale, RS_COPY );
		}
	}

	//	コイン獲得用エフェクト
	void	Particle::CoinUp( const Vector3& pos, const float& scale )
	{
		Vector3	Pos, Move, Power;

		for ( int j = 0; j<Par1; j++ )
		{
			Pos = pos + Vector3( 0.0f, 5.0f, 0.0f );

			Move.x = 0;
			Move.y = 0.8f * scale;
			Move.z = 0;

			Power.x = 0.0f;
			Power.y = -( scale * 0.05f );
			Power.z = 0.0f;

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( COIN, 0, 1.0f, 30, 1.0f, 15, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 0.0f, scale, RS_COPY );
		}
	}

	//	引き寄せエフェクト
	void	Particle::Magnet( const Vector3& pos, const float& length, const float& scale, const int& speed )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j < Par1; j++ )
		{
			Vector3 ran_pos, direction;

			direction.x = Random::GetFloat( -1.0f, 1.0f );
			direction.y = Random::GetFloat( 0.0f, 1.0f );
			direction.z = Random::GetFloat( -1.0f, 1.0f );

			direction.Normalize();
			
			Pos = pos + direction * length;

			Move = -direction * length / ( speed * length );

			Power = Vector3( 0.0f, 0.0f, 0.0f );
			//Power = Move * (Random::GetFloat(0.0001f, 0.001f));

			//	

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( SUCK, 0, 1.0f, speed * ( int )length, 0.0f, speed / 3, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0, 0.2f, scale, RS_COPY );

		}
	}

	//	死亡エフェクト
	void	Particle::Death( const Vector3& pos, const float& scale, const Vector3& color )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<Par2; j++ ){
			Pos.x = pos.x + sinf( Random::GetFloat( 0.0f, D3DX_PI * 2 ) ) * Random::GetInt( -50, 50 ) * 0.01f * scale;
			Pos.y = pos.y + ( Random::GetInt( 0, 10 ) - 5 ) * 0.1f * scale;
			Pos.z = pos.z + cosf( Random::GetFloat( 0.0f, D3DX_PI * 2 ) ) * Random::GetInt( -50, 50 ) * 0.01f * scale;

			Move = Pos - pos;
			Move.Normalize();
			Move *= 1.0f * scale;
			Move.y = -0.15f * scale;

			Power = pos - Pos;
			Power.Normalize();
			Power *= 0.01f * scale;
			Power.y = 0.05f * scale;

			pt->Set( SUCK, 0, 1.0f, 40, 1.0f, 15, 0.5f, &Pos, &Move, &Power, color.x, color.y, color.z, scale * 10.0f, RS_COPY );
		}

	}

	//	火球
	void	Particle::FireBall( const Vector3& pos, const float& scale, const Vector3& color )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<Par3; j++ )
		{
			Pos.x = pos.x + sinf( Random::GetFloat( 0.0f, D3DX_PI * 2 ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.01f * scale;
			Pos.y = pos.y + ( Random::GetInt( 0, 200 ) - 100 ) * 0.01f * scale;
			Pos.z = pos.z + cosf( Random::GetFloat( 0.0f, D3DX_PI * 2 ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.01f * scale;

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power = Pos - pos;
			Power.Normalize();
			Power *= 0.01f * scale;

			pt->Set( SPARK, 0, 0.3f, 30, 0.5f, 60, 0.1f, &Pos, &Move, &Power, color.x, color.y, color.z, scale * 10.0f, RS_COPY );
		}
	}

	//	火球爆発
	void	Particle::BombFireBall( const Vector3& pos, const float& scale, const Vector3& color )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<Par4; j++ ){
			Pos.x = pos.x + sinf( Random::GetFloat(0.0f, D3DX_PI * 2 ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.01f * scale;
			Pos.y = pos.y + ( Random::GetInt(0, 200) - 100) * 0.01f * scale;
			Pos.z = pos.z + cosf( Random::GetFloat(0.0f, D3DX_PI * 2 ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.01f * scale;

			Move = Pos - pos;
			Move.Normalize();
			Move *= 2.5f * scale;
			Move.y = -5.0f * scale;

			Power = pos - Pos;
			Power.Normalize();
			Power *= 0.15f * scale;
			Power.y = 0.5f * scale;

			pt->Set( SPARK, 0, 0.3f, 30, 1.0f, 60, 1.0f, &Pos, &Move, &Power, color.x, color.y, color.z, scale * 20.0f, RS_COPY );
		}
	}
	
		//	突進時の風を切る
	void	Particle::Semicircle( const Vector3& pos, const Vector3& front, const Vector3& right, const Vector3& up, const float& scale )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j <Par3; j++ )
		{
			Vector3 direction;

			//	前方半円
			direction = front	* ( Random::GetFloat( 0.0f, 1.0f ) );			//	前
			direction += right	* ( Random::GetFloat( -1.0f, 1.0f ) );		//	左右
			direction += up		* ( Random::GetFloat( -1.0f, 1.0f ) );			//	上下

			direction.Normalize();

			//	半径10離れた位置にセット
			Pos = ( pos + Vector3(0.0f, 3.0f, 0.0f ) ) + direction * 5.0f;

			//	後ろ向き
			Move = -front * ( Random::GetFloat( 0.0f, 5.0f ) ) * 0.1f * scale;


			Power = Vector3( 0.0f, 0.0f, 0.0f );

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( SPARK, 0, 0.0f, 40, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 0.0f, scale, RS_COPY );
		}
	}

	//	大砲打った時の煙
	void	Particle::CannonSmoke( const Vector3& pos, const Vector3& front, const Vector3& right, const Vector3& up, const float& scale )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j < Par3; j++ ){
			
			Vector3 direction;

			//	前方半円
			direction = front	* ( Random::GetFloat( 0.0f, 1.0f ) );			//	前
			direction += right	* ( Random::GetFloat( -1.0f, 1.0f ) );		//	左右
			direction += up		* ( Random::GetFloat( -1.0f, 1.0f ) );			//	上下
			direction.Normalize();

			Pos = pos + direction * 1.0f;

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;

			//	画像タイプ、出現フレーム、出現時透明度、最終フレーム、最終透明度、最高フレーム、最高透明度、出現位置、移動値、与力、	赤成分、緑成分、青成分、スケール、レンダーステート
			pt->Set( SUCK, 0, 0.0f, 50, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.5f, 0.5f, 0.5f, scale, RS_COPY );
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