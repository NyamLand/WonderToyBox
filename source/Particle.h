
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
	static	iexParticle*	particle;

	static	iex2DObj*		par[];
	static	int				timer;

public:
	//	初期化・解放
	Particle( void );
	~Particle( void );
	static bool	Initialize( void );
	static void Release( void );

	//	更新・描画
	static void	Update( void );
	static void	Render( void );

	//	動作関数
	static void	BlueFlame( const Vector3& pos, const float& scale = 1.0f );		//	青炎
	static void Spark( const Vector3& pos, const float& scale = 1.0f );			//	火花
	static void Hit( const Vector3& pos, const int& time = 1, const float& scale = 1.0f);			//	ヒット時
	static void Smoke( const Vector3& pos, const int& time = 1, const float& scale = 1.0f);			//	ヒット時
};

//****************************************************************************************
#endif // !__PARTICLE_H__
