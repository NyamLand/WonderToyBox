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

	//	������
	bool	PlayerWipe::Initialize( void )
	{
		WIPE_LEFT	= ( float )SPACE;
		WIPE_RIGHT	= ( float )iexSystem::ScreenWidth - WIPE_WIDTH - SPACE;
		WIPE_UP		= ( float )SPACE;
		WIPE_DOWN	= ( float )iexSystem::ScreenHeight - WIPE_HEIGHT - SPACE;
		WIPE_SIZE	= 100.0f;
		FRAME_SIZE	= 10.0f;
		LEN_MAX		= 0.0f;

		//	�����_�[�^�[�Q�b�g�p
		FOR(0, PLAYER_MAX)
		{
			//	�J����������
			playerView[value]	= make_unique<Camera>();
			playerView[value]->SetPos(Vector3(0.0f, 20.0f, -10.0f));

			//	�ϐ�������
			wipe[value]			= make_unique<iex2DObj>( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET );
			color[value]		= Vector3( 0.0f, 1.0f, 0.0f );
			pos[value]			= Vector3( 0.0f, 0.0f, 0.0f );
			arrow_pos[value]	= Vector3( 0.0f, 0.0f, 0.0f );
			arrow_angle[value]	= 0.0f;
		}

		//	�摜�Z�b�g
		arrow.obj = new iex2DObj("DATA/Effect/w-arrow.png");
		ImageInitialize(arrow, 0, 0, SPACE, SPACE, 0, 0, 256, 256);

		//	���C�v���Z�b�g
		Set();

		return true;
	}

