
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
	//	オブジェクト
	iexMesh*		org;			//	オリジナルモデル
	list<Coin*>	coinList;

	//	変数
	int				coin_num;	//	コイン枚数

public:
	//	初期化・解放
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	動作関数
	void	Append( const Vector3& pos, const Vector3& vec, const float& speed );
	void	DistCheck( int n );

	//	情報設定
	void	SetCoinGetAwayFlag( bool flag );
	void	SetState( bool state );
	
	//	情報取得
	bool	GetMinPos( Vector3& out, const Vector3& pos );
	int		GetFreeCoinNum( void )const;
	list<Coin*>	GetList( void )const;
	static CoinManager*	GetInstance( void );
};
#define	coinManager ( CoinManager::GetInstance() )

//******************************************************************************
#endif // !__COIN_MANAGER_H__
 