
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"ItemManager.h"

//**********************************************************************************
//
//	ItemManager�N���X
//
//**********************************************************************************

//------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------

	//	������
	bool	ItemManager::Initialize( void )
	{
		//	�I���W�i��������
		FOR( 0, ITEM_TYPE::MAX )
		{
			org[value] = nullptr;
		}
		org[ITEM_TYPE::ATTACK_UP] = new iexMesh( "DATA/Object/Item/Item.IMO" );
		org[ITEM_TYPE::MAGNET] = new iexMesh( "DATA/Object/Item/Item04.IMO" );
		org[ITEM_TYPE::SPEED_UP] = new iexMesh( "DATA/Object/Item/Item03.IMO" );
		org[ITEM_TYPE::UNRIVALED] = new iexMesh();
		itemList.clear();

		return	true;
	}

	//	���
	void	ItemManager::Release( void )
	{
		FOR( 0, ITEM_TYPE::MAX )
		{
			SafeDelete( org[value] );
		}
		
		for ( auto it = itemList.begin(); it != itemList.end(); )
		{
			it = itemList.erase( it );
		}
	}

//------------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------------

	//	�X�V
	void	ItemManager::Update( void )
	{
		for ( auto it = itemList.begin(); it != itemList.end(); it++ )
		{
			( *it )->Update();

			bool	state = ( *it )->GetState();
			
			if ( !state )
			{
				it = itemList.erase( it );
				break;
			}
		}
	}

	//	�`��
	void	ItemManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( auto it = itemList.begin(); it != itemList.end(); it++ )
		{
			if (shader == nullptr || technique == nullptr)
			{
				( *it )->Render();
			}
			else
			{
				( *it )->Render( shader, technique );
			}
		}
	}

//------------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------------

	//	���X�g�ǉ�
	void	ItemManager::Append( const Vector3& pos, const int& type )
	{
		Item*	item = nullptr;
		item = new Item();
		item->Initialize();
		item->SetMesh( org[type]->Clone() );
		item->SetType( type );
		item->SetPos( pos );
		item->Update();
		item->SetState( true );
		itemList.push_back( item );
	}

//------------------------------------------------------------------------------------
//	���擾
//------------------------------------------------------------------------------------

	//	���̎擾
	ItemManager*	ItemManager::GetInstance( void )
	{
		static	ItemManager	out;
		return	&out;
	}

	//	���X�g�擾
	std::list<Item*>	ItemManager::GetList( void )
	{
		return	itemList;
	}

	//	�^�������W�Ɉ�ԋ߂����W���擾
	bool	ItemManager::GetMinPos( Vector3& out, const Vector3& pos )
	{
		//	�K���ɐ��l��p��
		float		dist = 1000.0f;

		//	���X�g�̖��[�܂Ō���
		for ( auto it = itemList.begin(); it != itemList.end(); it++ )
		{
			//	�A�C�e���̍��W���擾
			Vector3 i_pos = ( *it )->GetPos();

			//	�󂯎�������W�ƃA�C�e���̍��W�̋������Z�o
			float	length = ( i_pos - pos ).Length();

			//	�������ŏ��l��菬��������������㏑��
			if ( length < dist )
			{
				dist = length;
				out = i_pos;
			}
		}

		//	�l���ς���ĂȂ�������false��Ԃ�
		if ( dist >= 1000.0f )	return	false;
		else	return	true;
	}
