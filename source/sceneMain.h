//*****************************************************************************************************************************
//
//		���C���V�[��
//
//*****************************************************************************************************************************

//	include
#include	<memory>

//	class
class	sceneMain : public Scene
{
private:
	//	�萔
	static	const		int		GAME_START_COIN_NUM = 100;	//	�S��

	//	�ϐ�
	Vector3 dir;
	int		gameStartCoinNum;

private:
	//	�o�b�N�o�b�t�@
	Surface*	backBuffer;

	//	�����_�[�^�[�Q�b�g
	unique_ptr<iex2DObj>	m_screen;

public:
	//	�������E���
	sceneMain( void );
	~sceneMain( void );
	bool Initialize( void );
	void	PlayerInitialize( void );

	//	�X�V
	void	Update( void );
	void	StartUpdate( void );
	void	MainGameUpdate( void );
	void	ClimaxUpdate( void );
	void	FinishUpdate( void );
	void	AllUpdate( void );

	//	�`��
	void	Render( void );
	void	RenderWipe( void );
};

