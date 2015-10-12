
#ifndef __SCENETITLE_H__
#define	__SCENETITLE_H__

//*****************************************************************************
//
//	sceneTitle�N���X
//
//*****************************************************************************

//	�^�C�g�����
namespace
{
	//	�^�C�g�����[�h
	namespace TITLE_MODE
	{
		enum
		{
			TITLE,
			MENU,
			SELECT_PLAYERNUM,
			SELECT_CHARACTER,
			SELECT_STAGE,
			SELECT_CHECK,
			OPTION,
			CREDIT,
			MOVE_MAIN,
		};
	}

	//	���j���[���[�h
	namespace SELECT_MODE
	{
		enum
		{
			PLAYER_NUM,
			CHARACTER,
			STAGE,
		};
	}
}

class sceneTitle : public	Scene
{
private:	//	�萔
	static const int STAGE_MAX = 8;

private:	//	�\����

	//	�e�L�����N�^�[���
	struct CharacterInfo
	{
		LPSTR	name;		//	�L�����N�^�[��
		bool	select;		//	�I������Ă��邩
	};

	//	�I�����
	struct SetInfo
	{
		int		characterType[4];
		int 	playerNum;		//�@���l�ŗV�ԁH
		int		stageType;		//�@�ǂ̃X�e�[�W�H
		bool	ready;			//	����OK���H
		int		step_cs;
	};

	//	�v���C���[�I�����
	struct PlayerSelectInfo
	{
		int		num;
		bool	isSelect;
	};

	//	�J�����p�p�����[�^�\����
	struct CameraInfo
	{
		int			posNum;				//	���̃J�����ړ���̃|�W�V�������w�������l
		float		t;					//	��ԃp�����[�^
		Vector3		pos;				//	�J�����|�W�V����
		Vector3		target;				//	�J�����̃^�[�Q�b�g�|�W�V����
		Vector3		lerpStartPos;		//	���[�v�X�^�[�g�|�W�V����
	};

	//	�J�[�e���p�\����
	struct CurtainInfo
	{
		iex2DObj*	obj;
		TLVERTEX	tlv[4];
		float			t;
	};

	//	�^�C�g���p�p�����[�^
	struct TITLE_INFO
	{
		int		step;
		CurtainInfo	curtainR;
		CurtainInfo	curtainL;
		ImageObj		pressSpace;
		ImageObj		titleImage;
	};

	//	���j���[�p�p�����[�^
	struct MENU_INFO
	{
		int	 menu_num;
	};

	//	���I��p�p�����[�^
	struct SELECT_INFO
	{
		int		mode;
		int		select_mode;
		int		menu_num;
		PlayerSelectInfo	playerselectInfo[4];
	};

private:
	//	�w�i
	iexMesh*	stage;

	//	�J�����p�p�����[�^
	CameraInfo	cameraInfo;

	//	�Q�[���ݒ�p�p�����[�^
	CharacterInfo	characterInfo[CHARACTER_TYPE::MAX];
	SetInfo		setInfo;
	int				mode;

	//	�e���[�h�p�p�����[�^
	TITLE_INFO		titleInfo;
	MENU_INFO		menuInfo;
	SELECT_INFO	selectInfo;

public:
	//	�������E���
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );
	void	TitleInitialize( void );
	void	MenuInitialize( void );
	void	SelectInitialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//-------------------------------------
	//�@�e��ʂ��Ƃ̃��\�b�h
	//-------------------------------------
	//�@�^�C�g��
	void	TitleUpdate( void );
	void	TitleRender( void );

	//�@���j���[���
	void	MenuUpdate( void );
	void	MenuRender( void );

	//	�I��
	void	SelectUpdate( void );
	void	SelectRender( void );

	//�@�l���I��
	void	SelectPlayerNumUpdate( void );
	void	SelectPlayerNumRender( void );

	//�@�L�����I��
	void	SelectCharacterUpdate( void );
	void	SelectCharacterRender( void );

	//	�L�����N�^�[�I�𓮍�
	void	SelectPlayer( void );
	void	SelectCPU( void );
	
	//�@�X�e�[�W�I��
	void	SelectStageUpdate( void );
	void	SelectStageRender( void );
	
	//�@�ŏI�m�F
	void	SelectCheckUpdate( void );
	void	SelectCheckRender( void );

	//�@�I�v�V����
	void	OptionUpdate( void );
	void	OptionRender( void );

	//�@�N���W�b�g
	void	CreditUpdate( void );
	void	CreditRender( void );
};

//*****************************************************************************
#endif // !__SCENETITLE_H__
