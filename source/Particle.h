
#ifndef		__PARTICLE_H__
#define		__PARTICLE_H__

//****************************************************************************************
//
//	ParticleÖA
//
//****************************************************************************************

class Particle
{
private:
	iexParticle*		pt;
	int						timer;

public:
	//	ú»Eðú
	bool	Initialize( void );
	void Release( void );

	//	XVE`æ
	void	Update( void );
	void	Render( void );

	//	®ìÖ
	void	BlueFlame( const Vector3& pos, const float& scale = 1.0f );		//	Â
	void	Spark( const Vector3& pos, const float& scale = 1.0f );			//	ÎÔ
	void	Hit( const Vector3& pos, const float& scale = 1.0f, const int& time = 5 );										//	¯
	void	Smoke( const Vector3& pos, const float& scale = 1.0f, const int& time = 20 );										//	
	void	Arrow_UP(const Vector3& pos, const float& scale = 1.0f, const int move = 100, const int& time = 10);				//	îóUP
	void	Arrow_DOWN(const Vector3& pos, const float& scale = 1.0f, const int move = 100, const int& time =11);				//	îóDOWN
	void	Suck(const Vector3& pos, const Vector3& target, const Vector3& side, const float& length, const float& scale = 1.0f, const int& time = 60);					//	z¢Ý
	void	Dust(const Vector3& pos, const Vector3& back, const Vector3& side, const float& scale = 1.0f, const int& time = 1);
	void	Bom(const Vector3& pos, const float& move, const float& scale = 1.0f, const int& time = 6);
	void	Flower( const Vector3& pos, float scale, const Vector3& color);
	void	FlowerDisseminate( const Vector3& pos, float scale, float speed, const Vector3& color );
	void	CoinGet(const Vector3& pos, const float& scale = 1.0f);										//	RCGtFNg




	//	îñæ¾
	static	Particle*	GetInstance( void );
};

#define	particle ( Particle::GetInstance() )

//****************************************************************************************
#endif // !__PARTICLE_H__
