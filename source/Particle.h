
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
	void	Particle::Hit( const Vector3& pos, const float& scale = 1.0f, const int& time = 5 );										//	星
	void	Particle::Smoke( const Vector3& pos, const float& scale = 1.0f, const int& time = 20 );										//	煙
	void	Particle::Arrow_UP(const Vector3& pos, const float& scale = 1.0f, const int move = 100, const int& time = 10);				//	矢印UP
	void	Particle::Arrow_DOWN(const Vector3& pos, const float& scale = 1.0f, const int move = 100, const int& time =11);				//	矢印DOWN



	//	情報取得
	static	Particle*	GetInstance( void );
};

#define	particle ( Particle::GetInstance() )

//****************************************************************************************
#endif // !__PARTICLE_H__
