
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
//	Princess_CPUクラス
//
//*********************************************************************************

#define		QUICK_RATE	0.05f
#define		POWER_RATE	0.5f
#define		HYPER_RATE	0.005f

//-----------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------

	//	コンストラクタ
	Princess_CPU::Princess_CPU( void ) : Princess()
	{

	}

	//	デストラクタ
	Princess_CPU::~Princess_CPU( void )
	{

	}

//-----------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------

	void	Princess_CPU::ControlAI(void)
	{
		//　ハイパー
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

		//　基本行動
		if (coinManager->GetFreeCoinNum() >= CPU_SERCH_COIN_MIN)
		{
			AutoPickCoin();
		}

		//　順位別行動
		else
		{
			//　１位　→　逃げる
			switch (rank)
			{
			case 1:
				aiInfo.mode = AI_MODE_STATE::RUNAWAY;
				break;

			//　２位　→　１位を攻撃：逃げる（６：４）
			case 2:
				//　１位を追跡し、近付いたらクイック
				if (Random::PercentageRandom(0.6f))
				{
					//　ターゲット設定
					Vector3 target;
					int	rankTopPlayerNum;
					FOR(0, PLAYER_MAX)
					{
						if (characterManager->GetRank(value) != 1)	continue;
						rankTopPlayerNum = value;
						target = characterManager->GetPos(rankTopPlayerNum);
					}

					//　１位のキャラへ接近
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

					//　近ければクイック
					float dist = (target - pos).Length();
					if (dist <= 3.0f)
					{
						if (Random::PercentageRandom(QUICK_RATE))
							aiInfo.mode = AI_MODE_STATE::QUICKARTS;
					}

					//aiInfo.mode = AI_MODE_STATE::WAIT;

				}
				else	//　４０％
				{
					aiInfo.mode = AI_MODE_STATE::RUNAWAY;
				}
				break;

			//　３位・４位　→　１位か２位をランダムで攻撃
			case 3:
			case 4:

				//static int targetPlayerRank = (Random::PercentageRandom(0.65f)) ? 1 : 2;
				int targetPlayerRank = 1;
				//　ターゲット調整
				Vector3 target;
				FOR(0, PLAYER_MAX)
				{
					if (characterManager->GetRank(value) != targetPlayerRank)	continue;
					target = characterManager->GetPos(value);
				}

				//　１or２位のキャラへ接近
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

				//　近ければクイック
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
		//　程よくパワーアーツも使う
		BaseChara::AutoPickCoin();

		//　側にコインがいっぱいあればパワーで回収
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
		//　逃げつつ、たまーに攻撃する。パワー使える時は随時使用
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

		//　行動続行是非
		if (aiInfo.count_runaway <= 0)
		{
			aiInfo.count_runaway = 3 * SECOND;
			aiInfo.act_flag = false;
			aiInfo.mode = AI_MODE_STATE::MOVE;
		}
		else aiInfo.count_runaway--;

		//　稀にクイック
		if (Random::PercentageRandom(0.001f))
		{
			aiInfo.mode = AI_MODE_STATE::QUICKARTS;
		}
	}