
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"PlayerNumUI.h"

//***********************************************************************
//
//	PlayerNumUI�N���X
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	�O���[�o��
//------------------------------------------------------------------------------------
#define	PLAYER_NUM_DIST	50

//------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	PlayerNumUI::PlayerNumUI( void ) : orgPlayerNum( nullptr )
	{
		FOR( 0, PLAYER_MAX )
		{
			playerNumImage[value].obj = nullptr;
		}
	}

	//	�f�X�g���N�^
	PlayerNumUI::~PlayerNumUI( void )
	{
		Release();
	}

	//	������
	bool	PlayerNumUI::Initialize( void )
	{
		Load();

		FOR( 0, PLAYER_MAX )
		{
			//	�\���̏�����
			ImageInitialize( playerNumImage[value], 
				0, 0, 40, 40, 
				128 * ( value % 2 ),
				128 * ( value / 2 ), 
				128,	128 );

			//	�摜�ݒ�
			playerNumImage[value].obj = orgPlayerNum;

			if ( playerNumImage[value].obj == nullptr )	return	false;
		}

		return	true;
	}

	//	���
	void	PlayerNumUI::Release( void )
	{
		SafeDelete( orgPlayerNum );
	}

	//	�ǂݍ���
	void	PlayerNumUI::Load( void )
	{
		orgPlayerNum = new iex2DObj( "DATA/UI/playerNum.png" );
	}

//------------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------------

	//	�X�V
	void	PlayerNumUI::Update( void )
	{
		Vector3	p_Pos;
		Vector3	p_Up;
		Vector3	crownPos;
		Vector3	out;

		FOR( 0, PLAYER_MAX )
		{
			//	�\�����W�Z�o
			p_Pos = characterManager->GetPos( value );
			p_Up = characterManager->GetUp( value );
			crownPos = p_Pos + p_Up * 5.0f;
			WorldToClient( crownPos, out, matView *matProjection );

			//	�`��ʒu�ݒ�
			playerNumImage[value].x = static_cast<int>( out.x );
			playerNumImage[value].y = static_cast<int>( out.y - PLAYER_NUM_DIST );
		}
	}

	//	�`��
	void	PlayerNumUI::Render( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( playerNumImage[value], 
				playerNumImage[value].sx, playerNumImage[value].sy, 
				playerNumImage[value].sw, playerNumImage[value].sh, 
				IMAGE_MODE::NORMAL );
		}
	}

//------------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//	���ݒ�
//------------------------------------------------------------------------------------

	//	���W�ݒ�
	void	PlayerNumUI::SetPos( int playerNum, int posX, int posY )
	{

	}

//------------------------------------------------------------------------------------
//	���擾
//------------------------------------------------------------------------------------


