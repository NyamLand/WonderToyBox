
#ifndef __SCENERESULT_H__
#define	__SCENERESULT_H__

//*******************************************************************************
//
//	sceneResult�N���X
//
//*******************************************************************************
class sceneResult : public	Scene
{
private:
	struct ORIGIN_INFO
	{
		int		coinNum[4];
		int		rank[4];
	};

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
		bool	H_flg;			//�S�̈ʃ����_�[�p�t���O
	};

	struct RANK
	{
		int		rank;
		bool	rankflg;		//	���ʗp�t���O
	};
private:
	RESULT_INFO	resultInfo[4];
	NUMBER		number[4];
	NUMBER		BonusNumber[4];
	NUMBER		totalNumber[4];
	RANK		rank[4];

	//	�\�[�g�p
	char	str[256];

	//	�p�����[�^
	int		coinNum[4];
	int		totalCoinNum[4];
	int		playerNum[4];
	int		lastBonus;
	int		lastBonusNum[4];
	int		step;
	int		wait;
	int		resultcount;	//	���o�p�J�E���g
	int		waitTimer;		//	�ҋ@�p�^�C�}�[
	int		mode;			//	���[�h�����p�ϐ�
	int		Sy;				//	�Z���N�g��ʃ|�W�V����
	int		StringPos_Y;	//	�Z���N�g��ʂ̕����|�W�V����
	
	//	�t���O�֌W
	bool	Modeflg;		//	���[�h�p�t���O
	bool	bonusflg;		//	�{�[�i�X�l�p�t���O
	bool	addCoinflg;		//	�R�C�����Z�l�p�t���O


	//	�摜�f�[�^�֌W
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

	void	ResultRender(NUMBER& number, Vector3 Pos);
	void	RankRender(int ranking);
	void	SelectRender( void );
	
	//	����֐�
	void	BubbleSort( void );
	void	SetLastBonus( void );

	//	���o�p�֐�
	void	SetRank( void );
	void	Production( void );						//���U���g�̉��o�p�֐�
	void	ProductionRotation( int playerNum );	//�R�C��������]�֐�
	void	ProductionCoinHandOff(NUMBER& number, int coinNum);	//�R�C�����������n��
};
//*******************************************************************************
#endif // !__SCENERESULT_H__
