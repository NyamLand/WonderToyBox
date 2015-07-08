#include "iextreme.h"

# pragma comment(lib, "IEX\\OGG\\vorbis_static.lib")
# pragma comment(lib, "IEX\\OGG\\ogg_static.lib")
# pragma comment(lib, "IEX\\OGG\\vorbisfile_static.lib")

//**************************************************************************************************************
//
//		�T�E���h�o�b�t�@
//
//**************************************************************************************************************

//**************************************************************************************************************
//
//**************************************************************************************************************
SoundBuffer::SoundBuffer( LPDIRECTSOUND lpDS, char* filename, bool b3D )
{
	DSBUFFERDESC	dsbd;
	LPVOID			lpbuf1, lpbuf2;
	DWORD			dwbuf1, dwbuf2;

	WAVEFORMATEX	wfx;
	LPBYTE			lpWBuf;
	DWORD			size;

	lpBuf3D = NULL;
	lpBuf = NULL;

	/*	WAV�t�@�C���̃��[�h	*/ 
	lpWBuf = LoadWAV( filename, &size, &wfx );
	/*	���[�h���s	*/ 
	if( lpWBuf == NULL ){
		return;
	}
    
	/* �񎟃o�b�t�@�쐬	*/ 
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	if( b3D == true ) dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRL3D; 
	else dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME; 
 	dsbd.dwBufferBytes = size;
	dsbd.lpwfxFormat = &wfx; 
	if( lpDS->CreateSoundBuffer( &dsbd, &lpBuf, NULL) != DS_OK)
	{
		return;
	}

	lpBuf3D = NULL;
	if( b3D == true )
	{
		lpBuf->QueryInterface( IID_IDirectSound3DBuffer, (LPVOID*)&lpBuf3D );
	}

	/* �񎟃o�b�t�@�̃��b�N	*/ 
	lpBuf->Lock( 0, size,&lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0);
	/* �����f�[�^�̐ݒ�	*/ 
	memcpy(lpbuf1, lpWBuf, dwbuf1);
	if(dwbuf2 != 0) memcpy(lpbuf2,lpWBuf+dwbuf1, dwbuf2);
	/* �����f�[�^�̉��	*/ 
	GlobalFree(lpWBuf);
	/* �񎟃o�b�t�@�̃��b�N����	*/ 
	lpBuf->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2); 
}

SoundBuffer::~SoundBuffer()
{
	if( lpBuf != NULL )lpBuf->Release();
	if( lpBuf3D != NULL ) lpBuf3D->Release();

}

