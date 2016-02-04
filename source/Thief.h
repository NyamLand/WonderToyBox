
#ifndef __THIEF_H__
#define	__THIEF_H__

//*********************************************************************************
//
//	Thiefクラス
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Thief : public		BaseChara
{
private:
	//後でマジックナンバー直す
	int HyperStep = 0;
	float HyperRate = 0;

	enum MOTION_DATA
	{
		POSTURE = 0,
		RUN,
		JUMP,
		GUARD,
		QUICK,
		POWER,
		HYPER,
		DAMAGE,
		DOWN,
		GUTS,
		HAPPY,
		SAD,
		ANGRY
	};
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


	//	更新・描画
	void	Render(iexShader* shader = nullptr, LPSTR technique = nullptr)override;

	//	動作関数
	bool	QuickArts(void)override;
	bool	PowerArts(void)override;
	bool	HyperArts(void)override;

	//	情報設定
	void	SetArmTransform(void);
	void	SetHandTransform(void);
	void	SetAttackParam(int attackKind)override;
};

//*********************************************************************************
#endif // !__MAID_H__
