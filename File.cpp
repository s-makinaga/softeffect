#include<stdio.h>
OPENFILENAME _FileOpenDialog( HWND );
int _FileOpen( HWND, OPENFILENAME );

OPENFILENAME _FileOpenDialog( HWND hEdit ){
	
	OPENFILENAME	ofn;
	
	memset( &ofn, 0, sizeof(OPENFILENAME) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hEdit;
	ofn.lpstrFilter = "wav(*.wav)\0";
	ofn.lpstrFile = szFileName;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "wav";

	if( GetOpenFileName(&ofn) == 0 ){
		//MessageBox( hEdit, "Error: FileOpen", "Error", MB_OK );	
		ErrorFlag = true;
		return ofn;
	}
	
	return ofn;
}

int _FileOpen( HWND hEdit, OPENFILENAME ofn ){

	HANDLE	hFile;
	DWORD	dwSize;
	
	FILE *pFile;
	
	hFile = CreateFile( szFileName,
			    GENERIC_READ,
			    0, NULL,
			    OPEN_ALWAYS,
			    FILE_ATTRIBUTE_NORMAL,
			    NULL );
	dwSize = GetFileSize( hFile, NULL );
	CloseHandle( hFile );

	/* ファイルオープン */
	if( ( pFile = fopen( szFileName, "rb" ) ) == NULL ){
		MessageBox( hEdit, szFileName, "Error", MB_OK );
		return -1;
	}
	
	MessageBox( hEdit, "Please wait file opening...", "Please wait", MB_OK );
	
	_GetWaveInfoHead( pFile );
	dWaveDataL = (int *)malloc( sizeof(int)*dwSize*4 );
	dWaveDataR = (int *)malloc( sizeof(int)*dwSize*4 );
	dEffDataL = (int *)malloc( sizeof(int)*dwSize*4 );
	dOldEffDataL = (int *)malloc( sizeof(int)*dwSize*4 );
	_GetWaveData( dWaveDataL, dWaveDataR, pFile );
	memcpy( dEffDataL, dWaveDataL, 4*tagWaveinfo.DataNum );
	
	fclose( pFile );
	
	MessageBox( hEdit, "Finished file open!!", "Finished", MB_OK );
	
	flagWaveShow = true;

	return 0;
}
