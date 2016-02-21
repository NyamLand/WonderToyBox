
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"system/System.h"
#include	"Particle.h"
#include	"Camera.h"
#include	"CoinUIEffect.h"

//**********************************************************************
//
//	CoinUIEffect�N���X
//	
//**********************************************************************

//---------------------------------------------------------------------------------------
//	�O���[�o��
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//	�������E���
//---------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	CoinUIEffect::CoinUIEffect( void ) : targetPar( nullptr ), camera( nullptr ),
		parPos( 0.0f, 0.0f, 0.0f ), posY( 0 ), scale( 0 )
	{
		FOR( 0, PLAYER_MAX )
		{
			coinFlag[value] = false;
			coinTimer[value] = 0;
			posX[value] = 0;
		}
	}

	//	�f�X�g���N�^
	CoinUIEffect::~CoinUIEffect( void )
	{
		Release();
	}

	//	������
	bool	CoinUIEffect::Initialize( void )
	{
		//	�p�[�e�B�N���p�o�b�t�@
		parPos = Vector3( 100.0f, 100.0f, 100.0f );
		targetPar = new iex2DObj( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET );
		camera = new Camera();
		camera->SetPos( Vector3( 0.0f, 10.0f, -10.0f ) + parPos );
		return	true;
	}

	//	���
	void	CoinUIEffect::Release( void )
	{
		SafeDelete( camera );
		SafeDelete( targetPar );
	}

//---------------------------------------------------------------------------------------
//	�X�V�E�`��
//---------------------------------------------------------------------------------------

	//	�X�V
	void	CoinUIEffect::Update( void )
	{
		camera->Update( VIEW_MODE::FIX, parPos );
		particle->Update();

		FOR( 0, PLAYER_MAX )
		{
			//	�p�[�e�B�N����30�t���[���Ԃ����`�拖����
			if ( coinFlag[value] )
			{
				coinTimer[value]++;
				if ( coinTimer[value] > 30 )
				{
					coinFlag[value] = false;
					coinTimer[value] = 0;
				}
			}
		}
	}

	//	�`��
	void	CoinUIEffect::Render( int player )
	{
		if ( !coinFlag[player] )	return;

		targetPar->Render( 
			static_cast<int>( posX[player] - scale * 1.5f  ), 
			static_cast<int>( posY - scale * 1.5f ),
			scale * 3, scale * 2,
			0, 0, 
			iexSystem::ScreenWidth, iexSystem::ScreenHeight,
			shader2D, "add" );
	}

	//	�o�b�N�o�b�t�@�ɕ`��
	void	CoinUIEffect::RenderToBackBaffer( void )
	{
		//	�����_�[�^�[�Q�b�g�Ƀo�b�N�o�b�t�@���w��
		targetPar->RenderTarget( 0 );

		//	��ʃN���A
		camera->Activate();
		camera->Clear();

		//	�p�[�e�B�N���`��
		particle->Render();
	}

//---------------------------------------------------------------------------------------
//	���ݒ�
//---------------------------------------------------------------------------------------

	//	�G�t�F�N�g�`��t���O�ݒ�( �v���C���[�ԍ��Atrue�ŕ`�� )
	void	CoinUIEffect::SetEffectFlag( int playerNum, bool state )
	{
		coinFlag[playerNum] = true;
		particle->CoinGet( parPos, 0.5f );
	}

	//	���W�ݒ�
	void	CoinUIEffect::SetPos( int playerNum, int x, int y )
	{
		posX[playerNum] = x;
		posY = y;
	}

	//	�X�P�[���ݒ�
	void	CoinUIEffect::SetScale( int scale )
	{
		this->scale = scale;
	}