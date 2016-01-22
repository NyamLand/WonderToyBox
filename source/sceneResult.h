
#ifndef __SCENERESULT_H__
#define	__SCENERESULT_H__

//*******************************************************************************
//
//	sceneResult�N���X
//
//*******************************************************************************

//	include
#include	<memory>
#include	"Camera.h"
using namespace std;

//	class
class sceneResult : public	Scene
{
public:
	//	�������
	struct NUMBER_INFO
	{
		int		hundred;		//�R�C���O����
		int		ten;			//�R�C���񌅖�
		int		one;			//�R�C���ꌅ��
		bool	H_flg;			//�S�̈ʃ����_�[�p�t���O
	};

	//	���ׂ�������
	struct SORT_INFO
	{
		int		num;
		int		sortRank;
		int		rank;
	};

	//	���̃f�[�^
	struct ORIGINAL_INFO
	{
		int		num;			//	�Q�[���I�����̃R�C������
		int		rank;			//	�Q�[���I�����̏���
		int		bonus;			//	�{�[�i�X
	};

	//	���X�g�{�[�i�X�f�[�^
	struct LASTBONUS_INFO
	{
		int		bonus[4];
		int		step;
		float	t;
		ImageObj	textImage;
		TLVERTEX	v[4];
	};

	//	�R�C�������\����
	struct NUMBERIMAGE_INFO
	{
		int			scale;
		POINT	pos;
		ImageObj		hundred;		//	�R�C���O����
		ImageObj		ten;				//	�R�C���񌅖�
		ImageObj		one;				//	�R�C���ꌅ��
		bool	hundredRenderFlag;	//	�S�̈ʃ����_�[�p�t���O
	};

	//	���[���b�g�\����
	struct ROULETTE_INFO
	{
		int		step;
		int		timer;
	};

	//	���j���[�\����
	struct MENU_INFO
	{
		int		select;
		int		screenH;
		float	alpha;
		float	t;
	};

	//	�����N�\���p�\����
	struct VIEW_RANK_INORDER
	{
		int		timer;
		int		step;
	};

	//	�J�������
	struct VIEW_INFO
	{
		Vector3	pos;
		Vector3	target;
		Vector3	texPos;		//	�`��ʒu
		LVERTEX	v[4];		//	�`��p���_
	};

	//	���C�t���\���
	struct LIFE_INFO
	{
		int		step;
		bool		renderflag;
		float		t;
		int		waitTimer;
		bool		isEnd;
		int		culLife;
		int		maxW;
		int		maxH;
	};

	//	�J�[�e���ʒu�ۑ��\����
	struct CurtainPosInfo
	{
		int			leftOutPosX;
		int			rightOutPosX;
		int			centerPosX;
		int			upPosY;
		int			underPosY;
		int			leftPos;
		int			rightPos;
	};

	//	�J�[�e���p�\����
	struct CurtainInfo
	{
		iex2DObj*	obj;
		TLVERTEX	tlv[4];
		float			t;
	};

private:
	SORT_INFO						sortInfo[4];
	ORIGINAL_INFO				originInfo[4];
	NUMBER_INFO					number[4];
	NUMBER_INFO					bonusNumber[4];
	NUMBER_INFO					roundCoinNumber[3][4];
	NUMBERIMAGE_INFO		numberImageInfo[4];
	NUMBERIMAGE_INFO		bonusNumberImageInfo[4];
	NUMBERIMAGE_INFO		roundCoinNumberImageInfo[3][4];
	ROULETTE_INFO				rouletteInfo;
	MENU_INFO						menuInfo;
	VIEW_RANK_INORDER	viewRankInOrder;
	LASTBONUS_INFO			lastBonusInfo;
	VIEW_INFO						viewInfo;
	CurtainPosInfo					curtainPosInfo;
	CurtainInfo						curtainInfoR, curtainInfoL;

	//	���X�g�{�[�i�X�v�Z�p
	SORT_INFO						maxCoinNum[4];
	SORT_INFO						fallStageNum[4];
	SORT_INFO						coin77[4];
	SORT_INFO						minCoinNum[4];
	SORT_INFO						hitAttackNum[4];

