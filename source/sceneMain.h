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
	int		playerNum;
	int		stageType;

private:
	//	�I�u�W�F�N�g
	iexMesh*	m_CollisionStage;
	iexMesh*	m_Stage;

	//	�J�����p�����[�^
	Vector3		ViewPos;

	//	�e�N�X�`��
	iex2DObj*	ShadowTex;
	iex2DObj*	screen;

	//	�o�b�t�@
	Surface*	backBuffer;
	Surface*	ShadowZ;

public:
	//	�������E���
	sceneMain( void );
	~sceneMain( void );
	bool Initialize( void );
	void	PlayerInitialize( void );
	void	DifferedInitialize( void );

	//	�X�V
	void	Update( void );	//	�X�V
	void	StartUpdate( void );
	void	MainGameUpdate( void );
	void	DonketsuUpdate( void );
	void	ClimaxUpdate( void );
	void	FinishUpdate( void );
	void	AllUpdate( void );

	//	�`��
	void	Render( void );
	void	RenderShadowBuffer( void );
	void	RenderDiffered( void );

	//	����֐�
	void	PointLight( const Vector3& pos, const Vector3& color, float range );
	void	DirLight( const Vector3& dir, const Vector3& color );
};

