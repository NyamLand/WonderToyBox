#include	"iextreme.h"

//**************************************************************************************************
//
//		�X�L�����b�V���֘A
//
//**************************************************************************************************

//*****************************************************************************
//	�t���[���X�V
//*****************************************************************************
void	iex3DObj::UpdateSkinMeshFrame( float frame )
{
	u32			i, j;
	LPIEXANIME2	lpAnime;
	float		t;

	for( i=0 ; i<NumBone ; i++ ){
		lpAnime = &this->lpAnime[i];

		//	�|�[�Y�ݒ�
		if( lpAnime->rotNum == 0 ) CurPose[i] = orgPose[i];
		 else if( lpAnime->rotNum == 1 ) CurPose[i] = lpAnime->rot[0];
		 else {
			//	��]�L�[���
			for( j=0 ; j<lpAnime->rotNum-1 ; j++ )
			{
				//	���݈ʒu����
				if( (frame>=lpAnime->rotFrame[j]) && (frame<lpAnime->rotFrame[j+1]) )
				{
					//	�o�߃t���[���v�Z
					t = (float)(frame-lpAnime->rotFrame[j]) / (float)(lpAnime->rotFrame[j+1] - lpAnime->rotFrame[j]);
					//	���
					CurPose[i] = QuaternionSlerp( lpAnime->rot[j], lpAnime->rot[j+1], t );
					break;
				}
			}
			if( j == lpAnime->rotNum-1 ) CurPose[i] = lpAnime->rot[lpAnime->rotNum-1];
		}
		//	���W�ݒ�
		if( lpAnime->posNum == 0 ) CurPos[i] = orgPos[i];
		 else {
			//	�ʒu���
			for( j=0 ; j<lpAnime->posNum-1 ; j++ )
			{
				//	���݈ʒu����
				if( (frame>=lpAnime->posFrame[j]) && (frame<lpAnime->posFrame[j+1]) )
				{
					//	�o�߃t���[���v�Z
					t = (float)(frame-lpAnime->posFrame[j]) / (float)(lpAnime->posFrame[j+1] - lpAnime->posFrame[j]);
					//	���
					CurPos[i] = lpAnime->pos[j] + (lpAnime->pos[j+1]-lpAnime->pos[j])*t;
					break;
				}
			}
			if( j == lpAnime->posNum-1 ) CurPos[i] = lpAnime->pos[lpAnime->posNum-1];
		}
	}
}

//*****************************************************************************
//	�{�[���s��X�V
//*****************************************************************************
void iex3DObj::UpdateBoneMatrix()
{
	u32			i;

	//	�{�[���X�V
	for( i=0 ; i<NumBone ; i++ )
	{
		CurPose[i].toMatrix( lpBoneMatrix[i] );
		//	�ʒu���R�s�[
		lpBoneMatrix[i]._41 = CurPos[i].x;
		lpBoneMatrix[i]._42 = CurPos[i].y;
		lpBoneMatrix[i]._43 = CurPos[i].z;

		if( BoneParent[i] != 0xFFFF ) lpBoneMatrix[i] *= lpBoneMatrix[ BoneParent[i] ];
	}
	for( i=0 ; i<NumBone ; i++ )
	{
		lpMatrix[i] = lpOffsetMatrix[i] * lpBoneMatrix[i];
	}
}


//*****************************************************************************
//	�X�L�����b�V���X�V
//*****************************************************************************
void	iex3DObj::UpdateSkinMesh()
{
	//	���b�V���X�V
	void*	pVertex;
	lpMesh->LockVertexBuffer( D3DLOCK_DISCARD, &pVertex );
	lpSkinInfo->UpdateSkinnedMesh( lpMatrix, NULL, lpVertex, pVertex );
	lpMesh->UnlockVertexBuffer();
}

