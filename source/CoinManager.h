
#ifndef		__COIN_MANAGER_H__
#define		__COIN_MANAGER_H__

//******************************************************************************
//
//	CoinManager�N���X
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
	//	�I�u�W�F�N�g
	iexMesh*		org;			//	�I���W�i�����f��
	list<Coin*>	coinList;

	//	�ϐ�
	int				coin_num;	//	�R�C������

public:
	//	�������E���
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	����֐�
	void	Append( const Vector3& pos, const Vector3& vec, const float& speed );
	void	DistCheck( int n );

	//	���ݒ�
	void	SetCoinGetAwayFlag( bool flag );
	void	SetState( bool state );
	
	//	���擾
	bool	GetMinPos( Vector3& out, const Vector3& pos );
	int		GetFreeCoinNum( void )const;
	list<Coin*>	GetList( void )const;
	static CoinManager*	GetInstance( void );
};
#define	coinManager ( CoinManager::GetInstance() )

//******************************************************************************
#endif // !__COIN_MANAGER_H__
 