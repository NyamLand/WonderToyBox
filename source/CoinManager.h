
#ifndef		__COIN_MANAGER_H__
#define		__COIN_MANAGER_H__

//******************************************************************************
//
//	CoinManagerクラス
//
//******************************************************************************

//	include
#include	"Coin.h"
#include	<list>

using namespace std;

//	class
class CoinManager
{
private:

protected:
	//	定数
	static const int COIN_MAX = 201;		//	コイン最大数

	//	オブジェクト
	iexMesh*	org;			//	オリジナルモデル
	list<Coin*>	coinList;
	Coin*		c_Coin;

	//	変数
	int				coin_num;	//	コイン枚数

public:
	//	初期化・解放
	CoinManager( void );
	~CoinManager( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	動作関数
	void	Set( const Vector3& pos, const Vector3& vec, const float& speed );
	void	DistCheck( int n );

	//	情報設定
	void	SetCoinGetAwayFlag( bool flag );
};

extern	CoinManager*	m_CoinManager;

//******************************************************************************
#endif // !__COIN_MANAGER_H__
 