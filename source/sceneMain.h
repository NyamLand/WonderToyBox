//*****************************************************************************************************************************
//
//		メインシーン
//
//*****************************************************************************************************************************

//	include
#include	<memory>

//	class
class	sceneMain : public Scene
{
private:
	//	定数
	static	const		int		GAME_START_COIN_NUM = 100;	//	百枚

	//	変数
	Vector3 dir;
	int		gameStartCoinNum;

private:
	//	バックバッファ
	Surface*	backBuffer;

	//	レンダーターゲット
	unique_ptr<iex2DObj>	m_screen;

public:
	//	初期化・解放
	sceneMain( void );
	~sceneMain( void );
	bool Initialize( void );
	void	PlayerInitialize( void );

	//	更新
	void	Update( void );
	void	StartUpdate( void );
	void	MainGameUpdate( void );
	void	ClimaxUpdate( void );
	void	FinishUpdate( void );
	void	AllUpdate( void );

	//	描画
	void	Render( void );
	void	RenderWipe( void );
};

