
#ifndef		__COIN_MANAGER_H__
#define		__COIN_MANAGER_H__

//******************************************************************************
//
//	CoinManager�N���X
//
//******************************************************************************
class CoinManager
{
private:

private:
	//	�萔
	static const int COIN_MAX = 2000;

	//	�I�u�W�F�N�g
	Coin*	c_Coin[COIN_MAX];
	
	//	�ϐ�
	std::mt19937 ran{ std::random_device()() };

public:
	//	�������E���
	CoinManager( void );
	~CoinManager( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );
};

extern	CoinManager*	m_CoinManager;

//******************************************************************************
#endif // !__COIN_MANAGER_H__
 