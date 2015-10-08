
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

	//	�R���X�g���N�^
	ItemManager::ItemManager( void )
	{
	
	}

	//	�f�X�g���N�^
	ItemManager::~ItemManager( void )
	{

	}

	//	������
	bool	ItemManager::Initialize( void )
	{
		org = new iexMesh( "DATA/Object/Item.IMO" );
		itemList.clear();

		for ( int i = 0; i < ITEM_MAX; i++ )
		{
			Append( Vector3( Random::GetFloat( -10.0f, 10.0f ), Random::GetFloat( 0.0f, 50.0f ), Random::GetFloat( -10.0f, 10.0f ) ) );
		}

		return	true;
	}

	//	���
	void	ItemManager::Release( void )
	{
		SafeDelete( org );
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
	void	ItemManager::Render( void )
	{
		for ( auto it = itemList.begin(); it != itemList.end(); it++ )
		{
			( *it )->Render();
		}
	}

	//	�V�F�[�_�[�t���`��
	void	ItemManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( auto it = itemList.begin(); it != itemList.end(); it++ )
		{
			(*it)->Render( shader, technique );
		}
	}

//------------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------------

	//	���X�g�ǉ�
	void	ItemManager::Append( const Vector3& pos )
	{
		Item*	item = nullptr;
		item = new Item();
		item->Initialize();
		item->SetMesh( org->Clone() );
		item->SetPos( pos );
		item->Update();
		item->SetState( true );
		itemList.push_back( item );
	}

//------------------------------------------------------------------------------------
//	���擾
//------------------------------------------------------------------------------------

	//	���̎擾
	ItemManager*	ItemManager::GetInstance()
	{
		static	ItemManager	out;
		return	&out;
	}

	//	���X�g�擾
	std::list<Item*>	ItemManager::GetList( void )
	{
		return	itemList;
	}

