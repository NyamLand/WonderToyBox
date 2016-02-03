
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
//	Scavenger_CPUクラス
//
//*********************************************************************************

#define		POWERARTS_RATE	0.001f
//-----------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------


	//	コンストラクタ
	Scavenger_CPU::Scavenger_CPU(void) : Scavenger()
	{
	
	}
	
	//	デストラクタ
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
//	動作関数
//-----------------------------------------------------------------------------
	
	void	Scavenger_CPU::AutoPickCoin( void )
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
			if (Random::PercentageRandom(POWERARTS_RATE))
			{
				aiInfo.mode = AI_MODE_STATE::POWERARTS;
			}
		}
	}

	//　逃げる（程よくパワーアーツ発動）
	void	Scavenger_CPU::RunAway()
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
		if (Random::PercentageRandom(POWERARTS_RATE))
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

	//　AIクイック
	bool	Scavenger_CPU::QuickArts()
	{
		//	情報取得
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

				//	他のプレイヤーに当たったら発火
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


	// モード遷移（aiInfo.mode == MOVE）
	void	Scavenger_CPU::ControlAI(void)
	{
		//　ハイパー
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
				RunAway();
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
		}

		AutoJump();
	}