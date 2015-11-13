
#include	"iextreme.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GlobalFunction.h"

#include	"CoinManager.h"

//******************************************************************************
//
//	CoinManager�N���X
//
//******************************************************************************

//	�萔
#define	COIN_SIZE	2.0f

//---------------------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------------------

	//	����
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
		//	���f���ǂݍ���
		org = nullptr;
		org = make_unique<iexMesh>( LPSTR( "DATA/Object/coin01.imo" ) );

		//	�R�C��������
		c_Coin = new Coin[ COIN_MAX ];
		coin_num = 0;
		FOR( 0, COIN_MAX )
		{
			c_Coin[value].Initialize();
			c_Coin[value].state = false;
		}

		if ( org != nullptr ) 	return	false;
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

		//	�S�R�C���X�V
		FOR( 0, COIN_MAX )
		{
			if ( !c_Coin[value].state )	continue;
			
			//	�����J�E���g
			coin_num++;
			
			//	�ʒu����
			DistCheck( value );

			//	���X�V
			c_Coin[value].Update();
		}
	}

	//	�`��
	void	CoinManager::Render( iexShader* shader, LPSTR technique )
	{
		//	���݂��Ă���S�R�C���`��
		FOR( 0, COIN_MAX )
		{
			//	���݂��ĂȂ�������X�L�b�v
			if ( !c_Coin[value].state )	continue;

			//	�R�C���`��
			if ( shader == nullptr || technique == nullptr )
			{
				c_Coin[value].Render();
			}
			else
			{
				c_Coin[value].Render( shader, technique );
			}
		}
	}

//---------------------------------------------------------------------------------
//	����֐�
//---------------------------------------------------------------------------------

	//	����
	void	CoinManager::Set( const Vector3& pos, const Vector3& vec, const float& speed )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			if ( c_Coin[i].state )	continue;

			c_Coin[i].judgeTimer		=		30;
			c_Coin[i].activate			=		false;
			c_Coin[i].obj					=		org->Clone();
			c_Coin[i].pos					=		pos;
			Vector3	v						=		vec;
			v.Normalize();
			c_Coin[i].move				=		v * speed;
			c_Coin[i].scale				=		COIN_SIZE;
			c_Coin[i].state				=		true;
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

//---------------------------------------------------------------------------------
//	���ݒ�
//---------------------------------------------------------------------------------

	//	�R�C���̃��[�h�ݒ�
	void	CoinManager::SetCoinGetAwayFlag( bool flag )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i].SetGetAwayFlag( flag );
		}
	}


	void	CoinManager::SetState(bool state)
	{
		for (int i = 0; i < COIN_MAX; i++)
		{
			c_Coin[i].SetState(state);
		}
	}
//---------------------------------------------------------------------------------
//	���擾
//---------------------------------------------------------------------------------

	//	�^����ꂽ���W�Ɉ�ԋ߂��R�C���̍��W��Ԃ�
	bool	CoinManager::GetMinPos( Vector3& out, const Vector3& pos )
	{
		//	�K���ɂł������l��p��
		float		dist = 1000.0f;

		//	�R�C���S�����`�F�b�N
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			//	���݃`�F�b�N
			if ( !c_Coin[i].state )	continue;

			//	�R�C���̍��W���擾
			Vector3 coin_pos = c_Coin[i].GetPos();

			//	�^����ꂽ���W�Ƃ̋������Z�o
			float	length = ( coin_pos - pos ).Length();

			//	�������ŏ��l���Z���������������
			if ( length < dist )
			{
				dist = length;
				out = coin_pos;
			}
		}

		//	�l���ς���ĂȂ�������false��Ԃ�
		if ( dist >= 1000.0f )	return	false;
		return	true;
	}

	//	�t�B�[���h�ɑ��݂���R�C���̖������擾
	int		CoinManager::GetFreeCoinNum( void )const
	{
		return	coin_num;
	}

	Coin*	CoinManager::GetCoin( void )const
	{
		return c_Coin;
	}