//**************************************************************************************************************
//		�v�`�u�t�@�C���̓ǂݍ���
//**************************************************************************************************************
LPBYTE SoundBuffer::LoadWAV( LPSTR fname, LPDWORD size, LPWAVEFORMATEX wfx)
{
	LPBYTE			buf = NULL;			/*	�ǂݍ��݃o�b�t�@	*/ 
	HMMIO			hMMIO= NULL;		/*	�t�@�C���n���h��	*/ 
	PCMWAVEFORMAT	pwf;				/*	WAV�f�[�^�`��		*/ 
	MMCKINFO		ckparent, ckinfo;	/*	RIFF�`�����N���	*/ 
	MMIOINFO		mminfo;				/*	�t�@�C�����		*/ 
	DWORD			i;

	/* �I�[�v��	*/ 
	if( (hMMIO = mmioOpen( fname, NULL, MMIO_ALLOCBUF | MMIO_READ)) == NULL) return NULL;
	if( mmioDescend(hMMIO, &ckparent, NULL, 0) != 0) goto WAVE_LoadError;
	/*	�v�`�u(RIFF)�t�@�C���`�F�b�N		*/ 
	if( (ckparent.ckid!=FOURCC_RIFF) || (ckparent.fccType!=mmioFOURCC('W', 'A', 'V', 'E')) ) goto WAVE_LoadError;
	/*	�������`�����N�ɐN��		*/ 
	ckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if( mmioDescend(hMMIO, &ckinfo, &ckparent, MMIO_FINDCHUNK) != 0) goto WAVE_LoadError;
	if( ckinfo.cksize < sizeof(PCMWAVEFORMAT)) goto WAVE_LoadError;
    /*	�`�����N���烊�[�h	*/ 
	if( mmioRead(hMMIO, (HPSTR)&pwf, sizeof(pwf)) != sizeof(pwf)) goto WAVE_LoadError;
	if( pwf.wf.wFormatTag != WAVE_FORMAT_PCM) goto WAVE_LoadError;
	/*	WAV�t�H�[�}�b�g�̕ۑ�	*/ 
	memset( wfx,0,sizeof(WAVEFORMATEX));
	memcpy( wfx, &pwf, sizeof(pwf));
	/*	�f�[�^�̓ǂݍ���	*/ 
	if( mmioSeek(hMMIO, ckparent.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1) goto WAVE_LoadError;
	/*	���������`�����N�ɐN��		*/ 
	ckinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if( mmioDescend(hMMIO, &ckinfo, &ckparent, MMIO_FINDCHUNK) != 0) goto WAVE_LoadError;
	if( mmioGetInfo(hMMIO, &mminfo, 0) != 0) goto WAVE_LoadError;
	/*	�o�b�t�@�T�C�Y�ۑ�	*/ 
	if( size != NULL) *size = ckinfo.cksize;
	/*	�v�`�u�p�o�b�t�@�̎擾	*/ 
	buf = (LPBYTE)GlobalAlloc( LPTR, ckinfo.cksize );
	if( buf == NULL) goto WAVE_LoadError;
	/*	�f�[�^�̓ǂ݂Ƃ�	*/ 
	for(i = 0; i < ckinfo.cksize; i++){
		/*	�G���[�`�F�b�N	*/ 
		if( mminfo.pchNext >= mminfo.pchEndRead ){
			if( mmioAdvance(hMMIO, &mminfo, MMIO_READ) != 0 ) goto WAVE_LoadError;
			if( mminfo.pchNext >= mminfo.pchEndRead ) goto WAVE_LoadError;
		}
		*(buf + i) = *((LPBYTE)mminfo.pchNext);
		mminfo.pchNext++;
	}
	/*	�t�@�C���A�N�Z�X�I��	*/ 
	mmioSetInfo(hMMIO, &mminfo, 0);
	mmioClose(hMMIO, 0);
	return buf;

WAVE_LoadError:	/*	�G���[�I��	*/ 
	mmioClose(hMMIO, 0);
	if( buf != NULL ) GlobalFree(buf);
	return NULL;
}


//**************************************************************************************************************
//	���ݒ�
//**************************************************************************************************************
//-------------------------------------------------------------
//	�ʒu�ݒ�	
//-------------------------------------------------------------
void	SoundBuffer::SetPos( Vector3* pos )
{
	if( lpBuf3D == NULL ) return;
	lpBuf3D->SetPosition( pos->x, pos->y, pos->z, DS3D_IMMEDIATE );
}

//**************************************************************************************************************
//	�Đ��Ǘ�
//**************************************************************************************************************
//-------------------------------------------------------------
//	�Đ�	
//-------------------------------------------------------------
void SoundBuffer::Play( BOOL loop )
{
	lpBuf->Stop();
	lpBuf->SetCurrentPosition(0);
	//	���[�v�Đ�
	if( loop ) lpBuf->Play(0, 0, DSBPLAY_LOOPING);
	//	�m�[�}���Đ�
	 else	   lpBuf->Play(0, 0, 0);

//	lpBuf->SetFrequency(8000);

}

//-------------------------------------------------------------
//	��~	
//-------------------------------------------------------------
void SoundBuffer::Stop()
{
	lpBuf->Stop();
}

//-------------------------------------------------------------
//	�{�����[���ύX
//-------------------------------------------------------------
void	SoundBuffer::SetVolume( int volume )
{
	lpBuf->SetVolume( volume );
}

//-------------------------------------------------------------
//	�Đ��`�F�b�N	
//-------------------------------------------------------------
BOOL	SoundBuffer::isPlay()
{
	DWORD	dwAns;
	lpBuf->GetStatus( &dwAns );
	if( (dwAns&DSBSTATUS_PLAYING) != 0 ) return TRUE;
	 else return FALSE;
}

//**************************************************************************************************************
//
//		�X�g���[���T�E���h
//
//**************************************************************************************************************

static int	NumStream = 0;
static bool ForceEnd = false;

//**************************************************************************************************************
//		�Ǘ��X���b�h
//**************************************************************************************************************
DWORD WINAPI Thread( LPDWORD lpdwParam )
{
	DWORD	param;
	iexStreamSound*	lpStream;

	lpStream = (iexStreamSound*)(lpdwParam);
	for(;;){
		if( lpStream == NULL ) break;
		param = WaitForMultipleObjects( 3, lpStream->hEvent, FALSE, 100 );

		if( ForceEnd == true ) param = -1;

		switch(param){
			case WAIT_OBJECT_0:		if( lpStream->type == TYPE_WAV ) lpStream->SetBlockWAV( 1 );
									if( lpStream->type == TYPE_OGG ) lpStream->SetBlockOGG( 1 );
									break;
			case WAIT_OBJECT_0+1:	if( lpStream->type == TYPE_WAV ) lpStream->SetBlockWAV( 0 );
									if( lpStream->type == TYPE_OGG ) lpStream->SetBlockOGG( 0 );
									break;

			case WAIT_TIMEOUT:		switch( lpStream->GetMode() ){
									case STR_NORMAL:		break;
									case STR_FADEIN:	lpStream->SetVolume( lpStream->GetVolume() + lpStream->GetParam() );
														if( lpStream->GetVolume() >= 255 ) lpStream->SetMode(STR_NORMAL,0); 
														break;
									case STR_FADEOUT:	lpStream->SetVolume( lpStream->GetVolume() - lpStream->GetParam() );
														if( lpStream->GetVolume() <= 0 ){
															lpStream->Stop();
															delete lpStream;
															ExitThread(TRUE);
															return 0;
														}
														break;
									}
									break;

			default:
									lpStream->Stop();
									delete lpStream;
									ExitThread(TRUE);
									return 0;

		}
	}
	return 0;
}

//**************************************************************************************************************
//
//**************************************************************************************************************
iexStreamSound::iexStreamSound( LPDIRECTSOUND lpDS, LPSTR filename, BYTE mode, int param )
{
	NumStream ++;

	LPSTR	work;
	BOOL	bInit;
	//	�X�g���[���t�@�C�����J��
	work = &filename[ lstrlen(filename) - 4 ];
	if( lstrcmpi( work, ".wav" ) == 0 ) bInit = SetWAV( lpDS, filename );
	if( lstrcmpi( work, ".ogg" ) == 0 ) bInit = SetOGG( lpDS, filename );

	lpStream->SetCurrentPosition( 0 );
	if( mode != STR_FADEIN ) SetVolume(255);
	else SetVolume(0);
	/*	�Ǘ��X���b�h�̍쐬	*/ 
	hStrThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Thread, this, 0, &dwThreadId );
	if( hStrThread == NULL ) return;
	/*	�Đ��J�n	*/ 
	lpStream->Play( 0, 0, DSBPLAY_LOOPING );

	this->mode  = mode;
	this->param = param;
}



