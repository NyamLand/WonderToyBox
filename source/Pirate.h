
#ifndef __PIRATE_H__
#define	__PIRATE_H__

//*********************************************************************************
//
//	Princessクラス
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

namespace PIRATE
{
	namespace MOTION_FRAME
	{
		const int QUICKARTS_SHOT = 113;
		const int QUICKARTS_END = 131;

		const int POWERARTS_END = 210;

		const int HYPERARTS_END = 255;
		const int DAMAGE_END = 286;
	}
	
	enum MOTION_DATA
	{
		WAIT,
		RUN,
		QUICK,
		POWER_START,
		POWER,
		POWER_END,
		HYPER,
		DAMAGE,
		WIN,
		LOSE
	};
}


//	class
class Pirate : public		BaseChara
{
private:
	

	enum QuickArts_DATA
	{
		NORMAL_SHOT = 0,
		TIMER_SHOT,
	};

private:
	//void	MotionManagement( int motion )override;
	iexMesh*	cannon;

public:
	//	初期化・解放
	Pirate( void );
	~Pirate( void );
	bool	Initialize( int playerNum, Vector3 pos )override;

	//	更新・描画
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	動作関数
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;
	void	SetCannonPos( void );
	//	情報設定
	void	SetAttackParam( int attackKind )override;


};

//*********************************************************************************
#endif // !__PRINCESS_H__
