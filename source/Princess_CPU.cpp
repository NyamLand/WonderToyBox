
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"CoinManager.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Random.h"

#include	"Princess_CPU.h"

//*********************************************************************************
//
//	Princess_CPU�N���X
//
//*********************************************************************************

#define		QUICK_RATE	0.05f
#define		POWER_RATE	0.5f
#define		HYPER_RATE	0.005f

//-----------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Princess_CPU::Princess_CPU( void ) : Princess()
	{

	}

	//	�f�X�g���N�^
	Princess_CPU::~Princess_CPU( void )
	{

	}

//-----------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------

	void	Princess_CPU::ControlAI(void)
	{
		//�@�n�C�p�[
		if (life >= 3)
		{
			if (coinManager->GetFreeCoinNum() >= 40)
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

			//�@�Q�ʁ@���@�P�ʂ��U���F������i�U�F�S�j
			case 2:
				//�@�P�ʂ�ǐՂ��A�ߕt������N�C�b�N
				if (Random::PercentageRandom(0.6f))
				{
					//�@�^�[�Q�b�g�ݒ�
					Vector3 target;
					int	rankTopPlayerNum;
					FOR(0, PLAYER_MAX)
					{
						if (characterManager->GetRank(value) != 1)	continue;
						rankTopPlayerNum = value;
						target = characterManager->GetPos(rankTopPlayerNum);
					}

					//�@�P�ʂ̃L�����֐ڋ�
					const float adjustSpeed = 0.2f;
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

					//�@�߂���΃N�C�b�N
					float dist = (target - pos).Length();
					if (dist <= 3.0f)
					{
						if (Random::PercentageRandom(QUICK_RATE))
							aiInfo.mode = AI_MODE_STATE::QUICKARTS;
					}

					//aiInfo.mode = AI_MODE_STATE::WAIT;

				}
				else	//�@�S�O��
				{
					aiInfo.mode = AI_MODE_STATE::RUNAWAY;
				}
				break;

			//�@�R�ʁE�S�ʁ@���@�P�ʂ��Q�ʂ������_���ōU��
			case 3:
			case 4:

				//static int targetPlayerRank = (Random::PercentageRandom(0.65f)) ? 1 : 2;
				int targetPlayerRank = 1;
				//�@�^�[�Q�b�g����
				Vector3 target;
				FOR(0, PLAYER_MAX)
				{
					if (characterManager->GetRank(value) != targetPlayerRank)	continue;
					target = characterManager->GetPos(value);
				}

				//�@�Por�Q�ʂ̃L�����֐ڋ�
				const float adjustSpeed = 0.2f;
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

				//�@�߂���΃N�C�b�N
				float dist = (target - pos).Length();
				if (dist <= 3.0f)
				{
					if (Random::PercentageRandom(QUICK_RATE))
						aiInfo.mode = AI_MODE_STATE::QUICKARTS;
				}

				break;
			}
		}

		AutoJump();
	}

	void	Princess_CPU::AutoPickCoin(void)
	{
		//�@���悭�p���[�A�[�c���g��
		BaseChara::AutoPickCoin();

		//�@���ɃR�C���������ς�����΃p���[�ŉ��
		if (coinManager->GetNearCoinNum(3.0f, pos) >= 5)
		{
			if (Random::PercentageRandom(POWER_RATE))
			{
				aiInfo.mode = AI_MODE_STATE::POWERARTS;
			}
		}
	}

	void	Princess_CPU::RunAway(void)
	{
		//�@�����A���܁[�ɍU������B�p���[�g���鎞�͐����g�p
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

		//�@�s�����s����
		if (aiInfo.count_runaway <= 0)
		{
			aiInfo.count_runaway = 3 * SECOND;
			aiInfo.act_flag = false;
			aiInfo.mode = AI_MODE_STATE::MOVE;
		}
		else aiInfo.count_runaway--;

		//�@�H�ɃN�C�b�N
		if (Random::PercentageRandom(0.001f))
		{
			aiInfo.mode = AI_MODE_STATE::QUICKARTS;
		}
	}