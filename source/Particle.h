
#ifndef		__PARTICLE_H__
#define		__PARTICLE_H__

//****************************************************************************************
//
//	Particle�֘A
//
//****************************************************************************************

class Particle
{
private:
	iexParticle*		pt;
	int						timer;

public:
	//	�������E���
	bool	Initialize( void );
	void Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	BlueFlame( const Vector3& pos, const float& scale = 1.0f );		//	��
	void	Spark( const Vector3& pos, const float& scale = 1.0f );			//	�Ή�
	void	Particle::Hit( const Vector3& pos, const float& scale = 1.0f, const int& time = 5 );										//	��
	void	Particle::Smoke( const Vector3& pos, const float& scale = 1.0f, const int& time = 20 );										//	��
	void	Particle::Arrow_UP(const Vector3& pos, const float& scale = 1.0f, const int move = 100, const int& time = 10);				//	���UP
	void	Particle::Arrow_DOWN(const Vector3& pos, const float& scale = 1.0f, const int move = 100, const int& time =11);				//	���DOWN



	//	���擾
	static	Particle*	GetInstance( void );
};

#define	particle ( Particle::GetInstance() )

//****************************************************************************************
#endif // !__PARTICLE_H__
