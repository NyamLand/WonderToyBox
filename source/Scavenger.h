
#ifndef __SCAVENGER_H__
#define	__SCAVENGER_H__

//*********************************************************************************
//
//	Scavengerクラス
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

namespace MOTION_FRAME
{
	const int SUCK = 92;
	const int POWER_FINISH = 169;
	const int POWER_TO_WAIT = 200;
	const int HYPER_BEGIN = 201;
	const int HYPER_FINISH = 240;
}

//	class
class Scavenger : public		BaseChara
{
private:
	//　魔女モーション
	enum MOTION_DATA
	{
		WAIT,
		RUN,
		QUICK_START,
		QUICK,
		QUICK_END,
		POWER_START,
		POWER_END,
		HYPER_START,
		HYPER_END,
		DAMAGE,
		WIN,
		LOSE
	};
	const float DEFAULT_ABSORB_LENGTH = 3.0f;

private:
	int		stayTime;	//技関連の硬直経過時間
	float	absorb_length;		//コインを吸い込める距離
//	void	MotionManagement(int motion)override;

public:
	//	初期化・解放
	Scavenger(void);
	~Scavenger(void);
	bool	Initialize(int playerNum, Vector3 pos)override;

	//	更新・描画
	void	Render(iexShader* shader = nullptr, LPSTR technique = nullptr)override;

	//	動作関数
	bool	QuickArts(void)override;
	bool	PowerArts(void)override;
	bool	HyperArts(void)override;

	void	ShiftMove(void);
	void	RollAngle(void);

	//	情報設定
	void	SetAttackParam(int attackKind)override;
};

//*********************************************************************************
#endif // !__MAID_H__
