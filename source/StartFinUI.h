//*********************************************************************************
//
//	StartFinUI�N���X
//
//*********************************************************************************
class StartFinUI
{
private:
	int		count;
	Vector3 s_pos;
	Vector3 f_pos;
	float	s_t;
	int		s_step;
	int		waitTimer;
	bool	changeflag;

	ImageObj	s_obj;
	ImageObj	f_obj;

public:
	//	�������E���
	StartFinUI( void );
	~StartFinUI( void );

	//	�X�V
	void StartUpdate( void );
	bool StartMove( void );
	void FinishUpdate( void );

	//	�`��
	void StartRender( void );
	void FinishRender( void );

	//	���ݒ�E�擾
	void SetChangeFlag( const bool& flag );
	bool GetChangeFlag( void );

};