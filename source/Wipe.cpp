#include	"iextreme.h"
#include	"system\System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Camera.h"
#include	"CharacterManager.h"
#include	"Particle.h"

#include	"Wipe.h"


//*****************************************************************************************************************************
//
//	������
//
//*****************************************************************************************************************************

	//	�R���X�g���N�^
	PlayerWipe::PlayerWipe(void) : ASPECT( 0 )
	{
	
	}
	
	//	������
	bool	PlayerWipe::Initialize( void )
	{
		//***********************
		//	��`
		//***********************

		//	�J�����̎���p
		ASPECT = D3DX_PI / 4.0f;

		//***********************

		//	�����_�[�^�[�Q�b�g�p
		FOR(0, PLAYER_MAX)
		{
			//	�J����������
			playerView[value]	= make_unique<Camera>();
			playerView[value]->SetPos(Vector3(0.0f, 20.0f, -10.0f));

			//	�ϐ�������
			wipe[value]			= make_unique<iex2DObj>( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET );
			color[value]		= Vector3( 0.0f, 1.0f, 0.0f );
		}
		return true;
	}


	//	�f�X�g���N�^
	PlayerWipe::~PlayerWipe( void )
	{

	}


//*****************************************************************************************************************************
//
//		�X�V
//
//*****************************************************************************************************************************
	
	//	�X�V
	void	PlayerWipe::Update( void )
	{
		FOR(0, PLAYER_MAX){
			//	�J�����X�V
			playerView[value]->Update(VIEW_MODE::INDIVIDUAL, characterManager->GetPos(value));
			playerView[value]->SetPos(characterManager->GetPos(value) + Vector3(0.0f, 20.0f, -10.0f));
		}
		//	���C�v���Z�b�g
		Set();
	}

	//	���C�v�|�W�V�����A�J���[��ݒ�
	void	PlayerWipe::Set( void )
	{
		//	P1
		pos[0].x = 0;
		pos[0].y = 0;
		color[0] = Vector3(1.0f, 0.0f, 0.0f);

		//	P2
		pos[1].x = iexSystem::ScreenWidth - WIPE_WIDTH;
		pos[1].y = 0;
		color[1] = Vector3(0.0f, 0.0f, 1.0f);


		//	P3
		pos[2].x = 0;
		pos[2].y = iexSystem::ScreenHeight - WIPE_HEIGHT;
		color[2] = Vector3(1.0f, 1.0f, 0.0f);
	
		//	P4
		pos[3].x = iexSystem::ScreenWidth - WIPE_WIDTH;
		pos[3].y = iexSystem::ScreenHeight - WIPE_HEIGHT;
		color[3] = Vector3(0.0f, 1.0f, 0.0f);
		
	}

	//	���C�v�`�攻��
	void	PlayerWipe::Check( float angle, int num )
	{
		FOR(0, PLAYER_MAX)
		{
			//	����p���ɂ��̃v���C���[�����邩
			Vector3	stringPos;
			WorldToClient(characterManager->GetPos(num), stringPos, matView* matProjection);

			if (stringPos.x < 0	||
				stringPos.x > iexSystem::ScreenWidth ||
				stringPos.y < 0 ||
				stringPos.y > iexSystem::ScreenHeight)
			{
				check[num] = true;
			}

			else
			{
				check[num] = false;
			}
		}
	}

//*****************************************************************************************************************************
//
//		�`��֘A
//
//*****************************************************************************************************************************

	//	�`��
	void	PlayerWipe::Render( void )
	{
		FOR(0, PLAYER_MAX)
		{
			//	���W�E�X�P�[���ݒ�
			int w = WIPE_WIDTH;
			int h = WIPE_HEIGHT;
			float	wipe_size = 100.0f;
			float	frame_size = 10.0f;

			//	�V�F�[�_�[�փZ�b�g
			shader2D->SetValue("center_posX", pos[value].x);
			shader2D->SetValue("center_posY", pos[value].y);
			shader2D->SetValue("picture_width", w);
			shader2D->SetValue("picture_height", h);
			shader2D->SetValue("wipe_size", wipe_size);
			shader2D->SetValue("frame_size", frame_size);
			shader2D->SetValue("frame_color", color[value]);

			//	���C�v�`��
			if (check[value]){
				wipe[value]->Render(pos[value].x, pos[value].y, w, h, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shader2D, "WipeEffect");
			}
		}
	}




	//	�����_�[�^�[�Q�b�g�p
	void	PlayerWipe::RenderTarget( int num )
	{
		//	�����_�[�^�[�Q�b�g��؂�ւ�
		wipe[num]->RenderTarget(0);

		//	��ʃN���A
		playerView[num]->Activate();
		playerView[num]->Clear();

	}

//------------------------------------------------------------------------------------
//	���擾
//------------------------------------------------------------------------------------
	
	//	���̎擾
	PlayerWipe*		PlayerWipe::GetInstance()
	{
		static	PlayerWipe out;
		return &out;
	}