iexStreamSound::~iexStreamSound()
{
	if( lpStream != NULL ){
		if( type == TYPE_OGG ) ov_clear(&vf);
		 else if( hStrFile != NULL ) fclose( hStrFile );

		if( lpStrNotify != NULL ) lpStrNotify->Release();
		lpStrNotify = NULL;
		/*	�o�b�t�@�J��	*/ 
		if( lpStream != NULL ) lpStream->Release();
		lpStream = NULL;
	}
	
	NumStream --;
}

//**************************************************************************************************************
//
//**************************************************************************************************************
//-------------------------------------------------------------
//	�u���b�N�쐬
//-------------------------------------------------------------
BOOL	iexStreamSound::OGGRead( LPBYTE dst, unsigned long size )
{
	DWORD	remain = size;
	char*	dstPtr = (char*)dst;
	while( remain > 0 ){
		long actualRead = ov_read( &vf, dstPtr, remain, 0, 2, 1, NULL);
		//�I�[�`�F�b�N
		if(actualRead <= 0){
			if( ov_pcm_seek( &vf, 0) ) return FALSE;
		}
		remain -= actualRead;
		dstPtr += actualRead;
	}
	return TRUE;
}

BOOL	iexStreamSound::SetBlockOGG( int block )
{
	LPBYTE	blk1, blk2;
	DWORD	bs1, bs2;

	DWORD	CurPos;

	CurPos = StrSize - StrPos;
	//	�o�b�t�@���b�N
	lpStream->Lock( (rate*4*STRSECOND)*block, rate*4*STRSECOND, (LPVOID*)&blk1, &bs1, (LPVOID*)&blk2, &bs2, 0 );
	//	�u���b�N�P�ւ̃f�[�^�Z�b�g
	OGGRead( blk1, bs1 );
	StrPos += bs1;
	//	�u���b�N�Q�ւ̃f�[�^�Z�b�g
	if( blk2 ){
		OGGRead( blk2, bs2 );
		StrPos += bs2;
	}

	lpStream->Unlock( blk1,bs1, blk2,bs2 );
	return TRUE;
}


