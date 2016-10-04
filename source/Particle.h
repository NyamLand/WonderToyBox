
#ifndef		__PARTICLE_H__
#define		__PARTICLE_H__

//****************************************************************************************
//
//	Particle�֘A
//
//****************************************************************************************
//------------------------------------------------------------------------
//	�O���[�o��
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

		//	�֐��Ăяo���l�̏����l
		int		DEF_SPEED = 10;
		float	DEF_SCALE = 1.0f;
		Vector3	DEF_COLOR = Vector3(1.0f, 1.0f, 1.0f);
		int		DEF_MOVE = 100;

	}


}


//****************************************************************************************
//
//	Particle�N���X
//
//****************************************************************************************
class Particle
{
private:
	iexParticle*			pt;
	int						timer;
	//	�p�[�e�B�N���ʃ��x��
	enum
	{
		Par1 = 1,
		Par2 = 5,
		Par3 = 10,
		Par4 = 20
	};

	

public:
	//	�������E���
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	BlueFlame( const Vector3& pos, const float& scale =  DEF_SCALE);																										//	��
	void	Spark( const Vector3& pos, const float& scale = DEF_SCALE );																											//	�Ή�
	void	Hit( const Vector3& pos, const float& scale = DEF_SCALE, const int& time = STAR_TIME );																				//	��
	void	Smoke( const Vector3& pos, const float& scale = DEF_SCALE, const int& time = SMOKE_TIME );																			//	��
	void	PowerUp( const Vector3& pos, const float& scale = DEF_SCALE, const int& move = DEF_MOVE, const int& time = POWER_UP_TIME );												//	���UP
	void	SpeedUp( const Vector3& pos, const float& scale = DEF_SCALE, const int& move = DEF_MOVE, const int& time = SPEED_UP_TIME );												//	���DOWN
	void	Dust( const Vector3& pos, const Vector3& back, const Vector3& side, const float& scale = DEF_SCALE, const int& time = DUST_TIME );									//	����
	void	Suck( const Vector3& pos, const Vector3& target, const Vector3& side, const float& length, const float& scale = DEF_SCALE, const int& time = SUCK_TIME );		//	�z������
	void	Bom( const Vector3& pos, const float& move, const float& scale = DEF_SCALE, const int& time = BOM_TIME );															//	����
	void	Flower( const Vector3& pos, const float& scale = DEF_SCALE, const Vector3& color = DEF_COLOR );																								//	��
	void	FlowerDisseminate( const Vector3& pos, const float& scale = DEF_SCALE, const float& speed = DEF_SPEED, const Vector3& color = DEF_COLOR );																		//	�Ԃ΂�܂�
	void	Bomb( const Vector3& pos, const float& scale = DEF_SCALE, const Vector3& color = DEF_COLOR );																									//	��C���ˌ�
	void	CoinGet(const Vector3& pos, const float& scale = DEF_SCALE );																									//	�R�C�������p�G�t�F�N�g
	void	CoinUp( const Vector3& pos, const float& scale = DEF_SCALE );																									//	�R�C���l���p�G�t�F�N�g
	void	Magnet(const Vector3& pos, const float& length, const float& scale = DEF_SCALE, const int& speed = DEF_SCALE );													//	�����񂹃G�t�F�N�g
	void	Death( const Vector3& pos, const float& scale = DEF_SCALE, const Vector3& color = DEF_COLOR );																									//	���S�G�t�F�N�g
	void	FireBall( const Vector3& pos, const float& scale = DEF_SCALE, const Vector3& color = DEF_COLOR );																								//	�����t�@�C�A�[
	void	BombFireBall( const Vector3& pos, const float& scale = DEF_SCALE, const Vector3& color = DEF_COLOR );
	void	Semicircle( const Vector3& pos, const Vector3& front, const Vector3& right, const Vector3& up, const float& scale = DEF_SCALE );										//	�ːi���̕���؂�	
	void	CannonSmoke( const Vector3& pos, const Vector3& front, const Vector3& right, const Vector3& up, const float& scale = DEF_SCALE );									//	��C��ł������̉�



	//	���擾
	static	Particle*	GetInstance( void );
};

#define	particle ( Particle::GetInstance() )

//****************************************************************************************
#endif // !__PARTICLE_H__
