
#ifndef		__PARTICLE_H__
#define		__PARTICLE_H__

//****************************************************************************************
//
//	Particle関連
//
//****************************************************************************************
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
			POWER_UP,
			SPEED_UP,
			DUST,
			SUCK,
			FLOWER = 8,
			COIN,
		};

		enum EFF_TIMER
		{
			//SPARK_TIME,
			STAR_TIME = 5,
			SMOKE_TIME = 20,
			POWER_UP_TIME = 10,
			SPEED_UP_TIME = 11,
			DUST_TIME = 1,
			SUCK_TIME = 60,
			FLOWER_TIME = 8,
			BOM_TIME = 6,
 			COIN_TIME,
		};

		//	関数呼び出し値の初期値
		int		DEF_SPEED = 10;
		float	DEF_SCALE = 1.0f;
		Vector3	DEF_COLOR = Vector3(1.0f, 1.0f, 1.0f);
		int		DEF_MOVE = 100;

	}


}


//****************************************************************************************
//
//	Particleクラス
//
//****************************************************************************************
class Particle
{
private:
	iexParticle*			pt;
	int						timer;
	//	パーティクル量レベル
	enum
	{
		Par1 = 1,
		Par2 = 5,
		Par3 = 10,
		Par4 = 20
	};

	

public:
	//	初期化・解放
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	BlueFlame( const Vector3& pos, const float& scale =  DEF_SCALE);																										//	青炎
	void	Spark( const Vector3& pos, const float& scale = DEF_SCALE );																											//	火花
	void	Hit( const Vector3& pos, const float& scale = DEF_SCALE, const int& time = STAR_TIME );																				//	星
	void	Smoke( const Vector3& pos, const float& scale = DEF_SCALE, const int& time = SMOKE_TIME );																			//	煙
	void	PowerUp( const Vector3& pos, const float& scale = DEF_SCALE, const int& move = DEF_MOVE, const int& time = POWER_UP_TIME );												//	矢印UP
	void	SpeedUp( const Vector3& pos, const float& scale = DEF_SCALE, const int& move = DEF_MOVE, const int& time = SPEED_UP_TIME );												//	矢印DOWN
	void	Dust( const Vector3& pos, const Vector3& back, const Vector3& side, const float& scale = DEF_SCALE, const int& time = DUST_TIME );									//	砂埃
	void	Suck( const Vector3& pos, const Vector3& target, const Vector3& side, const float& length, const float& scale = DEF_SCALE, const int& time = SUCK_TIME );		//	吸い込み
	void	Bom( const Vector3& pos, const float& move, const float& scale = DEF_SCALE, const int& time = BOM_TIME );															//	爆発
	void	Flower( const Vector3& pos, const float& scale = DEF_SCALE, const Vector3& color = DEF_COLOR );																								//	花
	void	FlowerDisseminate( const Vector3& pos, const float& scale = DEF_SCALE, const float& speed = DEF_SPEED, const Vector3& color = DEF_COLOR );																		//	花ばらまき
	void	Bomb( const Vector3& pos, const float& scale = DEF_SCALE, const Vector3& color = DEF_COLOR );																									//	大砲発射後
	void	CoinGet(const Vector3& pos, const float& scale = DEF_SCALE );																									//	コイン枚数用エフェクト
	void	CoinUp( const Vector3& pos, const float& scale = DEF_SCALE );																									//	コイン獲得用エフェクト
	void	Magnet(const Vector3& pos, const float& length, const float& scale = DEF_SCALE, const int& speed = DEF_SCALE );													//	引き寄せエフェクト
	void	Death( const Vector3& pos, const float& scale = DEF_SCALE, const Vector3& color = DEF_COLOR );																									//	死亡エフェクト
	void	FireBall( const Vector3& pos, const float& scale = DEF_SCALE, const Vector3& color = DEF_COLOR );																								//	魔女ファイアー
	void	BombFireBall( const Vector3& pos, const float& scale = DEF_SCALE, const Vector3& color = DEF_COLOR );
	void	Semicircle( const Vector3& pos, const Vector3& front, const Vector3& right, const Vector3& up, const float& scale = DEF_SCALE );										//	突進時の風を切る	
	void	CannonSmoke( const Vector3& pos, const Vector3& front, const Vector3& right, const Vector3& up, const float& scale = DEF_SCALE );									//	大砲を打った時の煙



	//	情報取得
	static	Particle*	GetInstance( void );
};

#define	particle ( Particle::GetInstance() )

//****************************************************************************************
#endif // !__PARTICLE_H__