BOOL	iexStreamSound::SetBlockWAV( int block )
{
	LPBYTE	blk1, blk2;
	DWORD	bs1, bs2, work;

	DWORD	CurPos;

	CurPos = StrSize - StrPos;
	lpStream->Lock( (rate*4*STRSECOND)*block, rate*4*STRSECOND, (LPVOID*)&blk1, &bs1, (LPVOID*)&blk2, &bs2, 0 );

	work = fread( blk1, bs1, 1, hStrFile );
	StrPos += work;
	if( work < bs1 ){
		/*	�����߂�	*/ 
		fseek(hStrFile, LoopPtr, SEEK_SET);
		work = fread( blk1+work, bs1-work, 1, hStrFile );
		StrPos = work;
	}

	if( blk2 ){
		work = fread( blk2, bs2, 1, hStrFile );
		StrPos += work;
		if( work < bs2 ){
			/*	�����߂�	*/ 
			fseek(hStrFile, LoopPtr, SEEK_SET);
			work = fread( blk2+work, bs2-work, 1, hStrFile );
			StrPos = work;
		}
	}

	lpStream->Unlock( blk1,bs1, blk2,bs2 );
	return TRUE;
}


//**************************************************************************************************************
//	�f�[�^�Ǘ�
//**************************************************************************************************************
void iexStreamSound::Initialize( LPDIRECTSOUND lpDS, int rate )
{
	DSBUFFERDESC	dsbd;
	WAVEFORMATEX	wfx;


	/*	�������`�F�b�N	*/ 
	if( lpDS == NULL ) return;
	/*	�v�`�u�d�t�H�[�}�b�g������	*/ 
	ZeroMemory( &wfx, sizeof(WAVEFORMATEX) );
	wfx.wFormatTag      = WAVE_FORMAT_PCM;
	wfx.nChannels       = 2;			/*	stereo	*/ 
	wfx.nSamplesPerSec  = rate;
	wfx.wBitsPerSample  = 16;			/*	16bit	*/ 
	wfx.nBlockAlign     = 4;
	wfx.nAvgBytesPerSec = rate * 4;
	/* �񎟃o�b�t�@�̏�����	*/ 
	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize        = sizeof(DSBUFFERDESC);
	dsbd.dwFlags       = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_LOCSOFTWARE;
	dsbd.dwBufferBytes = rate*4 * STRSECOND*2;
	dsbd.lpwfxFormat   = &wfx; 
	if( lpDS->CreateSoundBuffer( &dsbd, &lpStream, NULL) != DS_OK) return;
	lpStream->SetFormat(&wfx);

	if(	lpStream->QueryInterface( IID_IDirectSoundNotify, (LPVOID*)&lpStrNotify ) != DS_OK ) return;
	/*	�ʒu�C�x���g�쐬	*/ 
	hEvent[0] = CreateEvent( NULL, FALSE, FALSE, NULL );
	hEvent[1] = CreateEvent( NULL, FALSE, FALSE, NULL );
	hEvent[2] = CreateEvent( NULL, FALSE, FALSE, NULL );

	pn[0].dwOffset     = 0;
	pn[0].hEventNotify = hEvent[0];
	pn[1].dwOffset     = rate*4 * STRSECOND;
	pn[1].hEventNotify = hEvent[1];

	pn[2].dwOffset     = DSBPN_OFFSETSTOP;
	pn[2].hEventNotify = hEvent[2];

	if( lpStrNotify->SetNotificationPositions( 3, pn ) != DS_OK ) return; 

	this->rate = rate;
	return;
}


