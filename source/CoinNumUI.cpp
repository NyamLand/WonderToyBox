
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"NumberUI.h"
#include	"CharacterManager.h"
#include	"Particle.h"
#include	"Camera.h"
#include	"CoinUIEffect.h"
#include	"CoinNumUI.h"

//**********************************************************************
//
//	CoinNumUI�N���X
//
//**********************************************************************

//-------------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------------

#define	NUMBER_SCALE	100

//-------------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	CoinNumUI::CoinNumUI( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			numberUI[value] = nullptr;
		}
	}

	//	�f�X�g���N�^
	CoinNumUI::~CoinNumUI( void )
	{
		Release();
	}

	//	������
	bool	CoinNumUI::Initialize( void )
	{
		//	���W�A�F�e�[�u��������
		TableInitialize();

		//	�G�t�F�N�g������
		coinUIEffect = new CoinUIEffect();
		coinUIEffect->Initialize();

		//	��񏉊���
		FOR( 0, PLAYER_MAX )
		{
			//	�R�C������������
			coinNum[value] = gameManager->GetCoinNum( value );

			//	���l�N���X������
			numberUI[value] = new NumberUI();
			numberUI[value]->Initialize( posXTable[value],
				static_cast<int>( iexSystem::ScreenHeight * 0.1f ),
				NUMBER_SCALE,
				coinNum[value] );

			//	�G�t�F�N�g�N���X�ɍ��W��n��
			coinUIEffect->SetPos( value, 
				numberUI[value]->GetPosX(), 
				numberUI[value]->GetPosY() );

			if ( numberUI[value] == nullptr )		return	false;
		}

		return	true;
	}

	//	���
	void	CoinNumUI::Release( void )
	{
		FOR( 0, PLAYER_MAX )	SafeDelete( numberUI[value] );
	}

	//	�ǂݍ���
	void	CoinNumUI::Load( void )
	{

	}

	//	���e�[�u��������
	void	CoinNumUI::TableInitialize( void )
	{
		//	���W�e�[�u��������
		posXTable[0] = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		posXTable[1] = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
		posXTable[2] = static_cast<int>( iexSystem::ScreenWidth * 0.7f );
		posXTable[3] = static_cast<int>( iexSystem::ScreenWidth * 0.9f );

		//	�\���F������
		FOR( 0, PLAYER_MAX )
		{
			colorTable[value] = characterManager->GetDamageColor( value );
		}

		//	���₷�����邽�ߏ������邭
		colorTable[1] += Vector3( 0.7f, 0.7f, 0.0f );	
	}

//-------------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------------

	//	�X�V
	void	CoinNumUI::Update( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			if ( gameManager->GetTimer() / SECOND < 15 )
			{
				numberUI[value]->SetSecretFlag( true );
			}
			CoinCount( gameManager->GetCoinNum( value ), value );
			numberUI[value]->SetNumber( coinNum[value] );
			numberUI[value]->Update();
			numberUI[value]->SetColor( colorTable[value] );
		}
	}

	//	�G�t�F�N�g�X�V
	void	CoinNumUI::CoinEffectUpdate( void )
	{
		coinUIEffect->Update();
	}

	//	�`��
	void	CoinNumUI::Render( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			numberUI[value]->Render();
			coinUIEffect->Render( value );
		}

	}

	//	�G�t�F�N�g�o�b�N�o�b�t�@�`��
	void	CoinNumUI::RenderToBackBuffer( void )
	{
		coinUIEffect->RenderToBackBaffer();
	}

//-------------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------------

	//	�R�C���������ꖇ���J�E���g�A�b�v�_�E��( �R�C�������A�v���C���[�ԍ� )
	void	CoinNumUI::CoinCount( int coinNum, int playerNum )
	{
		//	���l�ɕω����Ȃ���΃X���[
		if ( this->coinNum[playerNum] == coinNum )		return;

		//	�R�C������
		if ( this->coinNum[playerNum] > coinNum )	this->coinNum[playerNum]--;

		//	�R�C������
		if ( this->coinNum[playerNum] < coinNum )
		{
			this->coinNum[playerNum]++;
			coinUIEffect->SetEffectFlag( playerNum, true );
		}
	}
	