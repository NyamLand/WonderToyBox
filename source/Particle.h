
#ifndef		__PARTICLE_H__
#define		__PARTICLE_H__

//****************************************************************************************
//
//	ParticleึA
//
//****************************************************************************************

class Particle
{
private:
	iexParticle*		pt;
	iex2DObj**		effectImage;
	int						timer;

public:
	//	๚ปE๐๚
	bool	Initialize( void );
	void Release( void );

	//	XVE`ๆ
	void	Update( void );
	void	Render( void );

	//	ฎ์ึ
	void	BlueFlame( const Vector3& pos, const float& scale = 1.0f );		//	ย
	void	Spark( const Vector3& pos, const float& scale = 1.0f );			//	ฮิ
	void	Particle::Hit( const Vector3& pos, const int& time = 1, const float& scale = 1.0f );										//	ฏ
	void	Particle::Smoke( const Vector3& pos, const int& time = 1, const float& scale = 1.0f );										//	
	void	Particle::Arrow_UP(const Vector3& pos, const int& time = 1, const float& scale = 1.0f, const int move = 100);				//	๎๓UP
	void	Particle::Arrow_DOWN(const Vector3& pos, const int& time = 1, const float& scale = 1.0f, const int move = 100);				//	๎๓DOWN



	//	๎๑ๆพ
	static	Particle*	GetInstance( void );
};

#define	particle ( Particle::GetInstance() )

//****************************************************************************************
#endif // !__PARTICLE_H__