BOOL	iexStreamSound::SetWAV(  LPDIRECTSOUND lpDS, char* filename )
{
	HMMIO			hMMIO= NULL;		/*	�t�@�C���n���h��	*/ 
	MMCKINFO		ckinfo, ckparent;	/*	RIFF�`�����N���	*/ 
	LRESULT			ptr;

	/* �I�[�v��	*/ 
	hMMIO = mmioOpen( filename, NULL, MMIO_ALLOCBUF | MMIO_READ);
	mmioDescend(hMMIO, &ckparent, NULL, 0);
	/*	���������`�����N�ɐN��		*/ 
	ckinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMMIO, &ckinfo, &ckparent, MMIO_FINDCHUNK);
	//	���݈ʒu�擾
	ptr = mmioSeek(hMMIO, 0, SEEK_CUR);
	/*	�t�@�C���A�N�Z�X�I��	*/ 
	mmioClose(hMMIO, 0);
	if( ptr == -1 ) return FALSE;

	/*	�t�@�C���I�[�v��	*/ 
	hStrFile = fopen( filename, "rb" );
	if( hStrFile == NULL ) return FALSE;
	//	�X�g���[�����ݒ�
	StrPos  = 0;
	LoopPtr = ptr;

	fseek(hStrFile, 0L, SEEK_END);
	StrSize = ftell(hStrFile) - LoopPtr;

	StrSize = GetFileSize( hStrFile, NULL ) - LoopPtr;
	//	�t�@�C���V�[�N
	fseek(hStrFile, LoopPtr, SEEK_SET);

	type = TYPE_WAV;
	Initialize( lpDS, 22050);

	/*	�Đ�����	*/ 
	SetBlockWAV(0);

	return TRUE;
}

//
//	OGG�p�X�g���[��������
//

BOOL	iexStreamSound::SetOGG( LPDIRECTSOUND lpDS, char* filename )
{
	//	�t�@�C���I�[�v�� 
	hStrFile = fopen( filename, "rb" );
	if( hStrFile == NULL ) return FALSE;

	//Ogg���J��
	ov_open( hStrFile, &vf, NULL, 0 );

	//�V�[�N�\?
	if( !ov_seekable(&vf) ){
		ov_clear(&vf);
		return FALSE;
	}

	//���ݒ�
	const vorbis_info *info = ov_info(&vf, -1);
	Initialize(lpDS, info->rate);

	type = TYPE_OGG;
	StrPos  = 0;
	StrSize = (DWORD)ov_pcm_total(&vf, -1);

	/*	�Đ�����	*/ 
	SetBlockOGG(0);

	return TRUE;
}


//**************************************************************************************************************
//	�Đ��Ǘ�
//**************************************************************************************************************

void iexStreamSound::Stop()
{
	if( lpStream == NULL ) return;
	if( hStrFile == NULL ) return;

	lpStream->Stop();
}

void iexStreamSound::SetVolume( int volume )
{
	int		vol;
	if( lpStream == NULL ) return;
	/*	���ʃZ�b�g	*/
	if( volume > 255 ) volume = 255;
	if( volume < 0   ) volume = 0;
	this->volume = volume;
	volume -= 255;
	vol = (volume*volume*100 / (255*255) );
	lpStream->SetVolume( -vol*vol );
}

