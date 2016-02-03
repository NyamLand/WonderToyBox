
#include	"iextreme.h"
#include	"GlobalFunction.h"

#include	"CharacterManager.h"
#include	"Collision.h"
#include	"CoinManager.h"
#include	"GameManager.h"
#include	"Particle.h"
#include	"Random.h"
#include	"Sound.h"

#include	"Scavenger_CPU.h"

//*********************************************************************************
//
//	Scavenger_CPU�N���X
//
//*********************************************************************************

#define		POWERARTS_RATE	0.001f
//-----------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------


	//	�R���X�g���N�^
	Scavenger_CPU::Scavenger_CPU(void) : Scavenger()
	{
	
	}
	
	//	�f�X�g���N�^
	Scavenger_CPU::~Scavenger_CPU(void)
	{
	
	}

	bool	Scavenger_CPU::Initialize(int playerNum, Vector3 pos)
	{
		bool initFlag = Scavenger::Initialize(playerNum, pos);
		quickCount = 2 * SECOND;

		return initFlag;
	}

//-----------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------
	
	void	Scavenger_CPU::AutoPickCoin( void )
	{
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
			if (Random::PercentageRandom(POWERARTS_RATE))
			{
				aiInfo.mode = AI_MODE_STATE::POWERARTS;
			}
		}
	}

	//�@������i���悭�p���[�A�[�c�����j
	void	Scavenger_CPU::RunAway()
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
		if (Random::PercentageRandom(POWERARTS_RATE))
		{
			aiInfo.mode = AI_MODE_STATE::POWERARTS;
		}

		//�@�s�����s����
		if (aiInfo.count_runaway <= 0)
		{
			aiInfo.count_runaway = 3 * SECOND;
			aiInfo.act_flag = false;
			SetMode(MODE_STATE::MOVE);
		}
		else aiInfo.count_runaway--;
	}

	//�@AI�N�C�b�N
	bool	Scavenger_CPU::QuickArts()
	{
		//	���擾
		Matrix	mat = obj->TransMatrix;
		Vector3	front = Vector3(mat._31, mat._32, mat._33);
		front.Normalize();
		Vector3	up = Vector3(mat._21, mat._22, mat._23);
		up.Normalize();
		static	Vector3	p_pos = Vector3(0.0f, 0.0f, 0.0f);
		static	bool			initflag = false;
		bool			isEnd = false;
		bool			isHit = false;

		if (!initflag)
		{
			p_pos = pos + front * 2.0f + up * 2.0f;
			fireBallInterval = 0;
			attackInfo.t = 0.0f;
			attackInfo.r = 1.0f;
			sound->PlaySE(SE::MAJO_QUICK_START);
			initflag = true;
		}

		if (quickCount > 0)
		{
			if (fireBallState)
			{
				p_pos += front * 0.5f;
				attackInfo.pos = p_pos;
				attackInfo.r = 1.0f;

				//	���̃v���C���[�ɓ��������甭��
				FOR(0, PLAYER_MAX)
				{
					if (value == playerNum)	continue;
					Vector3	p2_pos = characterManager->GetPos(value);
					if (Collision::CapsuleVSSphere(p2_pos, p2_pos + Vector3(0.0f, 3.0f, 0.0f), 2.0f, attackInfo.pos, attackInfo.r))
					{
						sound->PlaySE(SE::MAJO_QUICK_BOMB);
						fireBallState = false;
					}
				}
				particle->FireBall(attackInfo.pos, attackInfo.r * 0.15f, Vector3(0.7f, 0.1f, 0.1f));
			}
			
			quickCount--;
		}
		else
		{
			quickCount = Random::GetInt(5, 60);
			fireBallState = false;
			sound->PlaySE(SE::MAJO_QUICK_BOMB);
		}
			


		if (!fireBallState)
		{
			attackInfo.t += 0.03f;
			if (attackInfo.t >= 1.0f)	attackInfo.t = 1.0f;
			isEnd = Lerp(attackInfo.r, 1.0f, 5.0f, attackInfo.t);
			particle->BombFireBall(attackInfo.pos, attackInfo.r * 0.04f, Vector3(0.5f, 0.1f, 0.1f));

			if (isEnd)
			{
				initflag = false;
				fireBallState = true;
				return	true;
			}
		}


		return	false;

	}


	// ���[�h�J�ځiaiInfo.mode == MOVE�j
	void	Scavenger_CPU::ControlAI(void)
	{
		//�@�n�C�p�[
		if (coinManager->GetFreeCoinNum() >= 40)
		{
			if (life >= 3)
			{
				if (Random::PercentageRandom(0.01f))
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
				RunAway();
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
					RunAway();
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
					//�@�����_���ɍs������
					static int rand_Q_P_W = Random::GetInt(0, 9);
					if (isMiddle(rand_Q_P_W, 0, 2))			aiInfo.mode = AI_MODE_STATE::QUICKARTS;
					else if (isMiddle(rand_Q_P_W, 2, 6))	aiInfo.mode = AI_MODE_STATE::POWERARTS;
					else									aiInfo.mode = AI_MODE_STATE::WAIT;

					aiInfo.AdjustingCount = 10;
				}
				aiInfo.AdjustingCount--;

				break;
			}
		}

		AutoJump();
	}