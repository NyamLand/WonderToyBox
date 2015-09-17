
#ifndef		__PLAYERMANAGER_H__
#define		__PLAYERMANAGER_H__

#include	"Player.h"

//****************************************************************************************
//
//	PlayerManagerクラス
//
//****************************************************************************************
class PlayerManager
{
private:
	//	定数
	static const int PLAYER_NUM = 4;
	static const int OBJ_MAX = 10;

public:
	//	変数
	bool	CanHyper;	//　ハイパーアーツ同時発動制限用

private:
	Player*		c_Player[PLAYER_NUM];
	iex3DObj*	org[OBJ_MAX];

public:
	//	初期化・解放
	PlayerManager( void );
	~PlayerManager( void );
	void	Initialize( int input, int type, Vector3 pos );
	void	Load( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	動作関数
	void	DonketsuBoost();
	void	RaiseStatus( int worst, int type );

	//	当たり判定
	void	HitCheck( void );
	void	HitCheckSphereVSCapsule( Player* p1, Player* p2 );
	void	HitCheckCapsuleVSCapsule( Player* p1, Player* p2 );

	//	情報取得
	Vector3		GetPos( int player );
	float		GetAngle( int player );
	Matrix		GetMatrix( int player );
	bool		GetUnrivaled( int player );
	int			GetAttackParam( int player );
	int			GetCoinNum( int player );
	int			GetType( int player );
	int			GetP_Num( int player );
	bool		GetCanHyper( int player );
	int			GetPower( int player );
	float		GetSpeed( int player );
	static	PlayerManager*	GetInstans()
	{
		PlayerManager	playerManager;
		return	&playerManager;
	}

	//	情報設定
	void		SetPos( int player, Vector3 pos );
	void		SetPos( int player, float x, float y, float z );
	void		SetAngle( int player, float angle );
	void		SetScale( int player, float scale );
	void		SetType( int player, int type );
	void		SetKnockBackVec( int player, Vector3	knockBackVec );
	void		SetMode( int player, PlayerData::STATE state );
	void		SetPower( int player, int power );
	void		SetSpeed( int player, float speed );
	void		SetBoosting(int player, bool boosting);
};
#define	playerManager	( PlayerManager::GetInstans() )
extern	PlayerManager*		m_Player;
//****************************************************************************************
#endif // !__PLAYERMANAGER_H__
