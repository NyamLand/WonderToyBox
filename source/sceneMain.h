//*****************************************************************************************************************************
//
//		���C���V�[��
//
//*****************************************************************************************************************************

//	include
#include	<memory>
#include	"Camera.h"

//	class
class	sceneMain : public Scene
{
private:
	//	�萔
	static	const		int		SHADOW_SIZE = 2048;
	static	const		int		SECOND = 60;			//	�P�b
	static	const		int		MINUTE = 60 * 60;	//	�P��
	static	const		int		TIMELIMIT = 10 * SECOND;		//	�b���~�b
	static	const		int		GAME_START_COIN_NUM = 200;	//	�S��

	//	�ϐ�
	int		timer;
	int		playerNum;
	int		stageType;
	int		gameStartCoinNum;
	float	stageScale;
	Vector3 dir;

private:
	//	�I�u�W�F�N�g
	iexMesh*	m_CollisionStage;
	iexMesh*	m_Stage;

	//	�J�����E�p�����[�^
	unique_ptr<Camera>	playerView[4];
	Vector3		ViewPos;

	//	�e�N�X�`��
	iex2DObj*	ShadowTex;
	iex2DObj*	RefTex;

	//	�o�b�t�@
	iex2DObj*	hdr;
	Surface*	backBuffer;
	Surface*	ShadowZ;

	//	�����_�[�^�[�Q�b�g
	unique_ptr<iex2DObj>	playerWipe[4];
	unique_ptr<iex2DObj>	m_screen;

	//	�f�B�t�@�[�h�p
	//iex2DObj*	diffuse;
	//iex2DObj*	specular;
	//iex2DObj*	depth;
	//iex2DObj*	normal;
	//iex2DObj*	light;
	//iex2DObj*	light_s;

public:
	//	�������E���
	sceneMain( void );
	~sceneMain( void );
	bool Initialize( void );
	void	InitializeDebug(void);
	void	PlayerInitialize( void );
	void	StageInitialize( void );
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
	void	RenderRef( void );
	void	RenderHDR( void );
	void	RenderWipe( void );

	//	����֐�
	void	PointLight( const Vector3& pos, const Vector3& color, float range );
	void	DirLight( const Vector3& dir, const Vector3& color );
};

