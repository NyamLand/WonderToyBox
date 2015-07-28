//*****************************************************************************************************************************
//
//		���C���V�[��
//
//*****************************************************************************************************************************

class	sceneMain : public Scene
{
private:
	//	�萔
	static	const		int		SHADOW_SIZE = 2048;
	static	const		int		SECOND = 60;			//	�P�b
	static	const		int		MINUTE = 60 * 60;	//	�P��
	static	const		int		TIMELIMIT = 10 * SECOND;		//	�b���~�b

	//	�ϐ�
	int		timer;

private:
	//	�I�u�W�F�N�g
	iexMesh*	m_CollisionStage;
	iexMesh*	m_Stage;

	//	�J�����p�����[�^
	Vector3	ViewPos;

	//	�e�N�X�`��
	iex2DObj*	ShadowTex;

	//	�o�b�t�@
	Surface*	backBuffer;
	Surface*	ShadowZ;

public:
	//	�������E���
	sceneMain( void );
	~sceneMain( void );
	bool Initialize( void );
	void	PlayerInitialize( void );

	//	�X�V
	void Update( void );	//	�X�V

	//	�`��
	void	Render( void );
	void	RenderShadowBuffer( void );
};


