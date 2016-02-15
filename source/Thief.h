
#ifndef __THIEF_H__
#define	__THIEF_H__

//*********************************************************************************
//
//	Thiefクラス
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

namespace THIEF
{
	//モーション番号
	enum MOTION_DATA
	{
		POSTURE = 0,
		RUN,
		JUMP,
		GUARD,
		QUICKARTS,
		POWERARTS,
		HYPERARTS,
		DAMAGE,
		DOWN,
		GUTS,
		HAPPY,
		SAD,
		ANGRY
	};

	//モーション固定、切り替えに使うフレーム
	namespace MOTION_FRAME
	{
		const int POWERARTS_END = 277;
		const int HYPERARTS_ATTACKSTART = 339;
		const int HYPERARTS_ATTACKEND = 399;
		const int DAMAGE_END = 408;
	}
}

//	class
class Thief : public		BaseChara
{
private:
	//後でマジックナンバー直す
	int HyperStep = 0;
	float HyperRate = 0;


	const float DEFAULT_ABSORB_LENGTH = 3.0f;

private:
	iexMesh*	arm;	//ハイパーアーツ攻撃用
	iexMesh*	hand;	//ハイパーアーツ攻撃用

	bool		armRenderflag;

	int		stayTime;	//技関連の硬直経過時間
	int		artsTimer;
	float	absorb_length;		//コインを吸い込める距離
//	void	MotionManagement(int motion)override;

public:
	//	初期化・解放
	Thief(void);
	~Thief(void);
	bool	Initialize(int playerNum, Vector3 pos)override;
	void	AttackParamInitialize( void )override;


	//	更新・描画
	void	Render(iexShader* shader = nullptr, LPSTR technique = nullptr)override;

	//	動作関数
	bool	QuickArts(void)override;
	bool	PowerArts(void)override;
	bool	HyperArts(void)override;

	//	情報設定
	void	SetAttackParam(int attackKind)override;
};

//*********************************************************************************
#endif // !__MAID_H__
