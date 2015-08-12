
#include	"iextreme.h"
#include	"system/System.h"
#include	<random>
#include	"GlobalFunction.h"
#include	"Coin.h"

#include	"CoinManager.h"

//******************************************************************************
//
//	CoinManager�N���X
//
//******************************************************************************

//---------------------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------------------
CoinManager*	m_CoinManager;

//---------------------------------------------------------------------------------
//	�������E���
//---------------------------------------------------------------------------------

	//	�R���X�g���N�^
	CoinManager::CoinManager( void )
	{
	
	}
	
	//	�f�X�g���N�^
	CoinManager::~CoinManager( void )
	{

	}

	//	������
	bool	CoinManager::Initialize( void )
	{
		org = new iexMesh( "DATA/coin.imo" );
		c_Coin = new Coin[ COIN_MAX ];
		coin_num = 0;

		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i].Initialize();
			c_Coin[i].state = false;
		}

		//	�R�C���������_������
		std::uniform_real_distribution<float> posrand( -10.0f, 10.0f );
		std::uniform_real_distribution<float> heightrand( 0.0f, 50.0f );
		std::uniform_real_distribution<float>	moverand( -0.1f, 0.1f );
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			Set( Vector3( posrand( ran ), heightrand( ran ), posrand( ran ) ), Vector3( 0.0f, 0.0f, 0.0f ), 1.0f );
		}

		if ( org != NULL ) 	return	false;
		return	true;
	}

//---------------------------------------------------------------------------------
//	�X�V�E�`��
//---------------------------------------------------------------------------------

	//	�X�V
	void	CoinManager::Update( void )
	{
		//	�����J�E���g������
		coin_num = 0;

		for ( int i = 0; i < COIN_MAX; i++ )
		{
			if ( !c_Coin[i].state )	continue;
			coin_num++;
			
			//	�ʒu����
			DistCheck( i );
			c_Coin[i].Update();
		}
	}

	//	�`��
	void	CoinManager::Render( void )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			if ( !c_Coin[i].state )	continue;
			c_Coin[i].Render();
		}

		//	�f�o�b�O�p
		if ( !debug )	return;	

		char	str[256];
		sprintf_s( str, "coin_num = %d", coin_num );
		DrawString( str, 20, 100 );
	}

	//	�V�F�[�_�[�t���`��
	void	CoinManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			if ( !c_Coin[i].state )	continue;
			c_Coin[i].Render( shader, technique );
		}
	}

//---------------------------------------------------------------------------------
//	����֐�
//---------------------------------------------------------------------------------

	//	����
	void	CoinManager::Set( const Vector3& pos, const Vector3& vec, float speed )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			if ( c_Coin[i].state )	continue;

			c_Coin[i].judgeTimer		=		30;
			c_Coin[i].activate			=		false;
			c_Coin[i].state				=		true;
			c_Coin[i].obj					=		org->Clone();
			c_Coin[i].pos					=		pos;
			Vector3	v						=		vec;
			v.Normalize();
			c_Coin[i].move				=		v * speed;
			c_Coin[i].scale				=		0.5f;
			break;
		}
	}

	//	�ʒu����
	void	CoinManager::DistCheck( int n )
	{
		//	�R�C�����m�̈ʒu�`�F�b�N		
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			//	����vs�����͏��O		
			if ( i == n ) continue;

			//	����������ւ̃x�N�g��
			Vector3	coin_pos1 = c_Coin[n].GetPos();
			Vector3	coin_pos2 = c_Coin[i].GetPos();
			Vector3	vec = coin_pos2 - coin_pos1;
			

			//	�����v��			
			float length;
			length = vec.Length();

			//	�߂��ꍇ�͗���			
			if ( length < 0.5f )
			{
				//	�x�N�g�����K��		
				vec.Normalize();

				//	����
				coin_pos1 =  coin_pos1 - vec * 0.5f;

				//	�ʒu���ݒ�
				c_Coin[n].SetPos( coin_pos1 );
			}
			             
		}
	}