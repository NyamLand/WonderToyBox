
#ifndef __GAMEMANAGER_H__
#define	__GAMEMANAGER_H__

//*******************************************************************************
//
//	GameManagerクラス
//
//*******************************************************************************
class GameManager
{
private:
	int		timer;


public:
	//	初期化・解放
	GameManager( void );
	~GameManager( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
};


//*******************************************************************************
#endif // !__GAMEMANAGER_H__