//*****************************************************************************************************************************
//
//		�X�V
//
//*****************************************************************************************************************************
	
	//	�X�V
	void	PlayerWipe::Update( Vector3 target )
	{
		//	��ʊO����
		Check();

		FOR(0, PLAYER_MAX){
			if (!check[value])	continue;

			//	���C�v�ړ�
			Move(value, target);

			//	���
			Arrow(value, target);

			//	�J�����X�V
			playerView[value]->Update(VIEW_MODE::INDIVIDUAL, characterManager->GetPos(value));
			playerView[value]->SetPos(characterManager->GetPos(value) + Vector3(0.0f, 15.0f, -10.0f));
		}
		
	}

	//	���C�v�|�W�V�����A�J���[��ݒ�
	void	PlayerWipe::Set( void )
	{
		//	P1
		pos[0].x = WIPE_LEFT;
		pos[0].y = WIPE_UP;
		color[0] = Vector3(1.0f, 0.0f, 0.0f);

		//	P2
		pos[1].x = WIPE_RIGHT;
		pos[1].y = WIPE_UP;
		color[1] = Vector3(0.0f, 0.0f, 1.0f);


		//	P3
		pos[2].x = WIPE_LEFT;
		pos[2].y = WIPE_DOWN;
		color[2] = Vector3(1.0f, 1.0f, 0.0f);
	
		//	P4
		pos[3].x = WIPE_RIGHT;
		pos[3].y = WIPE_DOWN;
		color[3] = Vector3(0.0f, 1.0f, 0.0f);
		
	}

	//	���C�v�`�攻��
	void	PlayerWipe::Check( void )
	{
		FOR(0, PLAYER_MAX)
		{
			//	����p���ɂ��̃v���C���[�����邩
			Vector3	stringPos;
			
			//	��ʊO����
			if (!WorldToClient(characterManager->GetPos(value), stringPos, matView* matProjection))
			{
				check[value] = true;
			}

			else
			{
				check[value] = false;
			}
		}
	}

	//	�v���C���[�̕����֖��
	void	PlayerWipe::Arrow( int num, Vector3 target )
	{
		Vector3	stringPos;
		WorldToClient(LengthChecker(num, target), stringPos, matView* matProjection);
	
		//	���C�v���S
		Vector3 center;
		center.x = pos[num].x + WIPE_WIDTH / 2;
		center.y = pos[num].y + WIPE_HEIGHT / 2;
		
		//	���C�v����v���C���[�ւ̖@���i�N���C�A���g�|�W�V�����j
		Vector3 normal = stringPos - center;
		float angle = atan2f(normal.y, normal.x);

		//	���C�v���S�����󒆐S�܂ł̔��a
		float r = (( float )arrow.w / 2) + (WIPE_WIDTH / 2);

		//	
		arrow_pos[num].x = center.x + (cosf(angle) * r);
		arrow_pos[num].y = center.y + (sinf(angle) * r);


		//	���C�v����v���C���[�ւ̊p�x
		arrow_angle[num] = angle;


	}

	//	���C�v�̈ړ�
	void	PlayerWipe::Move( int num, Vector3 target )
	{
		Vector3	stringPos;
		WorldToClient(LengthChecker(num,target), stringPos, matView* matProjection);

		//Vector3 targetPos;
		//WorldToClient(target, targetPos, matView* matProjection);


		//	���S���獶
		if (stringPos.x < iexSystem::ScreenWidth / 2)
		{
			pos[num].x = WIPE_LEFT;
		}
		//	���S����E
		else
		{
			pos[num].x = WIPE_RIGHT;
		}

		//	�������v�Z
		Vector3 v1, v2;
		Vector3 start = Vector3(pos[num].x, WIPE_UP, 0);
		Vector3 end = start;
		end.y = WIPE_DOWN;
		v1 = stringPos - start;
		v2 = end  - start;
		v2.Normalize();

		float dot = Vector3Dot(v1, v2);
		if (dot < 0) dot = -dot;

		pos[num] = start + (v2 * dot);

		if (pos[num].y > WIPE_DOWN)	pos[num].y = WIPE_DOWN;
		if (pos[num].y < WIPE_UP)	pos[num].y = WIPE_UP;

		SufferChecker(num);

	}

	//	���C�v���m�̔����Ȃ���
	void	PlayerWipe::SufferChecker( int num )
	{
		FOR(0, PLAYER_MAX)
		{
			//	�`�悵�ĂȂ����́A�������m�̓X�L�b�v
			if (!check[value] || value == num)	continue;

			//	�����ʒu�̏ꍇ�͔ԍ��ŏ㉺������
			if (pos[num] == pos[value])
			{
				//	��
				if (pos[num].y == WIPE_UP)
				{
					if (num < value)
					{
						pos[value].y += (WIPE_SIZE + FRAME_SIZE) * 2;
					}
					else{
						pos[num].y += (WIPE_SIZE + FRAME_SIZE) * 2;
					}

				}

				//	��
				else if (pos[num].y == WIPE_DOWN)
				{
					if (num < value)
					{
						pos[num].y -= (WIPE_SIZE + FRAME_SIZE) * 2;
					}
					else{
						pos[value].y -= (WIPE_SIZE + FRAME_SIZE) * 2;
					}

				}
			}


			//	��������
			float len;
			Vector3 v1;
			v1 = pos[num] - pos[value];
			len = v1.Length();
			
			if (WIPE_SIZE + FRAME_SIZE > len)
			{
				pos[num] = pos[value] + v1 * (WIPE_SIZE + FRAME_SIZE);
			}


		}

	}


	//	�����̏C������
	Vector3	PlayerWipe::LengthChecker( int num, Vector3 target )
	{
		//	�J�����ƃ^�[�Q�b�g�̎ˉe�������
		Vector3 v1;
		v1 = Vector3(mainView->GetSpringPos().x, 0, mainView->GetSpringPos().z) - target;
		LEN_MAX = v1.Length();

		//	�v���C���[�ƃ^�[�Q�b�g�̋��������
		v1 = characterManager->GetPos(num) - target;
		
		Vector3 test;
		//	���������ꂷ���Ă��Ȃ�������
		if (LEN_MAX < v1.Length())
		{
			v1.Normalize();
			return target + v1 * LEN_MAX;
		}

		return characterManager->GetPos(num);
	}

	//�`��̃`�F�b�N���I�t
	void	PlayerWipe::CheckOff( void )
	{
		FOR(0, PLAYER_MAX)
		{
			check[value] = false;
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
				wipe[value]->Render(( int )pos[value].x, ( int )pos[value].y, w, h, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shader2D, "WipeEffect");

				arrow.angle = arrow_angle[value] + PI /2;		arrow.p = ::GetPoint(( int )arrow_pos[value].x, ( int )arrow_pos[value].y);			arrow.color = color[value];
				RenderImage(arrow, arrow.sx, arrow.sy, arrow.sw, arrow.sh, IMAGE_MODE::ADOPTPARAM, ( int )arrow_pos[value].x, ( int )arrow_pos[value].y);
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

