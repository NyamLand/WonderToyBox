
#ifndef __SCAVENGER_H__
#define	__SCAVENGER_H__

//*********************************************************************************
//
//	Scavengerクラス
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

namespace SCAVENGER
{
	//定数
	const float SUCK_POWER = 2.5f;
	const float DEFAULT_ABSORB_LENGTH = 3.0f;

	namespace MOTION_FRAME
	{
		const int QUICKARTS_START_END = 92;	//QUICKARTS_STARTのモーションが終わったフレーム
		const int QUICKARTS_END = 153;
		const int POWER_TO_WAIT = 170;
		const int POWERARTS_END = 200;
		const int HYPER_BEGIN = 203;
		const int HYPER_ABSORB = 240;
		const int HYPER_FINISH = 248;
		const int DAMAGE_END = 278;
	}

	//　魔女モーション
	enum MOTION_DATA
	{
		WAIT,
		RUN,
		QUICKARTS_START,
		QUICKARTS,
		QUICKARTS_END,
		POWERARTS_START,
		POWERARTS_END,
		HYPERARTS_START,
		HYPERARTS_END,
		DAMAGE,
		WIN,
		LOSE
	};
}


//	class
class Scavenger : public		BaseChara
{

protected:
	int		stayTime;	//技関連の硬直経過時間
	float	absorb_length;		//コインを吸い込める距離
	bool		fireBallState;
	int		fireBallInterval;
	int		fireBallStep;
	Vector3	p_pos;
//	void	MotionManagement(int motion)override;

public:
	//	初期化・解放
	Scavenger(void);
	~Scavenger(void);
	bool	Initialize(int playerNum, Vector3 pos)override;

	//	更新・描画
	void	Update( void )override;
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
