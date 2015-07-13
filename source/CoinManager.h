
#ifndef		__COIN_MANAGER_H__
#define		__COIN_MANAGER_H__

//******************************************************************************
//
//	CoinManagerクラス
//
//******************************************************************************
class CoinManager
{
private:

private:
	//	定数
	static const int COIN_MAX = 2000;

	//	オブジェクト
	Coin*	c_Coin[COIN_MAX];
	
	//	変数
	std::mt19937 ran{ std::random_device()() };

public:
	//	初期化・解放
	CoinManager( void );
	~CoinManager( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );
};

extern	CoinManager*	m_CoinManager;

//******************************************************************************
#endif // !__COIN_MANAGER_H__
 