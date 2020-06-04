#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>

/*------------------------------------------------------
  指定したwaveファイルのヘッダ情報を構造体に格納
  [引数] _WAVEHEADINFO	ここで指定した構造体に値を格納
  	 FILE		WAVEファイルのファイルポインタ                              
------------------------------------------------------*/

typedef struct{
	char	RiffHead[5];
	long	RiffHeadSize;
	char	WaveHead[5];
	char	FileFormat[5];
	long	FileFormatSize;
	short	FormatID;
	short	Channels;
	long	Samples;
	long	DataSpeed;
	short	BlockSize;
	short	SampleBit;
	char	FactChank[5];
	long	FactChankSize;
	long	AllData;
	char	DataChank[5];
	long	DataSize;
	long	Offset;
	int	DataNum;
} _WAVEHEADINFO;

_WAVEHEADINFO tagWaveinfo;

/*------------------------------------------------------------------------------*/

/*-----------------------------*/
/* 4Byte 読み込み 10進数に変換 */
/*-----------------------------*/
unsigned long _getUlong(FILE *fp){
	
	unsigned char s[4];

	if (fread(s, 4, 1, fp) != 1) {
		return 0;
	}

	return (s[0] + 256L * (s[1] + 256L * (s[2] + 256L * s[3])));
}

/*-----------------------------*/
/* 2Byte 読み込み 10進数に変換 */
/*-----------------------------*/
unsigned short _getUshort(FILE *fp){
	
	unsigned char s[2];

	if (fread(s, 2, 1, fp) != 1) {
		return 0;
	}

	return (s[0] + (256U * s[1]));
}                     		

/*-----------------------------*/
/* 1Byte 読み込み 10進数に変換 */
/*-----------------------------*/
unsigned int _getUbyte(FILE *fp){
	
	unsigned char s[1];

	if (fread(s, 1, 1, fp) != 1) {
		return 0;
	}

	return (s[0]);
}

/*-------------*/
/* fmtチャンク */
/*-------------*/
void _fmtChunkRead( FILE *fp ){
	
	short	tag;
	
	short	FormatID;
	short	Channels;
	long	Samples;
	long	DateSpeed;
	short	BlockSize;
	short	SampleBit;
	
	/* wFormatTag 2bytes */
	tag = _getUshort( fp );
	tagWaveinfo.FormatID = tag;
	/* チャンネル 2bytes */
	tagWaveinfo.Channels = _getUshort(fp);
	/* サンプリングレート 4bytes */
	tagWaveinfo.Samples = _getUlong(fp);
	/* データ速度 4bytes*/
	tagWaveinfo.DataSpeed = _getUlong(fp);
	/* ブロックサイズ (Byte/sample×チャンネル数) 2bytes */
	tagWaveinfo.BlockSize = _getUshort(fp);
	/* サンプルあたりのビット数 (bit/sample) 2bytes */
	tagWaveinfo.SampleBit = _getUshort(fp);
}

/*--------------*/
/* factチャンク */
/*--------------*/
void _factChunkRead( FILE *fp ){
	
	tagWaveinfo.AllData = _getUlong(fp);
}


/*-----------------------------*/
/* Main */
/*-----------------------------*/
int _GetWaveInfoHead( FILE *fp ){

	unsigned char ID[5];
	long cursor, size;
	int x=1;
	
	ZeroMemory( tagWaveinfo.RiffHead, 4 );
	ZeroMemory( tagWaveinfo.WaveHead, 4 );
	ZeroMemory( tagWaveinfo.FileFormat, 4 );
	ZeroMemory( tagWaveinfo.FactChank, 4 );
	ZeroMemory( tagWaveinfo.DataChank, 4 );
	
	/* RIFFヘッダ情報 */
	if( fread( ID, 4, 1, fp ) != 1 ){
		return -1;
	}	
	size = _getUlong( fp );
		
	if( strncmp( ID, "RIFF", 4 ) != 0 ){
		return -1;
	}
	ID[4] = '\0';
	strcpy( tagWaveinfo.RiffHead, ID );
	tagWaveinfo.RiffHead[4]='\0';
	tagWaveinfo.RiffHeadSize = size; 
	
	/* WAVEヘッダ情報 */
	if( fread( ID, 4, 1, fp ) != 1 ){
		return -1;
	}
	if( strncmp( ID, "WAVE", 4 ) != 0 ){
		return -1;
	}
	ID[4] = '\0';
	strcpy( tagWaveinfo.WaveHead, ID );
	tagWaveinfo.WaveHead[4]='\0';
	
	/* チャンク情報 */
	while( fread( ID, 4, 1, fp ) == 1 ){ 
		ID[4] = '\0';
		if( strncmp( ID, "fmt ", 4 ) == 0 ){
			size = _getUlong( fp );
			ID[3]='\0';
			strcpy( tagWaveinfo.FileFormat, ID );
			tagWaveinfo.FileFormatSize = size;
			cursor = ftell(fp);
			_fmtChunkRead( fp );
		
		} else if( strncmp( ID, "fact", 4 ) == 0 ){
			//ID[4]='\0';
			size = _getUlong( fp );
			strncpy( tagWaveinfo.FactChank, ID, 4 );
			tagWaveinfo.FactChank[4]='\0';
			tagWaveinfo.FactChankSize = size;
			cursor = ftell(fp);
			_factChunkRead( fp );
		
		} else if( strncmp( ID, "data", 4 ) == 0 ){
			//ID[3]='\0';
			size = _getUlong( fp );
			strncpy( tagWaveinfo.DataChank, ID, 4 );
			tagWaveinfo.DataChank[4]='\0';
			tagWaveinfo.DataSize = size;	
		        cursor = ftell(fp);
			tagWaveinfo.Offset = ftell( fp );
		} else {
			break;
		}
		fseek(fp, cursor + size, SEEK_SET);
	}
	x *= tagWaveinfo.SampleBit/8;
	x *= tagWaveinfo.Channels;
	tagWaveinfo.DataNum = tagWaveinfo.DataSize/x; 
	return 0;
}