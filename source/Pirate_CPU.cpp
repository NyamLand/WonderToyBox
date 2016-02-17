
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"CoinManager.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Random.h"
#include	"Sound.h"

#include	"Pirate_CPU.h"

//*********************************************************************************
//
//	Pirate_CPU�N���X
//
//*********************************************************************************

#define		QUICK_RATE	0.01f
#define		POWER_RATE	0.01f
#define		HYPER_RATE	0.01f

namespace OFFENSIVE_POWER
{
	enum
	{
		//�N�C�b�N�ƃn�C�p�[�̓o���b�g���ŏ���
		QUICK = 1,
		POWER = 1,
		HYPER = 1,
	};
}

namespace DROP_POWER
{
	enum
	{
		//�N�C�b�N�ƃn�C�p�[�̓o���b�g���ŏ���
		QUICK = 0,
		POWER = 5,
		HYPER = 15,
	};
}

//-----------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Pirate_CPU::Pirate_CPU(void) : Pirate()
	{
	
	}
	
	//	�f�X�g���N�^
	Pirate_CPU::~Pirate_CPU(void)
	{
	
	}

//-----------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------
	
	//�@���[�h�J��
	void	Pirate_CPU::ControlAI(void)
	{
		//-----------------------------------------
		//�@�n�C�p�[
		//-----------------------------------------

		if (life >= 3)
		{
			bool hyperFlag = false;
			FOR(0, PLAYER_MAX)
			{
				//�@�����͖���
				if (value == playerNum)	continue;

				//�@�̗͂Q�ȉ����P�l�ł�����
				if (characterManager->GetLife(value) <= 5)
				{
					hyperFlag = true;
				}
			}

			if (coinManager->GetFreeCoinNum() >= 40 || hyperFlag == true)
			{
				if (Random::PercentageRandom(HYPER_RATE))
				{
					SubLife();
					aiInfo.mode = AI_MODE_STATE::HYPERARTS;
				}
			}
		}

		//-----------------------------------------
		//�@��{�s��
		//-----------------------------------------
		if (coinManager->GetFreeCoinNum() >= CPU_SERCH_COIN_MIN)
		{
			AutoPickCoin();
		}

		//-----------------------------------------
		//�@���ʕʍs��
		//-----------------------------------------
		switch (rank)
		{
			//�@�P�ʁ@���@������
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
					if (isMiddle(rand_Q_P_W, 0, 3))			aiInfo.mode = AI_MODE_STATE::QUICKARTS;
					else if (isMiddle(rand_Q_P_W, 4, 6))	aiInfo.mode = AI_MODE_STATE::POWERARTS;
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

		AutoJump();
	}

	//�@�����R�C�����
	void	Pirate_CPU::AutoPickCoin(void)
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
			if (Random::PercentageRandom(POWER_RATE))
			{
				aiInfo.mode = AI_MODE_STATE::POWERARTS;
			}
		}
	}

	//�@������
	void	Pirate_CPU::RunAway(void)
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
		if (Random::PercentageRandom(POWER_RATE))
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

	//�@�b�o�t�p���[
	bool	Pirate_CPU::PowerArts(void)
	{
		attackInfo.power = OFFENSIVE_POWER::POWER;
		attackInfo.dropPower = DROP_POWER::POWER;
		//power = POWER;
		if (!initflag)
		{
			sound->PlaySE(SE::PIRATE_POWER);
			initflag = true;
		}

		if (attackInfo.t == 0) SetMotion(PIRATE::MOTION_DATA::POWER_START);

		float run_speed = 0.5f;
		SetUnrivaled(false);

		//	�s�񂩂�O���擾
		SetMove(Vector3(0.0f, 0.0f, 0.0f));
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		Vector3	p_pos = GetPos() + front * 2.0f;

		//	�����蔻��̃p�����[�^��^����
		attackInfo.r = 2.0f;
		attackInfo.bottom = p_pos;
		attackInfo.top = attackInfo.bottom + Vector3(0.0f, 3.0f, 0.0f);

		attackInfo.bottom = p_pos - right * 2.0f;
		attackInfo.top = attackInfo.bottom + right * 4.0f;


		static	float adjustSpeed = 0.02f;
		AutoAngleAdjust(adjustSpeed, front*2.0f + right*Random::GetFloat(-1, 1));

		//	�p�����[�^���Z
		attackInfo.t += 0.015f;

		if (attackInfo.t < 1.0f) move = front * run_speed;
		else SetMotion(PIRATE::MOTION_DATA::POWER_END);

		if (obj->GetFrame() == PIRATE::MOTION_FRAME::POWERARTS_END)
		{
			initflag = false;
			return	true;
		}
		return	false;
	}