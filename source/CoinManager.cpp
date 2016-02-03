
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
#define	COIN_SIZE	3.0f
#define	COIN_BAG_SIZE 0.05f

//---------------------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//	�������E���
//---------------------------------------------------------------------------------

	//	������
	bool	CoinManager::Initialize( void )
	{
		//	�����f���ǂݍ���
		FOR( 0, Coin::MAX )	org[value] = nullptr;
		org[Coin::COIN] = new iexMesh( "DATA/Object/Coin/Coin/coin01.imo" );
		org[Coin::COIN_BAG_5] = new iexMesh( "DATA/Object/Coin/CoinBag/CoinBag5.IMO" );
		org[Coin::COIN_BAG_10] = new iexMesh( "DATA/Object/Coin/CoinBag/CoinBag10.IMO" );

		//	���X�g������
		coinList.clear();

		if ( org != nullptr ) 	return	false;
		return	true;
	}

	//	���
	void	CoinManager::Release( void )
	{
		for ( auto it = coinList.begin(); it != coinList.end(); )
		{
			it = coinList.erase( it );
		}

		FOR( 0, Coin::MAX ) 
		{
			SafeDelete( org[value] );
		}
	}

//---------------------------------------------------------------------------------
//	�X�V�E�`��
//---------------------------------------------------------------------------------

	//	�X�V
	void	CoinManager::Update( void )
	{
		//	�����J�E���g������
		coin_num = 0;

		//	���X�g���R�C���X�V
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			//	�����J�E���g
			coin_num++;

			//	���X�V
			( *it )->Update();

			//	���ł��Ă����烊�X�g����폜
			bool state = ( *it )->GetState();
			if ( !state )
			{
				it = coinList.erase( it );
				break;
			}
		}
	}

	//	�`��
	void	CoinManager::Render( iexShader* shader, LPSTR technique )
	{
		//	�R�C���`��
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			if ( shader == nullptr || technique == nullptr )
			{
				( *it )->Render();
			}
			else
			{
				( *it )->Render( shader, technique );
			}
		}
	}

//---------------------------------------------------------------------------------
//	����֐�
//---------------------------------------------------------------------------------

	//	����
	void	CoinManager::Append( const Vector3& pos, const Vector3& vec, const float& speed, int type )
	{
		Coin*	coin = new Coin();
		//	�e����ݒ�
		coin->Initialize();
		coin->judgeTimer = 30;
		coin->activate = false;
		coin->type = type;
		coin->obj = org[type]->Clone();
		Vector3	v = vec;
		//v.Normalize();
		Vector3	moveParam = v * speed;
		coin->SetPos( pos );
		coin->SetMove( moveParam );
		coin->SetState( true );

		switch ( type )
		{
		case Coin::COIN:
			coin->SetScale( COIN_SIZE );
			break;

		case Coin::COIN_BAG_5:
		case Coin::COIN_BAG_10:
			coin->SetScale( COIN_BAG_SIZE );
			break;
		}
		coin->Update();

		//	���X�g�ɒǉ�
		coinList.push_back( coin );
	}

//---------------------------------------------------------------------------------
//	���ݒ�
//---------------------------------------------------------------------------------

	//	�R�C���̃��[�h�ݒ�
	void	CoinManager::SetCoinGetAwayFlag( bool flag )
	{
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			( *it )->SetGetAwayFlag( flag );
		}
	}

	//	��Ԏ擾
	void	CoinManager::SetState( bool state )
	{
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			( *it )->SetState( state );
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
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			if ( !( *it )->GetState() )	continue;

			//	�R�C���̍��W���擾
			Vector3 coin_pos = ( *it )->GetPos();

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
	
	//�@�߂��ɂ���R�C���̐���Ԃ�
	int		CoinManager::GetNearCoinNum( float dist, const Vector3& pos )
	{
		int	coinNum = 0;

		//	�R�C���S�����`�F�b�N
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			if ( !( *it )->GetState() )	continue;

			//	�R�C���̍��W���擾
			Vector3 coin_pos = ( *it )->GetPos();

			//	�^����ꂽ���W�Ƃ̋������Z�o
			float	length = ( coin_pos - pos ).Length();

			//	�������ŏ��l���Z���������������
			if ( length < dist )
			{
				coinNum++;
			}
		}

		return coinNum;
	}

	//	�R�C�����X�g�擾
	list<Coin*>	CoinManager::GetList( void )const
	{
		return	coinList;
	}

	//	���̎擾
	CoinManager*	CoinManager::GetInstance( void )
	{
		static	CoinManager	out;
		return	&out;
	}