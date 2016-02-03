
#ifndef		__PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	Princessクラス
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

namespace PRINCESS
{
	namespace MOTION_FRAME
	{

	}

	enum MOTION_DATA
	{
		WAIT,
		RUN,
		JUMP,
		QUICK,
		POWER,
		GUARD,
		DAMAGE,
		KNOCKBACK,
		FALL,
		WAKEUP,
		WIN,
		LOSE
	};
}

//	class
class Princess : public		BaseChara
{
private:
	int hyperNum;	//	ハイパーウェーブ回数

private:
//	void	MotionManagement( int motion )override;

public:
	//	初期化・解放
	Princess( void );
	~Princess( void );
	bool	Initialize( int playerNum, Vector3 pos )override;

	//	更新・描画
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	動作関数
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;

	//	情報設定
	void	SetAttackParam( int attackKind )override;
};

//*********************************************************************************
#endif // !__PRINCESS_H__
