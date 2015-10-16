#include	<windows.h>

#include	"TextLoader.h"

//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

bool	textLoader::Open( LPSTR filename )
{
	FileBuf = NULL;
	//	�t�@�C���ǂݍ���
	HANDLE	hfile;
	DWORD	dum;
	hfile = CreateFile( filename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL );
	if( hfile == INVALID_HANDLE_VALUE ) return false;
	TextSize = GetFileSize( hfile, NULL );
	FileBuf = new char[TextSize];	
	ReadFile( hfile, FileBuf, TextSize, &dum, NULL );
	CloseHandle(hfile);

	ReadIndex = 0;

	return true;
}

bool	textLoader::EndCheck()
{
	if( ReadIndex < TextSize ) return true;
	return false;
}

void	textLoader::SetIndex( int index )
{
	ReadIndex = index;
}
//*****************************************************************************************************************************
//
//*****************************************************************************************************************************

textLoader::textLoader( LPSTR filename )
{
	bInit = Open(filename);
}

//--------------------------------------------------------------------------------------
//	���o��
//--------------------------------------------------------------------------------------
int	textLoader::SearchTop( void )
{
	for(;;){
		if( ReadIndex >= TextSize ) return -1;

		if( IsCharAlphaNumeric(FileBuf[ReadIndex]) ) break;
		if( IsDBCSLeadByte(FileBuf[ReadIndex]) ) break;

		if( FileBuf[ReadIndex] == '{' ) break;
		if( FileBuf[ReadIndex] == '}' ) break;
		if( FileBuf[ReadIndex] == '"' ) return 1;
		if( FileBuf[ReadIndex] == '.' ) break;
		if( FileBuf[ReadIndex] == '-' ) break;
		if( FileBuf[ReadIndex] == '=' ) break;
		ReadIndex ++;
		if( ReadIndex >= TextSize ) return -1;
	}
	return	0;
}

//
void	textLoader::LoadString( LPSTR str )
{
	int		len = 0;
	
	int mode = SearchTop();
	if( ReadIndex >= TextSize ) return;
	
	//	���O�ǂݍ��݃��[�h
	if( mode == 1 )
	{
		str[len] = '"';
		len   ++;
		ReadIndex ++;
		for(;;){
			if( IsDBCSLeadByte(FileBuf[ReadIndex]) ){ 
				str[len] = FileBuf[ReadIndex];
				len   ++;
				ReadIndex ++;
			} else {
				if( FileBuf[ReadIndex] == '"' ) break;
			}
			if( ReadIndex >= TextSize ) break;

			str[len] = FileBuf[ReadIndex];
			len   ++;
			ReadIndex ++;
		}
		str[len] = '"';
		len   ++;
		str[len] = '\0';
		ReadIndex ++;
		return;
	}


	for(;;){
		if( ReadIndex>0 && !IsDBCSLeadByte(FileBuf[ReadIndex-1]) ){ 
			if( FileBuf[ReadIndex] == ';' ) break;
			if( FileBuf[ReadIndex] == ')' ) break;
			if( FileBuf[ReadIndex] == '(' ) break;
			if( FileBuf[ReadIndex] == ',' ) break;
			if( FileBuf[ReadIndex] == ' ' ) break;
			if( FileBuf[ReadIndex] == '\t' ) break;
			if( FileBuf[ReadIndex] == 0x0d ) break;
			if( FileBuf[ReadIndex] == 0x0a ) break;
			if( ReadIndex >= TextSize ) break;
		}
		str[len] = FileBuf[ReadIndex];
		len   ++;
		ReadIndex ++;
	}
	str[len] = '\0';
	ReadIndex ++;
}

void	textLoader::LoadName( LPSTR str )
{
	int		len = 0;
	bool	bDQMode = false;

	SearchTop();
	if( ReadIndex >= TextSize ) return;

	if( FileBuf[ReadIndex] == '"' )
	{
		ReadIndex++;
		bDQMode = true;
	}

	for(;;){
		if( ReadIndex>0 && !IsDBCSLeadByte(FileBuf[ReadIndex-1]) ){ 
			if( FileBuf[ReadIndex] == '"' ){ ReadIndex++; break; }
			if( FileBuf[ReadIndex] == '(' ) break;
			if( FileBuf[ReadIndex] == ',' ) break;
			if( !bDQMode && FileBuf[ReadIndex] == ' ' ) break;
			if( FileBuf[ReadIndex] == '\t' ) break;
			if( FileBuf[ReadIndex] == 0x0d ) break;
			if( FileBuf[ReadIndex] == 0x0a ) break;
		}
		str[len] = FileBuf[ReadIndex];
		len   ++;
		ReadIndex ++;
	}
	str[len] = '\0';
	ReadIndex ++;
}

