
#ifndef		__PLAYERMANAGER_H__
#define		__PLAYERMANAGER_H__

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

	//	変数

private:
	Player*	c_Player[PLAYER_NUM];
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
	void	AddCoin( int player );
	void	SubCoin( int player );

	//	当たり判定
	void	HitCheck( void );
	void	HitCheckSphereVSCapsule( Player* p1, Player* p2 );

	//	情報取得
	Vector3	GetPos( int player );
	float		GetAngle( int player );
	Matrix	GetMatrix( int player );
	int			GetAttackParam( int player );
	int			GetCoinNum( int player );

	//	情報設定
	void		SetPos( int player, Vector3 pos );
	void		SetPos( int player, float x, float y, float z );
	void		SetAngle( int player, float angle );
	void		SetScale( int player, float scale );
};

extern	PlayerManager*		m_Player;
//****************************************************************************************
#endif // !__PLAYERMANAGER_H__