void iexStreamSound::SetMode( BYTE mode, int param )
{
	this->mode  = mode;
	this->param = param;
}




//**************************************************************************************************************
//
//		�T�E���h�}�l�[�W��
//
//**************************************************************************************************************

//**************************************************************************************************************
//
//**************************************************************************************************************
iexSound::iexSound()
{
	hWndWAV = iexSystem::Window;
	CoInitialize(NULL);
	//	DirectSound�̏�����
	if( DirectSoundCreate8( NULL, &lpDS, NULL ) != DS_OK ){
		lpDS = NULL;
		return;
	}

	lpDS->SetCooperativeLevel( hWndWAV, DSSCL_PRIORITY );
	for( int i=0 ; i<WavNum ; i++ ) buffer[i] = NULL;

	lpPrimary = NULL;
	DSBUFFERDESC	dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	lpDS->CreateSoundBuffer(&dsbd, &lpPrimary, NULL);

	lp3DListener = NULL;
	lpPrimary->QueryInterface( IID_IDirectSound3DListener, (LPVOID *)lp3DListener);

	WAVEFORMATEX   wfx;   
	ZeroMemory( &wfx, sizeof(WAVEFORMATEX) );
	wfx.wFormatTag		= WAVE_FORMAT_PCM;     
	wfx.nChannels		= 1;     
	wfx.nSamplesPerSec	= 22050;     
	wfx.wBitsPerSample	= 16;     
	wfx.nBlockAlign		= wfx.wBitsPerSample / 8 * wfx.nChannels;   
	wfx.nAvgBytesPerSec	= wfx.nSamplesPerSec * wfx.nBlockAlign;   
	lpPrimary->SetFormat(&wfx);

}

iexSound::~iexSound()
{
	int		i;

	//	�o�b�t�@�̉��
	for(i=0 ; i<WavNum ; i++){
		if(buffer[i] != NULL) delete buffer[i];
		buffer[i] = NULL;
	}

	//	Direct Sound���
	if(lpPrimary != NULL) lpPrimary->Release();
	if(lpDS != NULL) lpDS->Release();
	
	lpDS = NULL;
	lpPrimary = NULL;
}

//**************************************************************************************************************
//
//**************************************************************************************************************
void	iexSound::Set( int no, char* filename, bool b3D )
{
	//	�������`�F�b�N
	if( lpDS == NULL ) return;
	//	�����̃o�b�t�@�̉��
	if( buffer[no] != NULL ) delete buffer[no];
	buffer[no] = NULL;
	//	WAV�t�@�C���̃��[�h
	if( filename != NULL )
	{
		buffer[no] = new SoundBuffer( lpDS, filename, b3D );
		if( buffer[no]->GetBuf() == NULL )
		{
			delete buffer[no];
			buffer[no] = NULL;
		}
	}
}

//**************************************************************************************************************
//
//**************************************************************************************************************

void iexSound::Play( int no, BOOL loop )
{
	//	�������`�F�b�N
	if( lpDS == NULL ) return;
	//	�f�[�^�������I�I
	if( buffer[no] == NULL ) return;

	buffer[no]->Play(loop);
}

void iexSound::Stop( int no )
{
	//	�������`�F�b�N
	if( lpDS == NULL ) return;
	//	�f�[�^�������I�I
	if( buffer[no] == NULL ) return;

	buffer[no]->Stop();
}


/*	�{�����[���̐ݒ�	*/ 
void	iexSound::SetVolume( int no, int volume )
{
	//	�������`�F�b�N
	if( lpDS == NULL ) return;
	//	�f�[�^�������I�I
	if( buffer[no] == NULL ) return;
	//	���ʃZ�b�g
	buffer[no]->SetVolume( volume );
}


/*	�Đ��󋵂̃`�F�b�N	*/ 
BOOL	iexSound::isPlay( int no )
{
	//	�������`�F�b�N
	if( lpDS == NULL ) return FALSE;
	//	�f�[�^�������I�I
	if( buffer[no] == NULL ) return FALSE;
	
	return buffer[no]->isPlay();
}


