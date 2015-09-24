
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Particle.h"

//*******************************************************************************
//
//	Paticle�N���X
//
//*******************************************************************************

//------------------------------------------------------------------------
//	�O���[�o��
//------------------------------------------------------------------------

iexParticle*	Particle::particle = NULL;

//------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------

	//	�R���X�g���N�^
	Particle::Particle( void )
	{

	}

	//	�f�X�g���N�^
	Particle::~Particle( void )
	{
		Release();
	}

	//	������
	bool	Particle::Initialize( void )
	{
		particle = new iexParticle();
		particle->Initialize( "DATA/particle.png", 10000 );

		return	true;
	}

	//	���
	void	Particle::Release( void )
	{
		SafeDelete( particle );
	}

//------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------

	//	�X�V
	void	Particle::Update( void )
	{
		particle->Update();
	}

	//	�`��
	void	Particle::Render( void )
	{
		particle->Render();
	}

//------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------

	//	��
	void	Particle::BlueFlame( const Vector3& pos, const float& scale )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<5; j++ )
		{
			Pos.x = pos.x + ( ( rand() % 1000 - 1000 ) * ( 0.001f * scale ) );
			Pos.y = pos.y + ( ( rand() % 500 - 500 ) * ( 0.001f * scale ) );
			Pos.z = pos.z + ( ( rand() % 1000 - 1000 ) * ( 0.001f * scale ) );

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = (rand() % 100)* (scale * 0.00015f);
			Power.z = 0.0f;

			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			particle->Set( 6, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.2f, 1.0f, 0.3f, scale, RS_COPY );
		}
	}

	//	�Ή�
	void	Particle::Spark( const Vector3& pos, const float& scale )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<10; j++ )
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = ( rand() % 200 - 100 ) * ( 0.002f * scale );
			Move.y = 1.0f * scale;
			Move.z = ( rand() % 200 - 100 ) * ( 0.002f * scale );

			Power.x = 0.0f;
			Power.y = -( rand() % 100 ) * ( scale * 0.001f );
			Power.z = 0.0f;

			//					�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			particle->Set( 1, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale, RS_COPY );
		}
	}