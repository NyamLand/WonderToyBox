
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
	iex2DObj**		effectImage;

public:
	//	�������E���
	bool	Initialize( void );
	void Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	BlueFlame( const Vector3& pos, const float& scale = 1.0f );		//	��
	void Spark( const Vector3& pos, const float& scale = 1.0f );			//	�Ή�

	//	���擾
	static	Particle*	GetInstance( void );
};

#define	particle ( Particle::GetInstance() )

//****************************************************************************************
#endif // !__PARTICLE_H__