void	iexSound::SetListener( Vector3* pos, Vector3* target )
{
	if( lp3DListener == NULL ) return;
	//	�ʒu�ݒ�
	lp3DListener->SetPosition( pos->x, pos->y, pos->z, DS3D_DEFERRED );
	//	�����ݒ�
	Vector3 dir = *target - *pos;
	//D3DXVec3Normalize(&dir, &dir);
	lp3DListener->SetOrientation( dir.x, dir.y, dir.z, 0, 1, 0, DS3D_DEFERRED );

	//	�K�p
	lp3DListener->CommitDeferredSettings();
}

//**************************************************************************************************************
//	�X�g���[���T�E���h�Ǘ�
//**************************************************************************************************************
iexStreamSound* iexSound::PlayStream( char* filename )
{
	return PlayStream( filename, STR_NORMAL, 0 );
}

iexStreamSound* iexSound::PlayStream( char* filename, BYTE mode, int param )
{
	iexStreamSound*	lpStream;

	//	�������`�F�b�N
	if( lpDS == NULL ) return NULL;

	lpStream = new iexStreamSound( lpDS, filename, mode, param );
	return lpStream;
}

















//*****************************************************************************
//
//	�ߋ��̂h�d�w�Ƃ̌݊��p
//
//*****************************************************************************

static iexSound*	sound;

iexSound*	IEX_GetSound(){ return sound; }

//**************************************************************************************************************
//
//		�c�����������@�r���������̏�����
//
//**************************************************************************************************************

BOOL	IEX_InitAudio()
{
	sound = new iexSound();
	return TRUE;
}

//**************************************************************************************************************
//
//		�c�����������@�r���������̉��
//
//**************************************************************************************************************

void	IEX_ReleaseAudio()
{
	ForceEnd = true;
	while( NumStream > 0 );
	delete	sound;
	sound = NULL;
}

//**************************************************************************************************************
//
//		�c�����������@�r���������@�o�b�t�@�̍Đ�
//
//**************************************************************************************************************

//
//		�Đ��֘A
//

/*	�o�b�t�@����	*/ 
BOOL	IEX_SetWAV( int no, LPSTR filename, bool b3D )
{
	sound->Set( no, filename, b3D );
	return TRUE;
}

/*	�Đ�	*/ 
void	IEX_PlaySound( int no, BOOL loop )
{
	sound->Play( no, loop );
}

/*	��~	*/ 
void	IEX_StopSound( int no )
{
	sound->Stop(no);
}

//
//		��ԕύX�E�擾
//

/*	�{�����[���̐ݒ�	*/ 
void	IEX_SetSoundVolume( int no, int volume )
{
	sound->SetVolume( no, volume );
}

/*	�Đ��󋵂̃`�F�b�N	*/ 
BOOL	IEX_GetSoundStatus( int no )
{
	return sound->isPlay(no);
}

//**************************************************************************************************************
//
//		�c�����������@�r���������@�o�b�t�@�̃X�g���[���֘A
//
//**************************************************************************************************************

//
//		��~
//

BOOL	IEX_StopStreamSound( LPDSSTREAM lpStream )
{
	lpStream->Stop();
	return TRUE;
}

//
//		�Đ�
//

LPDSSTREAM	IEX_PlayStreamSoundEx( LPSTR filename, BYTE mode, int param )
{
	return sound->PlayStream( filename, mode, param );
}

LPDSSTREAM	IEX_PlayStreamSound( LPSTR filename )
{
	return IEX_PlayStreamSoundEx( filename, STR_NORMAL, 0 );
}

void	IEX_SetStreamMode( LPDSSTREAM lpStream, BYTE mode, int param )
{
	lpStream->SetMode( mode, param );
}

/*	�{�����[���ύX	*/ 
void	IEX_SetStreamSoundVolume( LPDSSTREAM lpStream, int volume )
{
	lpStream->SetVolume(volume);
}

