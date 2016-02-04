
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"BulletManager.h"
#include	"CharacterManager.h"
#include	"Collision.h"
#include	"CoinManager.h"
#include	"GameManager.h"
#include	"Particle.h"
#include	"Random.h"
#include	"Sound.h"

#include	"Thief_CPU.h"

//*********************************************************************************
//
//	Thief_CPU�N���X
//
//*********************************************************************************

#define		QUICK_RATE	0.1f
#define		POWER_RATE	0.01f
#define		HYPER_RATE	0.05f

//-----------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Thief_CPU::Thief_CPU(void) : Thief()
	{
	
	}
	
	//	�f�X�g���N�^
	Thief_CPU::~Thief_CPU(void)
	{
	
	}

//-----------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------

	void	Thief_CPU::ControlAI(void)
	{
		//�@�n�C�p�[
		
		if (life >= 3)
		{
			int HavingCoinTotal = 0;
			FOR(0, PLAYER_MAX)
			{
				if (characterManager->GetPlayerNum(value) == playerNum) continue;
				HavingCoinTotal += gameManager->GetCoinNum(value);
			}
			if (HavingCoinTotal >= 100)
			{
				if (Random::PercentageRandom(HYPER_RATE))
				{
					//�@�^�[�Q�b�g����
					Vector3 target;
					FOR(0, PLAYER_MAX)
					{
						if (characterManager->GetRank(value) == 1)
						{
							target = characterManager->GetPos(value);
						}
					}
					//�@�p�x����
					const float adjustSpeed = 0.2f;
					if (aiInfo.AdjustingCount > 0)
					{
						AutoAngleAdjust(adjustSpeed, target);
					}
					else
					{
						aiInfo.mode = AI_MODE_STATE::HYPERARTS;

						aiInfo.AdjustingCount = 10;
					}
					aiInfo.AdjustingCount--;
				}
			}
		}
		if (gameManager->GetTimer() <= 10 * SECOND)
		{
			if (life >= 2)
			{
				if (Random::PercentageRandom(HYPER_RATE))
				{
					SubLife();
					aiInfo.mode = AI_MODE_STATE::HYPERARTS;
				}
			}
		}
		


		//�@��{�s��
		if (coinManager->GetFreeCoinNum() >= CPU_SERCH_COIN_MIN)
		{
			AutoPickCoin();
		}

		//�@���ʕʍs��
		else
		{
			//�@�P�ʁ@���@������
			switch (rank)
			{
			case 1:
				aiInfo.mode = AI_MODE_STATE::RUNAWAY;
				break;

				//�@�Q�ʁ@���@�P�ʂ��U���F������i�V�F�R�j
			case 2:
				if (Random::PercentageRandom(0.7f))
				{
					//�@�^�[�Q�b�g����
					Vector3 target;
					FOR(0, PLAYER_MAX)
					{
						if (characterManager->GetRank(value) != 1)	continue;
						target = characterManager->GetPos(value);
					}
					//�@�p�x����
					const float adjustSpeed = 0.2f;
					if (aiInfo.AdjustingCount > 0)
					{
						AutoAngleAdjust(adjustSpeed, target);
					}
					else
					{
						//�@�����_���ɍs������
						static int rand_Q_P_W = Random::GetInt(0, 9);
						if (isMiddle(rand_Q_P_W, 0, 2))			aiInfo.mode = AI_MODE_STATE::QUICKARTS;
						else if (isMiddle(rand_Q_P_W, 2, 6))	aiInfo.mode = AI_MODE_STATE::POWERARTS;
						else									aiInfo.mode = AI_MODE_STATE::WAIT;

						aiInfo.AdjustingCount = 10;
					}
					aiInfo.AdjustingCount--;
				}
				else
				{
					aiInfo.mode = AI_MODE_STATE::RUNAWAY;
				}
				break;

				//�@�R�ʁE�S�ʁ@���@�P�ʂ��Q�ʂ������_���ōU��
			case 3:
			case 4:
				int targetPlayerRank = (Random::PercentageRandom(0.65f)) ? 1 : 2;
				//�@�^�[�Q�b�g����
				Vector3 target;
				FOR(0, PLAYER_MAX)
				{
					if (characterManager->GetRank(value) != targetPlayerRank)	continue;
					target = characterManager->GetPos(value);
				}
				//�@�p�x����
				const float adjustSpeed = 0.2f;
				if (aiInfo.AdjustingCount > 0)
				{
					AutoAngleAdjust(adjustSpeed, target);
				}
				else
				{
					if (Random::PercentageRandom(QUICK_RATE))
					{
						aiInfo.mode = AI_MODE_STATE::QUICKARTS;
					}
					aiInfo.AdjustingCount = 10;
				}
				aiInfo.AdjustingCount--;

				break;
			}
		}

		AutoJump();
	}

	void	Thief_CPU::AutoPickCoin(void)
	{
		//�@�N�C�b�N�ƃp���[��K���ɑł��Ȃ���

		//-----------------------------------------
		//�@target�Ɍ�����1�`3������
		//-----------------------------------------
		Vector3			target = Vector3(0.0f, 0.0f, 0.0f);
		static	float	adjustSpeed = 0.2f;
		bool			existence = false;
		enum
		{
			AUTORUN_WALK = 0,
			AUTORUN_STAND
		};
		existence = coinManager->GetMinPos(target, pos);

		//	�Ώۂ����݂��Ă�����ΏۂɌ������đ���
		if (existence)
		{
			AutoAngleAdjust(adjustSpeed, target);
			if (!slip.state)
			{
				move.x = sinf(angle.y) * speed;
				move.z = cosf(angle.y) * speed;
			}
			else
			{
				if (move.Length() < speed)
				{
					move.x += sinf(angle.y) * slipInfo.speed;
					move.z += cosf(angle.y) * slipInfo.speed;
				}
			}

			//�@���悭�p���[
			if (Random::PercentageRandom(QUICK_RATE))
			{
				aiInfo.mode = (Random::PercentageRandom(0.8f)) ?
					AI_MODE_STATE::QUICKARTS :
					AI_MODE_STATE::POWERARTS;
			}
		}
	}

	void	Thief_CPU::RunAway(void)
	{
		aiInfo.act_flag = true;

		Vector3 vec_add(ZERO_VECTOR3);
		Vector3 target(ZERO_VECTOR3);


		for (int i = 0; i < PLAYER_MAX; i++)
		{
			Vector3 vec[4];

			//	�v���C���[�̔ԍ����擾
			int	p_num = characterManager->GetPlayerNum(i);

			//	�����Ɠ����ԍ���������X�L�b�v
			if (GetPlayerNum() == p_num)
			{
				vec[p_num] = Vector3(ZERO_VECTOR3);
				continue;
			}

			//	�ȉ��͎���VS����̏���
			vec[i] = characterManager->GetPos(characterManager->GetPlayerNum(i)) - pos;

			//�@����R�l�ւ̃x�N�g�������Z
			vec_add += vec[i];
		}

		//�@����������͑���R�l�ɑ΂��Ĕ��Ε���
		vec_add.Normalize();

		//�@�ǂɌ������ĂԂ�������Ȃ�t�����֒��i
		if (checkWall)
		{
			checkWallCount++;
		}
		if (aiInfo.runStraightCount <= 0)
		{
			target = pos - vec_add;
		}
		if (checkWallCount >= 10)
		{
			target *= -3.0f;
			aiInfo.runStraightCount = 1 * SECOND;
			checkWallCount = 0;
		}
		aiInfo.runStraightCount--;

		//�@�p�x����
		static	float	adjustSpeed = 0.2f;
		AutoAngleAdjust(adjustSpeed, target);

		//�@�ړ�
		if (!slip.state)
		{
			move.x = sinf(moveVec) * totalSpeed;
			move.z = cosf(moveVec) * totalSpeed;
		}
		else
		{
			if (move.Length() < totalSpeed)
			{
				move.x += sinf(moveVec) * slipInfo.speed;
				move.z += cosf(moveVec) * slipInfo.speed;
			}
		}

		//�@�p���[�A�[�c
		if (Random::PercentageRandom(0.01f))
		{
			aiInfo.mode = AI_MODE_STATE::QUICKARTS;
		}

		//�@�s�����s����
		if (aiInfo.count_runaway <= 0)
		{
			aiInfo.count_runaway = 3 * SECOND;
			aiInfo.act_flag = false;
			aiInfo.mode = AI_MODE_STATE::MOVE;
		}
		else aiInfo.count_runaway--;
	}

	bool	Thief_CPU::QuickArts( void )
	{
		static int time = 0;
		static	bool	initflag = false;
		if (!initflag)
		{
			sound->PlaySE(SE::KAITO_QUICK);
			initflag = true;
		}
		///Move();
		//	�s�񂩂���擾
		Vector3	up = GetUp();
		Vector3	right = GetRight();
		Vector3	p_pos = GetPos();
		SetMove(Vector3(0.0f, move.y, 0.0f));

		//���[�V�����A�g�f�i�I�X
		SetMotion(4);
		//if (obj->GetFrame() >= 237) obj->SetMotion(237);

		//	���ݒ�
		Vector3	vec[3] =
		{
			up * 5.0f + right * 3.0f,
			up * 5.0f,
			up * 5.0f + right * -3.0f
		};
		p_pos.y += 3.0f;
		float	 bulletSpeed = 0.5f;
		int playerNum = GetPlayerNum();


		{	//���������ɓ�������x�~�܂�p�^�[��
			//					//���[�V�����A�g�f�i�I�X(���傤�ǂ��������̃t���[����������e����)
			//if (time == 0 /* obj->GetFrame() == ���� */ )
			//{
			//	for (int i = 0; i < 3; i++)
			//	{
			//		m_BulletManager->Set(BULLET_TYPE::THIEF_01 , new Thief_Bullet01, p_pos, vec[i], bulletSpeed, playerNum);
			//	}
			//}
			//time++;

			////���[�V�����A�g�f�i�I�X(�I���̃��[�V������������I��)
			//if (obj->GetFrame() == 237)
			//{
			//	time = 0;
			//	return true;
			//}
		}

		{	//���������ɓ������~�܂�Ȃ��p�^�[��
			for (int i = 0; i < 3; i++)
			{
				m_BulletManager->Set(BULLET_TYPE::THIEF_01, new Thief_Bullet01, p_pos, vec[i], bulletSpeed, playerNum);
			}
			attackInfo.Interval = 60;
			initflag = false;
			return true;

		}
		return	false;
	}
