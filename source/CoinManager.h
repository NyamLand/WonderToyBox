
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
	static const int COIN_MAX = 2000;		//	�R�C���ő吔

	//	�I�u�W�F�N�g
	iexMesh*	org;			//	�I���W�i�����f��
	Coin*		c_Coin;

	//	�ϐ�
	int				coin_num;	//	�R�C������

public:
	//	�������E���
	CoinManager( void );
	~CoinManager( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	void	Set( const Vector3& pos, const Vector3& vec, float speed );
};

extern	CoinManager*	m_CoinManager;

//******************************************************************************
#endif // !__COIN_MANAGER_H__
 