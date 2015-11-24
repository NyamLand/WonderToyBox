
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
#define	COIN_SIZE	2.5f

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
		org = nullptr;
		org = new iexMesh( "DATA/Object/coin01.imo" );

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

		SafeDelete( org );
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

		//	�f�o�b�O�p
		if ( debug )
		{
			char	str[256];
			sprintf_s( str, "�R�C������ = %d", coin_num );
			DrawString( str, 10, 100 );
		}
	}

//---------------------------------------------------------------------------------
//	����֐�
//---------------------------------------------------------------------------------

	//	����
	void	CoinManager::Append( const Vector3& pos, const Vector3& vec, const float& speed )
	{
		//	�e����ݒ�
		Coin* obj = new Coin();
		obj->Initialize();
		obj->judgeTimer = 30;
		obj->activate = false;
		obj->obj = org->Clone();
		Vector3	v = vec;
		v.Normalize();
		Vector3	moveParam = v * speed;
		obj->SetPos( pos );
		obj->SetMove( moveParam );
		obj->SetScale( COIN_SIZE );
		obj->SetState( true );
		obj->Update();

		//	���X�g�ɒǉ�
		coinList.push_back( obj );
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