
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
//	Pirate_CPUクラス
//
//*********************************************************************************

#define		QUICK_RATE	0.01f
#define		POWER_RATE	0.01f
#define		HYPER_RATE	0.01f

namespace OFFENSIVE_POWER
{
	enum
	{
		//クイックとハイパーはバレット側で処理
		QUICK = 1,
		POWER = 1,
		HYPER = 1,
	};
}

namespace DROP_POWER
{
	enum
	{
		//クイックとハイパーはバレット側で処理
		QUICK = 0,
		POWER = 5,
		HYPER = 15,
	};
}

//-----------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------

	//	コンストラクタ
	Pirate_CPU::Pirate_CPU(void) : Pirate()
	{
	
	}
	
	//	デストラクタ
	Pirate_CPU::~Pirate_CPU(void)
	{
	
	}

//-----------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------
	
	//　モード遷移
	void	Pirate_CPU::ControlAI(void)
	{
		//-----------------------------------------
		//　ハイパー
		//-----------------------------------------

		if (life >= 3)
		{
			bool hyperFlag = false;
			FOR(0, PLAYER_MAX)
			{
				//　自分は無視
				if (value == playerNum)	continue;

				//　体力２以下が１人でも存在
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
		//　基本行動
		//-----------------------------------------
		if (coinManager->GetFreeCoinNum() >= CPU_SERCH_COIN_MIN)
		{
			AutoPickCoin();
		}

		//-----------------------------------------
		//　順位別行動
		//-----------------------------------------
		switch (rank)
		{
			//　１位　→　逃げる
		case 1:
			aiInfo.mode = AI_MODE_STATE::RUNAWAY;
			break;

			//　２位　→　１位を攻撃：逃げる（７：３）
		case 2:
			if (Random::PercentageRandom(0.7f))
			{
				//　ターゲット調整
				Vector3 target;
				FOR(0, PLAYER_MAX)
				{
					if (characterManager->GetRank(value) != 1)	continue;
					target = characterManager->GetPos(value);
				}
				//　角度調整
				const float adjustSpeed = 0.2f;
				if (aiInfo.AdjustingCount > 0)
				{
					AutoAngleAdjust(adjustSpeed, target);
				}
				else
				{
					//　ランダムに行動分岐
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

			//　３位・４位　→　１位か２位をランダムで攻撃
		case 3:
		case 4:
			int targetPlayerRank = (Random::PercentageRandom(0.65f)) ? 1 : 2;
			//　ターゲット調整
			Vector3 target;
			FOR(0, PLAYER_MAX)
			{
				if (characterManager->GetRank(value) != targetPlayerRank)	continue;
				target = characterManager->GetPos(value);
			}
			//　角度調整
			const float adjustSpeed = 0.2f;
			if (aiInfo.AdjustingCount > 0)
			{
				AutoAngleAdjust(adjustSpeed, target);
			}
			else
			{
				//　ランダムに行動分岐
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

	//　自動コイン回収
	void	Pirate_CPU::AutoPickCoin(void)
	{
		//-----------------------------------------
		//　targetに向けて1～3歩歩く
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

		//	対象が存在していたら対象に向かって走る
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

			//　程よくパワー
			if (Random::PercentageRandom(POWER_RATE))
			{
				aiInfo.mode = AI_MODE_STATE::POWERARTS;
			}
		}
	}

	//　逃げる
	void	Pirate_CPU::RunAway(void)
	{
		aiInfo.act_flag = true;

		Vector3 vec_add(ZERO_VECTOR3);
		Vector3 target(ZERO_VECTOR3);


		for (int i = 0; i < PLAYER_MAX; i++)
		{
			Vector3 vec[4];

			//	プレイヤーの番号を取得
			int	p_num = characterManager->GetPlayerNum(i);

			//	自分と同じ番号だったらスキップ
			if (GetPlayerNum() == p_num)
			{
				vec[p_num] = Vector3(ZERO_VECTOR3);
				continue;
			}

			//	以下は自分VS相手の処理
			vec[i] = characterManager->GetPos(characterManager->GetPlayerNum(i)) - pos;

			//　相手３人へのベクトルを合算
			vec_add += vec[i];
		}

		//　逃げる方向は相手３人に対して反対方向
		vec_add.Normalize();

		//　壁に向かってぶち当たるなら逆方向へ直進
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

		//　角度調整
		static	float	adjustSpeed = 0.2f;
		AutoAngleAdjust(adjustSpeed, target);

		//　移動
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

		//　パワーアーツ
		if (Random::PercentageRandom(POWER_RATE))
		{
			aiInfo.mode = AI_MODE_STATE::POWERARTS;
		}

		//　行動続行是非
		if (aiInfo.count_runaway <= 0)
		{
			aiInfo.count_runaway = 3 * SECOND;
			aiInfo.act_flag = false;
			SetMode(MODE_STATE::MOVE);
		}
		else aiInfo.count_runaway--;
	}

	//　ＣＰＵパワー
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

		//	行列から前方取得
		SetMove(Vector3(0.0f, 0.0f, 0.0f));
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		Vector3	p_pos = GetPos() + front * 2.0f;

		//	あたり判定のパラメータを与える
		attackInfo.r = 2.0f;
		attackInfo.bottom = p_pos;
		attackInfo.top = attackInfo.bottom + Vector3(0.0f, 3.0f, 0.0f);

		attackInfo.bottom = p_pos - right * 2.0f;
		attackInfo.top = attackInfo.bottom + right * 4.0f;


		static	float adjustSpeed = 0.02f;
		AutoAngleAdjust(adjustSpeed, front*2.0f + right*Random::GetFloat(-1, 1));

		//	パラメータ加算
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