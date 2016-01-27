
#pragma once

//*******************************************************************************
//
//	sceneMenu�N���X
//
//*******************************************************************************

//	include
#include	<memory>
#include	"BaseChara.h"
using namespace std;

//	class
class sceneMenu	:	public	Scene
{
private:
	struct PlayerNumSelectInfo
	{
		float	t;
		int		num;
		int		saveY;
		int		sy;
	};

	struct CharacterSelectInfo
	{
		int		step;
		int		timer;
		int		character[4];
		int		imagePos;
		int		playerNum;
		bool	select[4];
	};

	struct StageSelectInfo
	{
		float	angle;
		int		stage;
	};

	struct CheckSelectInfo
	{
		bool	select;
		bool	check;
		int		step;
	};

	struct OptionInfo
	{
		bool	itemflg;
		int		life;
		int		minute;
		int		second;
		int		step;
	};

	struct BGInfo{
		Vector3 target;
		Vector3 start;
		Vector3 end;
		Vector3	cpos;
		Vector3 cspos;//�J�����ړ��p�X�^�[�g�|�W�V����
		Vector3 cepos;//�J�����ړ��p�G���h�|�W�V����
		float t;	//��ԃp�����[�^
		float mt;	//�ړ��p��ԃp�����[�^
		bool moveflg;//�ړ��p�t���O
	};
private:	
	//option�݌v��
	iex2DObj*				optionImage;
	iex2DObj*				optionTime;
	iex2DObj*				optionLife;
	iex2DObj*				optionMenu;
	unique_ptr<iexMesh>		BG;
	
public:
	//	�I�v�V�����֐�
	void	OptionInitialize(void);
	void	OptionUpdate(void);
	void	OptionRender(void);
	void	ArrowRender(void);
	void	TimerRender(void);
	void	OptionSelectRender(void);
	//void	OptionDUpdate(void);

private:
	//	�摜�E���f��
	unique_ptr<iex2DObj>	back;
	unique_ptr<iex2DObj>	frame;
	unique_ptr<iex2DObj>	checkBack;
	unique_ptr<iex2DObj>	checkCursor;
	unique_ptr<iex2DObj>	playerNumText;
	unique_ptr<iexMesh>		deskStage;
	unique_ptr<iexMesh>		toyStage;
	unique_ptr<iex2DObj>	playerNum;
	unique_ptr<iex3DObj>	org[4];
	unique_ptr<iexMesh>		orgCannon;
	iexMesh*					cannon[4];
	iex3DObj*				obj[4];
	iex2DObj*				face;
	iex2DObj*				cursor;
	iex2DObj*				cpuCursor;
	iex2DObj*				decidecursor;
	iex2DObj*				selectCheckCursor;
	iex2DObj*				triangleCursor;
	ImageObj				textImage;
	ImageObj				faceImage[CHARACTER_TYPE::MAX];
	ImageObj				cursorImage[4];
	ImageObj				decidecursorImage[4];


	//	�e���[�h���
	PlayerNumSelectInfo		playerNumSelectInfo;
	CharacterSelectInfo		characterSelectInfo;
	StageSelectInfo			stageSelectInfo;
	CheckSelectInfo			checkSelectInfo;
	OptionInfo				optionInfo;
	BGInfo					bgInfo;

	//	���ʕϐ�
	int		mode;
	int		tempmode;
	bool	changeScene;
	Vector3 dir;
	Vector3 modelPos[4];
	Vector3 modelAngle[4];
	Vector3 modelScale[4];
	
public:
	//	�������E���
	sceneMenu( void );
	~sceneMenu( void );
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	void	CameraUpdate( void );
	void	ChangeToOption( void );
	void	ModelUpdate( void );
	void	CannonRender( void );

	//	�v���C���[�l���I���֐�
	void	SelectPlayerNumInitialize( void );
	void	SelectPlayerNumUpdate( void );
	void	SelectPlayerNumRender( void );

	//	�L�����I���֐�
	void	SelectCharacterInitialize( void );
	void	SelectCharacterUpdate( void );
	void	SelectCharacterRender( void );

	//	�X�e�[�W�I���֐�
	void	SelectStageInitialize( void );
	void	SelectStageUpdate( void );
	void	SelectStageRender( void );

	//	�`�F�b�N�֐�
	void	SelectCheckInitialize( void );
	void	SelectCheckUpdate( void );
	void	SelectCheckRender( void );

	//	���C���ړ��֐�
	void	MoveMainInitialize( void );
	void	MoveMainUpdate( void );
	void	MoveMainRender( void );

	//	�^�C�g���ړ��֐�
	void	MoveTitleInitialize( void );
	void	MoveTitleUpdate( void );
	void	MoveTitleRender( void );


	//	���ݒ�
	void	SetMode( int mode );
	void SetCannonPos( void );

};
