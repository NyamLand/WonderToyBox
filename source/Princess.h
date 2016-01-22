
#ifndef		__PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	Princessクラス
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Princess : public		BaseChara
{
private:
	enum MOTION_DATA
	{
		STAND = 0,			//	立ち
		POSTURE = 0,		//	構え
		RUN,					//	走り
		JUMP,					//	ジャンプ
		ATTACK1,				//	攻撃１段階目
		ATTACK2,				//	攻撃２段階目
		GUARD,				//	ガード
		DAMAGE,				//	ダメージ
		KNOCKBACK,		//	吹っ飛び
		FALLTOGROUND,	//	地面に落下
		GETUP,				//	起き上がる
		WIN,					//	勝利
		DEFEAT,				//	敗北
	};

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
