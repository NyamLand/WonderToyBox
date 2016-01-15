
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
private:
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

private:
	SORT_INFO						sortInfo[4];
	ORIGINAL_INFO				originInfo[4];
	NUMBER_INFO					number[4];
	NUMBER_INFO					bonusNumber[4];
	NUMBERIMAGE_INFO		numberImageInfo[4];
	NUMBERIMAGE_INFO		bonusNumberImageInfo[4];
	ROULETTE_INFO				rouletteInfo;
	MENU_INFO						menuInfo;
	VIEW_RANK_INORDER	viewRankInOrder;
	LASTBONUS_INFO			lastBonusInfo;
	VIEW_INFO						viewInfo;

	//	���X�g�{�[�i�X�v�Z�p
	SORT_INFO						maxCoinNum[4];
	SORT_INFO						fallStageNum[4];
	SORT_INFO						coin77[4];
	SORT_INFO						minCoinNum[4];
	SORT_INFO						hitAttackNum[4];
	
	//	���f���E�摜
	Surface*							backBuffer;
	unique_ptr<Camera>		view2D;
	iex2DObj*						infoScreen;
	unique_ptr<iex2DObj>	back;
	unique_ptr<iex3DObj>	org[4];
	iexMesh*							bgStage;
	iex3DObj*						obj[4];
	iex2DObj*						originNumber;
	iex2DObj*						menuText;
	iex2DObj*						lastBonusText;
	iex2DObj*						life;
	ImageObj							menuHead;
	ImageObj							originCoinImage[4];
	ImageObj							rankImage[4];
	ImageObj							menuImage[3];
	ImageObj							playerNumImage;
	ImageObj							faceImage;
	ImageObj							waveCircleImage;
	ImageObj							notApplicable;		//	�Y���Ȃ��摜
	ImageObj							lifeImage[4];		//	���C�t�摜
	LIFE_INFO						lifeInfo;

	//	�ϐ�
	int		mode;
	int		step;
	int		lastBonus;
	int		bonusPlayer;
	bool		changeScene;

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
	void	LifeImageInitialize( void );

	//	�S�̍X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	�e���[�h�X�V
	void	ResultUpdate( void );
	void	SelectUpdate( void );
	void	IntermediateResultsUpdate( void );	//	���Ԍ���

	//	�e�摜�`��
	void	SelectRender( void );
	void	NumberImageRender( void );
	void	RankRender( void );
	void	LifeRender( void );
	
	//	����֐�
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

	//	�I�����[�h�֐�
	bool	DownPolygon( void );
	void	ControlCursor( void );

	//	���U���g���[�h
	void	ModeRoulette( void );
	void	ModeLastBonus( void );
	void	ModeRank( void );
	void	ModeRankSkip( void );

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
	void	SetNumberImageInfo( const int& player, const int& coin );
	void	SetNumberImageInfo( NUMBERIMAGE_INFO& numImageInfo, NUMBER_INFO& numInfo, const int& num );
	void	SetNumberInfo( NUMBER_INFO& number, int coin );	//	�R�C�����������n��
};

//*******************************************************************************
#endif // !__SCENERESULT_H__
