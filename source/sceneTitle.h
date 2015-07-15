
#ifndef __SCENETITLE_H__
#define	__SCENETITLE_H__

//*****************************************************************************
//
//	sceneTitle�N���X
//
//*****************************************************************************

class sceneTitle : public	Scene
{
private:
	//iexView*	view;

	//	�I�u�W�F�N�g
	iexMesh*	m_CollisionStage;
	iexMesh*	m_Stage;	
	int			testpos;	//	���̃J�����ړ���̃|�W�V�������w�������l
	float		t;			//	���[�v�̂�
	Vector3		c_pos;		//	�J�����|�W�V����
	Vector3		s_pos;		//	���[�v�X�^�[�g�|�W�V����
	Vector3		t_pos;		//	�J�����̃^�[�Q�b�g�|�W�V����
	
	Quaternion orientation;	//	�N�H�[�^�j�I��


public:
	//	�������E���
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );
	void	InitSound( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
};

//*****************************************************************************
#endif // !__SCENETITLE_H__