	//	���f���E�摜
	Surface*							backBuffer;
	unique_ptr<Camera>		view2D, viewTest;
	iex2DObj*						infoScreen;
	iex2DObj*						lastResultTest;
	iex2DObj*						check;
	unique_ptr<iex2DObj>	back;
	unique_ptr<iex3DObj>	org[4];
	iexMesh*							bgStage;
	iex3DObj*						obj[4];
	iex2DObj*						originNumber;
	iex2DObj*						menuText;
	iex2DObj*						lastBonusText;
	iex2DObj*						life;
	iex2DObj*						orgRound;
	iex2DObj*						orgCurtain;
	ImageObj							menuHead;
	ImageObj							originCoinImage[4];
	ImageObj							rankImage[4];
	ImageObj							menuImage[3];
	ImageObj							playerNumImage;
	ImageObj							faceImage;
	ImageObj							waveCircleImage;
	ImageObj							notApplicable;		//	�Y���Ȃ��摜
	ImageObj							lifeImage[4];		//	���C�t�摜
	ImageObj							checkImage[4];
	ImageObj							roundImage[3];	
	LIFE_INFO						lifeInfo;

	//	�ϐ�
	int		mode;
	int		step;
	int		lastBonus;
	int		bonusPlayer;
	int		nextLife[3][4];	//	�S�l���R���
	int		culRound;
	float		curtainBrightness;		//	�J�[�e�����邳

	int	lightMoveNum;
	bool		inputCheck[4];		
	bool		changeScene;
	int		curtainMode;
	bool		curtainState;
	float		curtain_t;
	Vector3	lightPos[4];	//	�V�F�[�_�[�p���C�g�ʒu
	Vector3 lightMove_start[4];
	Vector3 lightMove_finish[4];
	float	lightSize[4];
	float	light_t[4];
	int	waitTimer;

public:
	//	�������E���
	sceneResult( void );
	~sceneResult( void );
	bool	Initialize( void );
	void	Release( void );

	//	�e���񏉊���
	void	CameraInitialize( void );
	void	ModelInitialize( void );
	void	ResultInfoInitialize( void );
	void	NumberImageInfoInitialize( void );
	void	RankImageInitialize( void );
	void	LifeInfoInitialize( void );
	void	RenderTargetTextureInitialize( void );
	void	LastBonusImageInitialize( void );
	void	MenuInfoInitialize( void );
	void RouletteInfoInitialize( void );
	void	CurtainInfoInitialize( void );
	void	RoundInfoInitialize( void );
	void	Load( void );

	//	�S�̍X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	�e���[�h�X�V
	void	ResultUpdate( void );
	void	SelectUpdate( void );
	void	LastResultUpdate( void );
	bool	CurtainUpdate( void );

	//	�e�摜�`��
	void	SelectRender( void );
	void	NumberImageRender( void );
	void	NumberImageRender( NUMBERIMAGE_INFO numberImageInfo );
	void	RankRender( void );
	void	LifeRender( void );
	void	InputCheckRender( void );
	void	ResultRender( void );
	void	LastResultRender( void );
	void	CurtainRender( void );
	
	//	����֐�
	bool	LightUpdate( void );
	void	SetLastBonus( void );
	void	AddLastBonus( void );
	void	SetBonusPlayer( void );
	void	Sort( SORT_INFO( &sort_info )[4] );
	void	ReverseSort( SORT_INFO( &sort_info )[4] );
	bool	Roulette( void );
	bool	ViewRankInOrder( void );
	bool	RankWave( void );
	void	MoveScene( void );
	bool	NextLifeAnnouncing( void );
	void	ProductionSkip( void );
	void	SetRoundCoinNumber( void );
	bool	WaitTimeUpdate( void );
	void	MoveLight( void );
	bool	MoveLightSet( int num );
	bool	WallLightCheck( int num );

	//	�I�����[�h�֐�
	bool	DownPolygon( void );
	void	ControlCursor( void );

	//	���U���g���[�h
	void	ModeRoulette( void );
	void	ModeLastBonus( void );
	void	ModeRank( void );
	void	ModeRankSkip( void );
	void	ModeLastResult( void );
	void	ModeInputWait( void );

	//	���X�g�{�[�i�X�p�֐�
	bool	LastBonusUpdate( void );
	bool	InBoard( void );
	bool	BonusAnnouncing( void );
	bool	PlayerAnnouncing( void );
	bool	OutBoard( void );
	bool	AddBonus( void );

	//	���ݒ�
	void	SetNextLife( void );
	void	SetRank( void );
	void	SetNumberImageInfo( NUMBERIMAGE_INFO& numImageInfo, NUMBER_INFO& numInfo, const int& num );
	void	SetNumberInfo( NUMBER_INFO& number, int coin );	//	�R�C�����������n��
	void	SetCurtainMode( int mode );
	void	SetWaitTimer( int time );
};

//*******************************************************************************
#endif // !__SCENERESULT_H__
