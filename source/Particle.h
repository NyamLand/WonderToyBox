
#ifndef		__PARTICLE_H__
#define		__PARTICLE_H__

//****************************************************************************************
//
//	Particle関連
//
//****************************************************************************************

class Particle
{
private:
	iexParticle*		pt;
	int						timer;

public:
	//	初期化・解放
	bool	Initialize( void );
	void Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	BlueFlame( const Vector3& pos, const float& scale = 1.0f );		//	青炎
	void	Spark( const Vector3& pos, const float& scale = 1.0f );			//	火花
	void	Hit( const Vector3& pos, const float& scale = 1.0f, const int& time = 5 );										//	星
	void	Smoke( const Vector3& pos, const float& scale = 1.0f, const int& time = 20 );										//	煙
	void	PowerUp(const Vector3& pos, const float& scale = 1.0f, const int move = 100, const int& time = 10);				//	矢印UP
	void	SpeedUp(const Vector3& pos, const float& scale = 1.0f, const int move = 100, const int& time = 11);				//	矢印DOWN
	void	Suck(const Vector3& pos, const Vector3& target, const Vector3& side, const float& length, const float& scale = 1.0f, const int& time = 60);					//	吸い込み
	void	Dust(const Vector3& pos, const Vector3& back, const Vector3& side, const float& scale = 1.0f, const int& time = 1);
	void	Bom(const Vector3& pos, const float& move, const float& scale = 1.0f, const int& time = 6);
	void	Flower( const Vector3& pos, float scale, const Vector3& color);
	void	FlowerDisseminate( const Vector3& pos, float scale, float speed, const Vector3& color );
	void	Bomb( const Vector3& pos, float scale, const Vector3& color );
	void	CoinGet(const Vector3& pos, const float& scale = 1.0f);										//	コイン枚数用エフェクト
	void	CoinUp( const Vector3& pos, const float& scale = 1.0f );									//	コイン獲得用エフェクト
	void	Magnet(const Vector3& pos, const float& length, const float& scale = 1.0f, const int& speed = 10);					//	引き寄せエフェクト
	void	Death( const Vector3& pos, float scale, const Vector3& color );				//	死亡エフェクト
	void	Semicircle(const Vector3& pos, const Vector3& front, const Vector3& right, const Vector3& up, const float& scale = 1.0f);	//	突進時の風を切る	
	void	CannonSmoke(const Vector3& pos, const Vector3& front, const Vector3& right, const Vector3& up, const float& scale = 1.0f);	//	大砲を打った時の煙





	//	情報取得
	static	Particle*	GetInstance( void );
};

#define	particle ( Particle::GetInstance() )

//****************************************************************************************
#endif // !__PARTICLE_H__