//
void	textLoader::LoadStringTest( LPSTR str )
{
	int		len = 0;

	SearchTop();
	if( ReadIndex >= TextSize ) return;

	DWORD	WorkIndex = ReadIndex;

	for(;;){
		if( WorkIndex>0 && !IsDBCSLeadByte(FileBuf[WorkIndex-1]) ){ 
			if( FileBuf[WorkIndex] == ')' ) break;
			if( FileBuf[WorkIndex] == '(' ) break;
			if( FileBuf[WorkIndex] == ',' ) break;
			if( FileBuf[WorkIndex] == ' ' ) break;
			if( FileBuf[WorkIndex] == '\t' ) break;
			if( FileBuf[WorkIndex] == 0x0d ) break;
			if( FileBuf[WorkIndex] == 0x0a ) break;
		}
		str[len] = FileBuf[WorkIndex];
		len   ++;
		WorkIndex ++;
	}
	str[len] = '\0';
}

//
float	textLoader::LoadFloat( void )
{
	float	ret;
	char	temp[20];
	LoadString(temp);

	ret = (float)atof(temp);
	return ret;
}

int		textLoader::LoadInt( void )
{
	int		ret;
	char	temp[20];
	LoadString(temp);

	ret = atoi(temp);
	return ret;
}

//		{}�ǂݔ�΂��I�I
void	textLoader::Skip( void )
{
	int		work = 0;

	if( FileBuf[ReadIndex] != '{' ) work = 1;

	for(;;){
		if( FileBuf[ReadIndex] == '{' ) work ++;
		if( FileBuf[ReadIndex] == '}' ){
			work--;
			if( work <= 0 ) break;
		}
		ReadIndex++;
	}
	ReadIndex++;
}

//		{�ɓ���
void	textLoader::In( void )
{
	for(;;){
		if( ReadIndex >= TextSize ) return;

		if( FileBuf[ReadIndex] == '{' ) break;
		ReadIndex++;
	}
	ReadIndex++;
}

//		{����o��
void	textLoader::Out( void )
{
	int		count = 0;
	for(;;){
		if( ReadIndex >= TextSize ) return;

		if( FileBuf[ReadIndex] == '{' ) count++;
		if( FileBuf[ReadIndex] == '}' ){
			count--;
			if( count < 0 ) break;
		}
		ReadIndex++;
	}
	ReadIndex++;
}



//------------------------------------------------------
//	����
//------------------------------------------------------
bool	textLoader::Search( LPSTR str )
{
	char	temp[128];

	while(EndCheck()){
		LoadString(temp);
		if( lstrcmp( temp, str ) == 0 ) return true;
	}
	return false;
}

//------------------------------------------------------
//	�s������
//------------------------------------------------------
bool	textLoader::SearchLine( LPSTR str )
{
	char	temp[128];
	DWORD	oldIndex = ReadIndex;
	
	//	�s���ݒ�
	for(;;)
	{
		ReadIndex --;
		if( FileBuf[ReadIndex] == '\n' ) break;
		if( FileBuf[ReadIndex] == '\r' ) break;
	}
	ReadIndex ++;
	//	�s������
	for(;;)
	{
		if( FileBuf[ReadIndex] == '\n' ) break;
		if( FileBuf[ReadIndex] == '\r' ) break;
		if( !IsCharAlphaNumeric(FileBuf[ReadIndex]) )
		{
			ReadIndex ++;
			continue;
		}
		LoadString(temp);
		if( lstrcmp( temp, str ) == 0 ) return true;
	}
	ReadIndex = oldIndex;
	return false;
}

//------------------------------------------------------
//	�K�w������
//------------------------------------------------------
bool	textLoader::Search2( LPSTR str )
{
	char	temp[128];
	int		work = 0;

	if( FileBuf[ReadIndex] != '{' ) work = 1;
	while(EndCheck()){
		LoadString(temp);
		if( lstrcmp( temp, str ) == 0 ) return true;

		if( lstrcmp( temp, "{" ) == 0 ){
			work++;
		}
		if( lstrcmp( temp, "}" ) == 0 ){
			work--;
			if( work <= 0 ) return false;
		}
	}
	return false;
}

//------------------------------------------------------
//	����C���f���g������
//------------------------------------------------------
bool	textLoader::SearchIndent( LPSTR str )
{
	char	temp[128];
	int		work = 0;

	DWORD	oldIndex = ReadIndex;

	for(;;)
	{
		LoadString(temp);
		if( ReadIndex >= TextSize )
		{
			ReadIndex = oldIndex;
			return false;
		}

		if( lstrcmp( temp, str ) == 0 ) return true;

		if( lstrcmp( temp, "{" ) == 0 ){
			Skip();
			continue;
		}
		if( lstrcmp( temp, "}" ) == 0 ){
			work--;
			if( work <= 0 )
			{
				ReadIndex = oldIndex;
				return false;
			}
		}
	}
	ReadIndex = oldIndex;
	return false;
}


void	textLoader::push()
{
	PushIndex = ReadIndex;
}

void	textLoader::pop()
{
	ReadIndex = PushIndex;
}

void	textLoader::newLine()
{
	for(;;){
		if( ReadIndex >= TextSize ) return;
		if( FileBuf[ReadIndex] == '\n' ) break;
		if( FileBuf[ReadIndex] == '\r' ) break;
		ReadIndex++;
	}
}

//*****************************************************************************************************************************
//
//*****************************************************************************************************************************

