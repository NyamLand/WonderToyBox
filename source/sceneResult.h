
#ifndef __SCENERESULT_H__
#define	__SCENERESULT_H__

//*******************************************************************************
//
//	sceneResult�N���X
//
//*******************************************************************************

class	PlayerManager;

class sceneResult : public	Scene
{
private:
	struct RESULT_INFO
	{
		int		p_Coin;
		int		p_num;
		int		p_addCoin;
	};

	struct NUMBER
	{
		int		hundred;		//�R�C���O����
		int		ten;			//�R�C���񌅖�
		int		one;			//�R�C���ꌅ��
	};

private:
	RESULT_INFO	resultInfo[4];
	NUMBER		number[4];		//	�擾�R�C�������p�i���o�[
	NUMBER		addnumber[4];	//	�擾�R�C���ƃ{�[�i�X�̍��Z�p�i���o�[
	//	�\�[�g�p
	char	str[256];

	//	�p�����[�^
	int		coinNum[4];
	int		playerNum[4];
	int		lastBonus;
	int		lastBonusNum[4];
	int		step;
	int		wait;
	int		resultcount;	//	���o�p�J�E���g
	int		Mode;			//	���[�h�����p�ϐ�
	int		Sy;				//	�Z���N�g��ʃ|�W�V����
	int		StringPos_Y;	//	�Z���N�g��ʂ̕����|�W�V����
	bool	Modeflg;		//	���[�h�p�t���O
	bool	addCoinflg;		//	�R�C�����Z�l�p�t���O
	bool	rankingflg[4];	//	���ʗp�t���O
	iex2DObj*	back;		//	�w�i
	iex2DObj*	r_number;	//	�R�C���̖���
	iex2DObj*	Sback;		//	�Z���N�g���̔w�i
	iex2DObj*	Smenu;		//	�Z���N�g���̃��j���[
	iex2DObj*	result;		//	���U���g
	iexMesh*	collision;


public:
	//	�������E���
	sceneResult( void );
	~sceneResult( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	void	ResultUpdate( void );
	void	SelectUpdata( void );

	void	SelectRender( void );
	//	����֐�
	void	BubbleSort( void );
	void	SetLastBonus( void );
	//	���o�p�֐�
	void	Production( void );						//���U���g�̉��o�p�֐�
	void	ProductionRotation( int playerNum );	//�R�C��������]�֐�
	void	ProductionCoinHandOff(NUMBER& number, int coinNum);	//�R�C�����������n��
};
//*******************************************************************************
#endif // !__SCENERESULT_H__
