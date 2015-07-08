
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
	static const int PLAYER_NUM = 4;

private:
	Player*	c_Player[PLAYER_NUM];

public:
	//	初期化・解放
	PlayerManager( void );
	~PlayerManager( void );
	void	Initialize( int input, int type, Vector3 pos );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	動作関数
	void	HitCheck( void );

	//	情報取得
	Vector3	GetPos( int player );
	float		GetAngle( int player );
	Matrix	GetMatrix( int player );
	int			GetAttackParam( int player );

	//	情報設定
	void		SetPos( int player, Vector3 pos );
	void		SetPos( int player, float x, float y, float z );
	void		SetAngle( int player, float angle );
	void		SetScale( int player, float scale );
};

extern	PlayerManager*		m_Player;
//****************************************************************************************
#endif // !__PLAYERMANAGER_H__
