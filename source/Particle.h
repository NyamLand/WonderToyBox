
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
	iex2DObj**		effectImage;

public:
	//	初期化・解放
	bool	Initialize( void );
	void Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	BlueFlame( const Vector3& pos, const float& scale = 1.0f );		//	青炎
	void Spark( const Vector3& pos, const float& scale = 1.0f );			//	火花

	//	情報取得
	static	Particle*	GetInstance( void );
};

#define	particle ( Particle::GetInstance() )

//****************************************************************************************
#endif // !__PARTICLE_H__
