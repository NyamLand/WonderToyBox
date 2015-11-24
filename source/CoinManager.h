
#ifndef		__COIN_MANAGER_H__
#define		__COIN_MANAGER_H__

//******************************************************************************
//
//	CoinManagerクラス
//
//******************************************************************************

//	include
#include	"Coin.h"
#include	<memory>
#include	<list>

using namespace std;

//	class
class CoinManager
{
private:

protected:
	//	定数
	//static const int COIN_MAX = 201;		//	コイン最大数

	//	オブジェクト
	unique_ptr<iexMesh>	org;			//	オリジナルモデル
	list<Coin*>	coinList;
	Coin*		c_Coin;

	//	変数
	int				coin_num;	//	コイン枚数
	int				COIN_NUM;
public:
	//	初期化・解放
	CoinManager( void );
	~CoinManager( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	動作関数
	void	Set( const Vector3& pos, const Vector3& vec, const float& speed );
	void	DistCheck( int n );

	//	情報設定
	void	SetCoinGetAwayFlag(bool flag);
	void	SetState(bool state);
	
	//	情報取得
	bool	GetMinPos( Vector3& out, const Vector3& pos );
	int		GetFreeCoinNum( void )const;
	Coin*	GetCoin( void )const;
};

extern	CoinManager*	m_CoinManager;

//******************************************************************************
#endif // !__COIN_MANAGER_H__
 