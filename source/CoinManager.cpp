
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Coin.h"
#include	<random>

#include	"CoinManager.h"

//******************************************************************************
//
//	CoinManager�N���X
//
//******************************************************************************

//---------------------------------------------------------------------------------
//	�������E���
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
		for ( int i = 0; i < COIN_MAX; i++ )
			SafeDelete( c_Coin[i] );
	}

	//	������
	bool	CoinManager::Initialize( void )
	{
		std::uniform_real_distribution<float> posrand( -20.0f, 20.0f );
		std::uniform_real_distribution<float> heightrand( 0.0f, 50.0f );
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i] = new Coin();
			c_Coin[i]->Initialize();
			c_Coin[i]->SetPos( Vector3( posrand( ran ), heightrand( ran ), posrand( ran ) ) );
		}

		return	true;
	}

//---------------------------------------------------------------------------------
//	�X�V�E�`��
//---------------------------------------------------------------------------------

	//	�X�V
	void	CoinManager::Update( void )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i]->Update();
		}
	}

	//	�`��
	void	CoinManager::Render( void )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i]->Render();
		}
	}

	//	�V�F�[�_�[�t���`��
	void	CoinManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i]->Render( shader, technique );
		}
	}